#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/reboot.h>

#define LOG_FILE "/var/log/test.txt"
#define USB_DEVICE_PATH "/dev/bus/usb"

void log_event(struct udev_device *dev) {
    FILE *log_fp;
    char uuid[256];
    time_t now;
    struct tm *tm;

    // Get the device UUID
    udev_device_get_property_value(dev, "ID_SERIAL", uuid, sizeof(uuid));

    // Open the log file
    log_fp = fopen(LOG_FILE, "a");
    if (!log_fp) {
        perror("fopen");
        return;
    }

    // Get the current date and time
    now = time(NULL);
    tm = localtime(&now);

    // Log the event
    fprintf(log_fp, "%04d-%02d-%02d %02d:%02d:%02d: USB device plugged in: %s\n",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, uuid);

    // Close the log file
    fclose(log_fp);

    // Shut down the computer immediately
    syslog(LOG_INFO, "Shutting down the computer...");
    sync();
    reboot(RB_AUTOBOOT);
}

int main() {
    struct udev *udev;
    struct udev_monitor *mon;
    struct udev_device *dev;

    // Create a udev context
    udev = udev_new();
    if (!udev) {
        perror("udev_new");
        return 1;
    }

    // Create a udev monitor
    mon = udev_monitor_new_from_netlink(udev, "udev");
    if (!mon) {
        perror("udev_monitor_new_from_netlink");
        udev_unref(udev);
        return 1;
    }

    // Set up the monitor to receive events for USB devices
    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", NULL);
    udev_monitor_enable_receiving(mon);

    // Run indefinitely and monitor for USB device events
    while (1) {
        dev = udev_monitor_receive_device(mon);
        if (dev) {
            const char *action = udev_device_get_action(dev);
            const char *path = udev_device_get_devpath(dev);

            if (strcmp(action, "add") == 0 && strstr(path, USB_DEVICE_PATH)) {
                log_event(dev);
            }

            udev_device_unref(dev);
        }
    }

    // Clean up
    udev_monitor_unref(mon);
    udev_unref(udev);

    return 0;
}
