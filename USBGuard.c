#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <libudev.h>
#include <time.h>

#define LOG_FILE_PATH "/root/USBGuard_log.txt"

void createLogEntry(const char *usbUuid) {
    time_t now;
    struct tm *localTime;
    char timeString[80];

    time(&now);
    localTime = localtime(&now);
    strftime(timeString, 80, "%Y-%m-%d %H:%M:%S", localTime);

    FILE *logFile = fopen(LOG_FILE_PATH, "a");
    if (logFile != NULL) {
        fprintf(logFile, "Date: %s, USB UUID: %s\n", timeString, usbUuid);
        fclose(logFile);
    } 
}

int main() {
    
     // Check if the effective user ID is not equal to 0 (root user)
    if (geteuid() != 0) {
    printf("This program requires root privileges to run.\n");
    exit(1);
	}
    
    struct udev *udev = udev_new();
    if (!udev) {
        fprintf(stderr, "Failed to create udev\n");
        return 1;
    }

    struct udev_monitor *mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", "disk");
    udev_monitor_enable_receiving(mon);

    int fd = udev_monitor_get_fd(mon);

    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        if (select(fd + 1, &fds, NULL, NULL, NULL) > 0 && FD_ISSET(fd, &fds)) {
            struct udev_device *dev = udev_monitor_receive_device(mon);
            if (dev) {
                const char *usbUuid = udev_device_get_property_value(dev, "ID_SERIAL");
                createLogEntry(usbUuid);
                system("shutdown -h now");
                udev_device_unref(dev);
            }
        }
    }

    udev_unref(udev);
    return 0;
}
}
