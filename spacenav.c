#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/input.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

int main() {
	struct libevdev *dev = NULL;
	struct libevdev_uinput *uidev = NULL;
	int fd;
	int rc;

	fd = open("/dev/input/by-id/usb-3Dconnexion_SpaceMouse_Compact-event-if00", O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		perror("Can't open device");
		return EXIT_FAILURE;
	}

	rc = libevdev_new_from_fd(fd, &dev);
	if (rc < 0) {
		fprintf(stderr, "Error initializing libevdev (%s)\n", strerror(-rc));
		close(fd);
		return EXIT_FAILURE;
	}

	rc = libevdev_grab(dev, LIBEVDEV_GRAB);
	if (rc < 0) {
		fprintf(stderr, "Error grabbing device (%s)\n", strerror(-rc));
		libevdev_free(dev);
		close(fd);
		return EXIT_FAILURE;
	}

	struct libevdev *virtual_dev = libevdev_new();
	libevdev_set_name(virtual_dev, "Spacenav");

	libevdev_enable_event_code(virtual_dev, EV_KEY, BTN_0, NULL);
	libevdev_enable_event_code(virtual_dev, EV_KEY, BTN_1, NULL);

	struct input_absinfo absinfo = {.minimum = -350, .maximum = 350, .value = 0};
	libevdev_enable_event_code(virtual_dev, EV_ABS, ABS_X, &absinfo);
	libevdev_enable_event_code(virtual_dev, EV_ABS, ABS_Y, &absinfo);
	libevdev_enable_event_code(virtual_dev, EV_ABS, ABS_Z, &absinfo);
	libevdev_enable_event_code(virtual_dev, EV_ABS, ABS_RX, &absinfo);
	libevdev_enable_event_code(virtual_dev, EV_ABS, ABS_RY, &absinfo);
	libevdev_enable_event_code(virtual_dev, EV_ABS, ABS_RZ, &absinfo);

	rc = libevdev_uinput_create_from_device(virtual_dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
	if (rc < 0) {
		fprintf(stderr, "Error creating uinput device (%s)\n", strerror(-rc));
		libevdev_free(virtual_dev);
		libevdev_free(dev);
		close(fd);
		return EXIT_FAILURE;
	}

	sleep(1);

	struct input_event ev;
	while (1) {
		rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
		if (rc == 0) {
			if (ev.type == EV_REL) {
				ev.type = EV_ABS;
				if (ev.code == REL_Z || ev.code == REL_Y || ev.code == REL_RY) {
					ev.value *= -1;
				}
				if (ev.value >= -20 && ev.value <= 20) {
					ev.value = 0;
				}
			}
			libevdev_uinput_write_event(uidev, ev.type, ev.code, ev.value);
			libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
		} else if (rc != -EAGAIN) {
			fprintf(stderr, "Error reading events (%s)\n", strerror(-rc));
			break;
		}
	}

	libevdev_uinput_destroy(uidev);
	libevdev_free(virtual_dev);
	libevdev_free(dev);
	close(fd);

	return EXIT_SUCCESS;
}
