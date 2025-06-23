#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/cpu.h"
#include "../include/gpu.h"
#include "../include/memory.h"
#include "../include/storage.h"

// Color codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

void print_cpu_info(CPUInfo *cpu) {
    printf("\n%s%s=== CPU Information ===%s\n", BOLD, BLUE, RESET);
    printf("%sModel:%s %s\n", CYAN, RESET, cpu->model_name);
    printf("%sArchitecture:%s %s\n", CYAN, RESET, cpu->architecture);
    printf("%sCores:%s %d\n", CYAN, RESET, cpu->num_cores);
    printf("%sThreads:%s %d\n", CYAN, RESET, cpu->num_threads);
    printf("%sCurrent Frequency:%s %.2f MHz\n", CYAN, RESET, cpu->current_freq);
    printf("%sCPU Usage:%s %s%.1f%%%s\n", CYAN, RESET, 
           cpu->cpu_usage > 80 ? RED : (cpu->cpu_usage > 50 ? YELLOW : GREEN),
           cpu->cpu_usage, RESET);
}

void print_gpu_info(GPUInfo *gpu) {
    printf("\n%s%s=== GPU Information ===%s\n", BOLD, MAGENTA, RESET);
    printf("%sModel:%s %s\n", CYAN, RESET, gpu->model_name);
    printf("%sDriver Version:%s %s\n", CYAN, RESET, gpu->driver_version);
    printf("%sMemory Total:%s %lu MB\n", CYAN, RESET, gpu->memory_total / (1024*1024));
    printf("%sMemory Used:%s %s%lu MB%s\n", CYAN, RESET, 
           (gpu->memory_used > gpu->memory_total * 0.8) ? RED : 
           (gpu->memory_used > gpu->memory_total * 0.5) ? YELLOW : GREEN,
           gpu->memory_used / (1024*1024), RESET);
    if (gpu->has_temperature) {
        printf("%sTemperature:%s %s%.1f°C%s\n", CYAN, RESET,
               gpu->temperature > 80 ? RED : (gpu->temperature > 60 ? YELLOW : GREEN),
               gpu->temperature, RESET);
    }
}

void print_memory_info(MemoryInfo *mem) {
    printf("\n%s%s=== Memory Information ===%s\n", BOLD, GREEN, RESET);
    printf("%sTotal RAM:%s %lu MB\n", CYAN, RESET, mem->total_ram / (1024*1024));
    printf("%sUsed RAM:%s %s%lu MB%s\n", CYAN, RESET,
           (mem->used_ram > mem->total_ram * 0.8) ? RED :
           (mem->used_ram > mem->total_ram * 0.5) ? YELLOW : GREEN,
           mem->used_ram / (1024*1024), RESET);
    printf("%sFree RAM:%s %s%lu MB%s\n", CYAN, RESET, GREEN, mem->free_ram / (1024*1024), RESET);
    printf("%sCached RAM:%s %lu MB\n", CYAN, RESET, mem->cached_ram / (1024*1024));
    printf("%sSwap Total:%s %lu MB\n", CYAN, RESET, mem->swap_total / (1024*1024));
    printf("%sSwap Used:%s %s%lu MB%s\n", CYAN, RESET,
           (mem->swap_used > mem->swap_total * 0.5) ? RED : GREEN,
           mem->swap_used / (1024*1024), RESET);
}

void print_storage_info(StorageInfo *storage) {
    printf("\n%s%s=== Storage Information ===%s\n", BOLD, YELLOW, RESET);
    for (int i = 0; i < storage->num_partitions; i++) {
        PartitionInfo *part = &storage->partitions[i];
        printf("\n%sMount Point:%s %s\n", CYAN, RESET, part->mount_point);
        printf("%sDevice:%s %s\n", CYAN, RESET, part->device);
        printf("%sTotal Space:%s %lu GB\n", CYAN, RESET, part->total_space / (1024*1024*1024));
        printf("%sUsed Space:%s %s%lu GB%s\n", CYAN, RESET,
               (part->usage_percent > 90) ? RED :
               (part->usage_percent > 70) ? YELLOW : GREEN,
               part->used_space / (1024*1024*1024), RESET);
        printf("%sFree Space:%s %s%lu GB%s\n", CYAN, RESET, GREEN,
               part->free_space / (1024*1024*1024), RESET);
        printf("%sUsage:%s %s%.1f%%%s\n", CYAN, RESET,
               (part->usage_percent > 90) ? RED :
               (part->usage_percent > 70) ? YELLOW : GREEN,
               part->usage_percent, RESET);
    }
}

int main(int argc, char *argv[]) {
    CPUInfo cpu;
    GPUInfo gpu;
    MemoryInfo memory;
    StorageInfo storage;
    int continuous_mode = 0;
    int interval = 1; 

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--continuous") == 0) {
            continuous_mode = 1;
        } else if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interval") == 0) && i + 1 < argc) {
            interval = atoi(argv[i + 1]);
            i++;
        }
    }

    if (init_cpu_monitoring() != 0 ||
        init_gpu_monitoring() != 0 ||
        init_memory_monitoring() != 0 ||
        init_storage_monitoring() != 0) {
        fprintf(stderr, "Failed to initialize monitoring systems\n");
        return 1;
    }

    do {
        if (continuous_mode) {
            printf("\033[2J\033[H");
        }

        if (get_cpu_info(&cpu) == 0) {
            print_cpu_info(&cpu);
        }
        
        if (get_gpu_info(&gpu) == 0) {
            print_gpu_info(&gpu);
        }
        
        if (get_memory_info(&memory) == 0) {
            print_memory_info(&memory);
        }
        
        if (get_storage_info(&storage) == 0) {
            print_storage_info(&storage);
        }

        if (continuous_mode) {
            sleep(interval);
        }
    } while (continuous_mode);

    cleanup_cpu_monitoring();
    cleanup_gpu_monitoring();
    cleanup_memory_monitoring();
    cleanup_storage_monitoring();

    return 0;
} 