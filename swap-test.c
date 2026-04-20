#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Change this to the device name of your physical swap (e.g., "/dev/nvme0n1p3" or "/dev/sda3")
#define PHYSICAL_SWAP_DEVICE "/dev/sda3" 
#define CHUNK_SIZE_MB 256
#define SLEEP_INTERVAL_SEC 1

float get_physical_swap_usage() {
    FILE *fp = fopen("/proc/swaps", "r");
    if (!fp) return 0.0;

    char line[256];
    char dev[64], type[64];
    long size, used;
    int prio;
    float usage = 0.0;

    // Skip header
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%s %s %ld %ld %d", dev, type, &size, &used, &prio) == 5) {
            if (strcmp(dev, PHYSICAL_SWAP_DEVICE) == 0) {
                usage = (float)used / size;
                break;
            }
        }
    }
    fclose(fp);
    return usage;
}

int main() {
    printf("Starting zram/Swap stress test...\n");
    printf("Targeting: %s at 30%% usage.\n", PHYSICAL_SWAP_DEVICE);

    size_t total_allocated = 0;

    while (1) {
        float current_usage = get_physical_swap_usage();
        printf("Physical Swap Usage: %.2f%%\r", current_usage * 100);
        fflush(stdout);

        if (current_usage >= 0.30) {
            printf("\nTarget reached (30%%)! Stopping allocation.\n");
            break;
        }

        // Allocate memory and touch it to force it into RAM/zram
        void *new_chunk = malloc(CHUNK_SIZE_MB * 1024 * 1024);
        if (new_chunk) {
            memset(new_chunk, 1, CHUNK_SIZE_MB * 1024 * 1024); // Mandatory to trigger swap
            total_allocated += CHUNK_SIZE_MB;
        } else {
            printf("\nFailed to allocate more memory.\n");
            break;
        }

        sleep(SLEEP_INTERVAL_SEC);
    }

    printf("Total allocated: %zu MB. Press Enter to exit and free memory...", total_allocated);
    getchar();
    return 0;
}

