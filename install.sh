#!/bin/bash

# Step 1: Compile the spacenav program
echo "Compiling spacenav.c..."
gcc -o spacenav spacenav.c -I/usr/include/libevdev-1.0 -levdev -Wall
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting..."
    exit 1
fi

# Step 2: Install the binary
echo "Installing spacenav binary to /usr/local/bin..."
chmod +x spacenav
cp spacenav /usr/local/bin

# Step 3: Load the uinput module and ensure it's loaded at boot
echo "Loading uinput kernel module..."
modprobe uinput

if ! grep -q "^uinput$" /etc/modules-load.d/uinput.conf; then
    echo "Adding uinput to /etc/modules-load.d/uinput.conf..."
    echo "uinput" | tee -a /etc/modules-load.d/uinput.conf
fi

# Step 4: Install the systemd service
echo "Installing systemd service..."
cp spacenav.service /etc/systemd/system/spacenav.service

# Step 5: Install udev rules (if needed)
if [ -f 99-spacenav.rules ]; then
    echo "Installing udev rules..."
    cp 99-spacenav.rules /etc/udev/rules.d
    udevadm control --reload-rules && udevadm trigger
fi

# Step 6: Enable and start the service
echo "Enabling and starting the spacenav service..."
systemctl enable spacenav.service
systemctl start spacenav.service

echo "Installation complete!"

