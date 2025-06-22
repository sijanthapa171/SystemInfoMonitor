#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/gpu.h"

#define BUFFER_SIZE 1024

static char nvidia_smi_available = -1; 

int init_gpu_monitoring(void) {
    FILE *fp = popen("which nvidia-smi 2>/dev/null", "r");
    if (fp) {
        char path[256];
        if (fgets(path, sizeof(path), fp) != NULL) {
            nvidia_smi_available = 1;
        } else {
            nvidia_smi_available = 0;
        }
        pclose(fp);
    }
    
    return 0;
}

static void get_nvidia_gpu_info(GPUInfo *info) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    
    fp = popen("nvidia-smi --query-gpu=name --format=csv,noheader", "r");
    if (fp) {
        if (fgets(buffer, sizeof(buffer), fp)) {
            char *newline = strchr(buffer, '\n');
            if (newline) *newline = '\0';
            strncpy(info->model_name, buffer, sizeof(info->model_name) - 1);
            info->model_name[sizeof(info->model_name) - 1] = '\0';
        }
        pclose(fp);
    }
    
    fp = popen("nvidia-smi --query-gpu=driver_version --format=csv,noheader", "r");
    if (fp) {
        if (fgets(buffer, sizeof(buffer), fp)) {
            char *newline = strchr(buffer, '\n');
            if (newline) *newline = '\0';
            strncpy(info->driver_version, buffer, sizeof(info->driver_version) - 1);
            info->driver_version[sizeof(info->driver_version) - 1] = '\0';
        }
        pclose(fp);
    }
    
    fp = popen("nvidia-smi --query-gpu=memory.total,memory.used --format=csv,noheader,nounits", "r");
    if (fp) {
        unsigned long total, used;
        if (fscanf(fp, "%lu,%lu", &total, &used) == 2) {
            info->memory_total = total * 1024 * 1024; 
            info->memory_used = used * 1024 * 1024;
        }
        pclose(fp);
    }
    
    fp = popen("nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader,nounits", "r");
    if (fp) {
        if (fscanf(fp, "%lf", &info->temperature) == 1) {
            info->has_temperature = 1;
        }
        pclose(fp);
    }
}

static void get_generic_gpu_info(GPUInfo *info) {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    
    fp = popen("lspci | grep -i 'vga\\|3d\\|2d'", "r");
    if (fp) {
        if (fgets(buffer, sizeof(buffer), fp)) {
            char *start = strchr(buffer, ':');
            if (start) {
                start += 2; 
                char *newline = strchr(start, '\n');
                if (newline) *newline = '\0';
                strncpy(info->model_name, start, sizeof(info->model_name) - 1);
            }
        }
        pclose(fp);
    }
    
    strcpy(info->driver_version, "Unknown");
    info->memory_total = 0;
    info->memory_used = 0;
    info->has_temperature = 0;
}

int get_gpu_info(GPUInfo *info) {
    memset(info, 0, sizeof(GPUInfo));
    
    if (nvidia_smi_available == 1) {
        get_nvidia_gpu_info(info);
    } else {
        get_generic_gpu_info(info);
    }
    
    return 0;
}

void cleanup_gpu_monitoring(void) {
    // Nothing to clean up for GPU monitoring
} 