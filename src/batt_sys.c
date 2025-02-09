#include "batt_sys.h"
#include <glib/gstdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SYSFS_BATTERY_CAPACITY "/sys/class/power_supply/BAT0/capacity"
#define TMP_BATTERY_STATUS "/tmp/battery_status"

int read_battery_capacity() {
    char buffer[16];
    int capacity = -1;

    // Try reading from /tmp/battery_status first
    FILE *file = fopen(TMP_BATTERY_STATUS, "r");
    if (file) {
        if (fgets(buffer, sizeof(buffer), file)) {
            capacity = atoi(buffer);
        }
        fclose(file);

        if (capacity >= 0 && capacity <= 100) {
            return capacity;
        }
    }

    // Fallback to sysfs
    file = fopen(SYSFS_BATTERY_CAPACITY, "r");
    if (file) {
        if (fgets(buffer, sizeof(buffer), file)) {
            capacity = atoi(buffer);
        }
        fclose(file);
    }

    return (capacity >= 0 && capacity <= 100) ? capacity : -1;
}

void battery_update(battery *b) {
    if (!b) return;

    b->charge_now = read_battery_capacity();
    if (b->charge_now == -1) {
        fprintf(stderr, "Warning: Could not read battery capacity.\n");
    } else {
        printf("Battery capacity: %d%%\n", b->charge_now);
    }
}
