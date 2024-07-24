#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/reboot.h>
#include "tinyusb.h"

#define LOG_FILE "/var/log/test.txt"

void log_event(tusb_device_t *dev) {
    FILE *log_fp;
    char uuid[256];
    time_t now;
    struct tm *tm;

    // Get the device UUID
    tusb_device_get_property_value(dev, "ID_SERIAL", uuid, sizeof(uuid));

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
    tusb_context_t *ctx;
    tusb_device_t *dev;

    // Initialize the TinyUSB context
    ctx = tusb_new();
    if (!ctx) {
        perror("tusb_new");
        return 1;
    }

    // Open a USB device
    dev = tusb_open(ctx);
    if (!dev) {
        perror("tusb_open");
        tusb_free(ctx);
        return 1;
    }

    // Set up the device to receive events for USB devices
    tusb_device_filter_add_match_subsystem_devtype(dev, "usb", NULL);
    tusb_device_enable_receiving(dev);

    // Run indefinitely and monitor for USB device events
    while (1) {
        dev = tusb_device_receive_device(dev);
        if (dev) {
            const char *action = tusb_device_get_action(dev);
            const char *path = tusb_device_get_devpath(dev);

            if (strcmp(action, "add") == 0 && strstr(path, USB_DEVICE_PATH)) {
                log_event(dev);
            }

            tusb_device_free(dev);
        }
    }

    // Clean up
    tusb_device_free(dev);
    tusb_free(ctx);

    return 0;
}
