#include <stdio.h>
#include <stdlib.h>
#include <sys/sd-device.h> // newer API for USB, #include<libudev.h> is the older one for systemD

int main() {
    sd_device_monitor *monitor = NULL;
    sd_device *device = NULL;

    sd_device_monitor_new("udev", &monitor);
    sd_device_monitor_attach_event(monitor, SD_DEVICE_EVENT_CHANGE);
    sd_device_monitor_attach_event(monitor, SD_DEVICE_EVENT_REMOVE);

    while (1) {
        if (sd_device_monitor_receive(monitor, &device) < 0) {
            perror("sd_device_monitor_receive");
            break;
        }

        if (device) {
            const char *action = sd_device_get_action(device);
            if (action && strcmp(action, "add") == 0) {
                printf("Hello, a new device was inserted!\n");
            }
        }

        sd_device_unref(device);
    }

    sd_device_monitor_unref(monitor);
    return 0;
}
