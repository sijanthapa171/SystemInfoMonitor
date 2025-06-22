#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include "../include/memory.h"

int init_memory_monitoring(void) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        return -1;
    }
    fclose(fp);
    return 0;
}

int get_memory_info(MemoryInfo *info) {
    FILE *fp;
    char line[256];
    
    memset(info, 0, sizeof(MemoryInfo));
    
    fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        return -1;
    }
    
    while (fgets(line, sizeof(line), fp)) {
        unsigned long value;
        if (sscanf(line, "MemTotal: %lu kB", &value) == 1) {
            info->total_ram = value * 1024;
        }
        else if (sscanf(line, "MemFree: %lu kB", &value) == 1) {
            info->free_ram = value * 1024;
        }
        else if (sscanf(line, "Cached: %lu kB", &value) == 1) {
            info->cached_ram = value * 1024;
        }
        else if (sscanf(line, "SwapTotal: %lu kB", &value) == 1) {
            info->swap_total = value * 1024;
        }
        else if (sscanf(line, "SwapFree: %lu kB", &value) == 1) {
            info->swap_free = value * 1024;
        }
    }
    
    fclose(fp);
    
    info->used_ram = info->total_ram - info->free_ram - info->cached_ram;
    info->swap_used = info->swap_total - info->swap_free;
    
    return 0;
}

void cleanup_memory_monitoring(void) {
    // Nothing to clean up for memory monitoring
} 