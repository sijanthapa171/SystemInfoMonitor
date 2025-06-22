#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/cpu.h"
#include "../include/gpu.h"
#include "../include/memory.h"
#include "../include/storage.h"

void print_cpu_info(CPUInfo *cpu) {
    printf("\n=== CPU Information ===\n");
    printf("Model: %s\n", cpu->model_name);
    printf("Architecture: %s\n", cpu->architecture);
    printf("Cores: %d\n", cpu->num_cores);
    printf("Threads: %d\n", cpu->num_threads);
    printf("Current Frequency: %.2f MHz\n", cpu->current_freq);
    printf("CPU Usage: %.1f%%\n", cpu->cpu_usage);
}

void print_gpu_info(GPUInfo *gpu) {
    printf("\n=== GPU Information ===\n");
    printf("Model: %s\n", gpu->model_name);
    printf("Driver Version: %s\n", gpu->driver_version);
    printf("Memory Total: %lu MB\n", gpu->memory_total / (1024*1024));
    printf("Memory Used: %lu MB\n", gpu->memory_used / (1024*1024));
    if (gpu->has_temperature) {
        printf("Temperature: %.1f°C\n", gpu->temperature);
    }
}

void print_memory_info(MemoryInfo *mem) {
    printf("\n=== Memory Information ===\n");
    printf("Total RAM: %lu MB\n", mem->total_ram / (1024*1024));
    printf("Used RAM: %lu MB\n", mem->used_ram / (1024*1024));
    printf("Free RAM: %lu MB\n", mem->free_ram / (1024*1024));
    printf("Cached RAM: %lu MB\n", mem->cached_ram / (1024*1024));
    printf("Swap Total: %lu MB\n", mem->swap_total / (1024*1024));
    printf("Swap Used: %lu MB\n", mem->swap_used / (1024*1024));
}

void print_storage_info(StorageInfo *storage) {
    printf("\n=== Storage Information ===\n");
    for (int i = 0; i < storage->num_partitions; i++) {
        PartitionInfo *part = &storage->partitions[i];
        printf("\nMount Point: %s\n", part->mount_point);
        printf("Device: %s\n", part->device);
        printf("Total Space: %lu GB\n", part->total_space / (1024*1024*1024));
        printf("Used Space: %lu GB\n", part->used_space / (1024*1024*1024));
        printf("Free Space: %lu GB\n", part->free_space / (1024*1024*1024));
        printf("Usage: %.1f%%\n", part->usage_percent);
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