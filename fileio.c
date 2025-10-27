#include <stdio.h>
#include <time.h>
#include "fileio.h"

static void timestamp(char *buf, int size) {
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm);
}

void logRunHeader(const char *filename, const char *title) {
    FILE *f = fopen(filename, "a");
    if (!f) return;
    char ts[64];
    timestamp(ts, sizeof(ts));
    fprintf(f, "\n=== %s at [%s] ===\n", title, ts);
    fclose(f);
}

void logAllocation(Zone z, int f, int m, int t, int unmetF, int unmetM, int unmetT) {
    FILE *f = fopen("allocation_log.txt", "a");
    if (!f) return;
    char ts[64];
    timestamp(ts, sizeof(ts));
    fprintf(f, "[%s] Zone %d -> Got F=%d M=%d T=%d | Unmet: %d %d %d\n",
            ts, z.id, f, m, t, unmetF, unmetM, unmetT);
    fclose(f);
}

void logHospital(const char *message) {
    FILE *f = fopen("hospital_log.txt", "a");
    if (!f) return;
    char ts[64];
    timestamp(ts, sizeof(ts));
    fprintf(f, "[%s] %s\n", ts, message);
    fclose(f);
}

void viewFile(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Could not open %s (file may not exist).\n", filename);
        return;
    }
    printf("\n--- Contents of %s ---\n\n", filename);
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    printf("\n--- End of %s ---\n", filename);
    fclose(f);
}
