#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
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

// Box drawing characters
#define TOP_LEFT "┌"
#define TOP_RIGHT "┐"
#define BOTTOM_LEFT "└"
#define BOTTOM_RIGHT "┘"
#define HORIZONTAL "─"
#define VERTICAL "│"
#define TEE_RIGHT "├"
#define TEE_LEFT "┤"

void print_header(const char* title, const char* color) {
    int width = 50;
    int padding = (width - strlen(title) - 2) / 2;
    
    printf("\n%s%s", BOLD, color);
    printf("%s", TOP_LEFT);
    for (int i = 0; i < width - 2; i++) printf("%s", HORIZONTAL);
    printf("%s\n", TOP_RIGHT);
    
    printf("%s ", VERTICAL);
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", title);
    for (int i = 0; i < padding - (strlen(title) % 2 == 0 ? 0 : 1); i++) printf(" ");
    printf(" %s\n", VERTICAL);
    
    printf("%s", TEE_RIGHT);
    for (int i = 0; i < width - 2; i++) printf("%s", HORIZONTAL);
    printf("%s%s\n", TEE_LEFT, RESET);
}

void print_footer(const char* color) {
    int width = 50;
    printf("%s%s%s", BOLD, color, BOTTOM_LEFT);
    for (int i = 0; i < width - 2; i++) printf("%s", HORIZONTAL);
    printf("%s%s\n", BOTTOM_RIGHT, RESET);
}

void print_value_line(const char* label, const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("%s%s %s", BOLD, VERTICAL, CYAN);
    printf("%-15s%s ", label, RESET);
    vprintf(format, args);
    printf("%s%s%s\n", BOLD, VERTICAL, RESET);
    va_end(args);
}

void print_cpu_info(CPUInfo *cpu) {
    print_header("CPU Information", BLUE);
    
    print_value_line("Model:", "%s", cpu->model_name);
    print_value_line("Architecture:", "%s", cpu->architecture);
    print_value_line("Cores:", "%d", cpu->num_cores);
    print_value_line("Threads:", "%d", cpu->num_threads);
    print_value_line("Frequency:", "%.2f MHz", cpu->current_freq);
    
    printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "CPU Usage:", RESET);
    printf("%s%.1f%%%s", 
           cpu->cpu_usage > 80 ? RED : (cpu->cpu_usage > 50 ? YELLOW : GREEN),
           cpu->cpu_usage, RESET);
    printf("%s%s%s\n", BOLD, VERTICAL, RESET);
    
    print_footer(BLUE);
}

void print_gpu_info(GPUInfo *gpu) {
    print_header("GPU Information", MAGENTA);
    
    print_value_line("Model:", "%s", gpu->model_name);
    print_value_line("Driver:", "%s", gpu->driver_version);
    print_value_line("Total Memory:", "%lu MB", gpu->memory_total / (1024*1024));
    
    printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "Used Memory:", RESET);
    printf("%s%lu MB%s", 
           (gpu->memory_used > gpu->memory_total * 0.8) ? RED : 
           (gpu->memory_used > gpu->memory_total * 0.5) ? YELLOW : GREEN,
           gpu->memory_used / (1024*1024), RESET);
    printf("%s%s%s\n", BOLD, VERTICAL, RESET);
    
    if (gpu->has_temperature) {
        printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "Temperature:", RESET);
        printf("%s%.1f°C%s", 
               gpu->temperature > 80 ? RED : (gpu->temperature > 60 ? YELLOW : GREEN),
               gpu->temperature, RESET);
        printf("%s%s%s\n", BOLD, VERTICAL, RESET);
    }
    
    print_footer(MAGENTA);
}

void print_memory_info(MemoryInfo *mem) {
    print_header("Memory Information", GREEN);
    
    print_value_line("Total RAM:", "%lu MB", mem->total_ram / (1024*1024));
    
    printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "Used RAM:", RESET);
    printf("%s%lu MB%s", 
           (mem->used_ram > mem->total_ram * 0.8) ? RED :
           (mem->used_ram > mem->total_ram * 0.5) ? YELLOW : GREEN,
           mem->used_ram / (1024*1024), RESET);
    printf("%s%s%s\n", BOLD, VERTICAL, RESET);
    
    printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "Free RAM:", RESET);
    printf("%s%lu MB%s", GREEN, mem->free_ram / (1024*1024), RESET);
    printf("%s%s%s\n", BOLD, VERTICAL, RESET);
    
    print_value_line("Cached RAM:", "%lu MB", mem->cached_ram / (1024*1024));
    print_value_line("Swap Total:", "%lu MB", mem->swap_total / (1024*1024));
    
    printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "Swap Used:", RESET);
    printf("%s%lu MB%s", 
           (mem->swap_used > mem->swap_total * 0.5) ? RED : GREEN,
           mem->swap_used / (1024*1024), RESET);
    printf("%s%s%s\n", BOLD, VERTICAL, RESET);
    
    print_footer(GREEN);
}

void print_storage_info(StorageInfo *storage) {
    print_header("Storage Information", YELLOW);
    
    for (int i = 0; i < storage->num_partitions; i++) {
        PartitionInfo *part = &storage->partitions[i];
        
        if (i > 0) {
            printf("%s%s", BOLD, YELLOW);
            printf("%s", TEE_RIGHT);
            for (int j = 0; j < 48; j++) printf("%s", HORIZONTAL);
            printf("%s%s\n", TEE_LEFT, RESET);
        }
        
        print_value_line("Mount Point:", "%s", part->mount_point);
        print_value_line("Device:", "%s", part->device);
        print_value_line("Total Space:", "%lu GB", part->total_space / (1024*1024*1024));
        
        printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "Used Space:", RESET);
        printf("%s%lu GB%s", 
               (part->usage_percent > 90) ? RED :
               (part->usage_percent > 70) ? YELLOW : GREEN,
               part->used_space / (1024*1024*1024), RESET);
        printf("%s%s%s\n", BOLD, VERTICAL, RESET);
        
        printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "Free Space:", RESET);
        printf("%s%lu GB%s", GREEN, part->free_space / (1024*1024*1024), RESET);
        printf("%s%s%s\n", BOLD, VERTICAL, RESET);
        
        printf("%s%s %s%-15s%s ", BOLD, VERTICAL, CYAN, "Usage:", RESET);
        printf("%s%.1f%%%s", 
               (part->usage_percent > 90) ? RED :
               (part->usage_percent > 70) ? YELLOW : GREEN,
               part->usage_percent, RESET);
        printf("%s%s%s\n", BOLD, VERTICAL, RESET);
    }
    
    print_footer(YELLOW);
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