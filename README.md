Here is a comprehensive **README** that explains everything to an end user about setting up and using the SpaceNav daemon:

---

# SpaceNav Daemon Installation and Setup Guide

This guide will walk you through installing and configuring the SpaceNav daemon on your Linux system, which allows you to use your 3Dconnexion SpaceMouse (or similar 3D input device) as a virtual joystick.

## Prerequisites

- A **3Dconnexion SpaceMouse** or a similar 6-axis device.
- A Linux-based operating system (tested on Arch Linux).
- `gcc` and required development libraries (like `libevdev`) installed.

## Overview

This project provides a daemon that listens for input events from a 3Dconnexion SpaceMouse and translates them into joystick input events. These events can be used by other applications that support joystick input, such as 3D modeling software or games.

The daemon runs as a system service and is designed to automatically start when the system boots.

## Features

- **6-axis support**: The SpaceMouse's 6 axes of movement are mapped to joystick axes.
- **Virtual joystick support**: The SpaceMouse is presented as a virtual joystick to other applications.
- **Uinput-based**: Uses the `uinput` kernel module to create virtual input devices.
- **Systemd integration**: Runs as a background service, starting automatically on boot.

---

## Installation Instructions

Follow these steps to install and set up the SpaceNav daemon:

### Step 1: Clone or Download the Repository

First, clone the repository or download the necessary files. You should have the following:

- `spacenav.c` (the main C program)
- `spacenav.service` (systemd service configuration)
- `99-spacenav.rules` (udev rules to ensure proper device permissions)

### Step 2: Run the Installation Script

The easiest way to install the daemon is by running the installation script provided. This script compiles the program, installs it, and sets up the systemd service.

1. Save the script as `install_spacenav.sh` on your system.
2. Make the script executable:
   ```bash
   chmod +x install_spacenav.sh
   ```
3. Run the script as `root`:
   ```bash
   sudo ./install_spacenav.sh
   ```

The script performs the following tasks:
- Compiles the `spacenav.c` program.
- Installs the compiled binary to `/usr/local/bin`.
- Loads the `uinput` kernel module and ensures it loads automatically at boot.
- Installs the systemd service for the SpaceNav daemon.
- Installs udev rules to ensure proper device permissions.
- Starts the service and enables it to start on boot.

### Step 3: Verify the Installation

After running the script, the SpaceNav daemon should be running. You can verify this with:

```bash
systemctl status spacenav.service
```

If the service is running, you should see output indicating that the daemon is active and running.

---

## How It Works

1. **SpaceNav Device Detection**:
   - The script detects your SpaceMouse by monitoring the input devices in `/dev/input`.
   - When the device is plugged in, the daemon listens for input events from the device.

2. **Event Translation**:
   - The daemon translates relative movement events from the SpaceMouse (like moving the device in space) into absolute joystick events, which can be used by applications that expect joystick input.
   - These events are sent through a virtual joystick device created by the `uinput` kernel module.

3. **Systemd Service**:
   - The daemon runs as a system service. It starts automatically when the system boots and ensures that the SpaceMouse is always available for use.

---

## Troubleshooting

### Device Not Detected
- Make sure your SpaceMouse is plugged in and detected by the system. You can check this with the following command:
  ```bash
  ls /dev/input
  ```
  Look for a device like `/dev/input/eventX`, where `X` is a number.

- Ensure that the `uinput` kernel module is loaded:
  ```bash
  lsmod | grep uinput
  ```

### Permissions Issues
- The service runs as `root`, so there shouldn't be permission issues. However, if you run the program manually as a user, you may need to ensure that your user has access to `/dev/input`.

### Systemd Service Fails to Start
- Check the systemd service logs for any errors:
  ```bash
  journalctl -u spacenav.service
  ```
  This will show detailed logs of what happened when the service started.

---

## Uninstallation Instructions

To uninstall the SpaceNav daemon, run the following steps:

1. **Stop the Service**:
   ```bash
   sudo systemctl stop spacenav.service
   ```

2. **Disable the Service**:
   ```bash
   sudo systemctl disable spacenav.service
   ```

3. **Remove Files**:
   - Remove the binary:
     ```bash
     sudo rm /usr/local/bin/spacenav
     ```

   - Remove the systemd service:
     ```bash
     sudo rm /etc/systemd/system/spacenav.service
     ```

   - Remove the udev rules (optional):
     ```bash
     sudo rm /etc/udev/rules.d/99-spacenav.rules
     ```

   - Remove the `uinput` configuration (optional):
     ```bash
     sudo sed -i '/uinput/d' /etc/modules-load.d/uinput.conf
     ```

4. **Unload `uinput` Module**:
   If you no longer need the `uinput` module:
   ```bash
   sudo modprobe -r uinput
   ```

---

## Additional Notes

- **Running the Service as `root`**: The SpaceNav daemon runs as `root` by default to avoid any permission issues with accessing input devices. This should not be a security concern, as it is explicitly controlled by the service and does not expose any unnecessary privileges.
  
- **Updating the Service**: If you need to update the software, you can re-run the installation script to overwrite the existing files.

---

## License

This project is released under the [MIT License](LICENSE).

---

## Contact

For any questions or issues, please feel free to open an issue on the project's GitHub page or contact the maintainer directly.

---

This **README** provides a complete guide for end users to install, configure, and troubleshoot the SpaceNav daemon.
