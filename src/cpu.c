#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "../include/cpu.h"

static unsigned long long prev_idle = 0;
static unsigned long long prev_total = 0;

int init_cpu_monitoring(void) {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        return -1;
    }
    fclose(fp);
    return 0;
}

static void get_cpu_stats(unsigned long long *idle, unsigned long long *total) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        return;
    }

    char line[256];
    if (fgets(line, sizeof(line), fp)) {
        unsigned long long user, nice, system, idle_time, iowait, irq, softirq, steal;
        sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
               &user, &nice, &system, &idle_time, &iowait, &irq, &softirq, &steal);
        
        *idle = idle_time + iowait;
        *total = user + nice + system + idle_time + iowait + irq + softirq + steal;
    }
    
    fclose(fp);
}

static double calculate_cpu_usage(void) {
    unsigned long long idle = 0, total = 0;
    get_cpu_stats(&idle, &total);
    
    unsigned long long idle_delta = idle - prev_idle;
    unsigned long long total_delta = total - prev_total;
    
    double cpu_usage = 0.0;
    if (total_delta != 0) {
        cpu_usage = 100.0 * (1.0 - ((double)idle_delta / total_delta));
    }
    
    prev_idle = idle;
    prev_total = total;
    
    return cpu_usage;
}

int get_cpu_info(CPUInfo *info) {
    FILE *fp;
    char line[256];
    int found_model = 0;
    info->num_threads = 0;  
    
    fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        return -1;
    }
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "model name", 10) == 0 && !found_model) {
            char *value = strchr(line, ':');
            if (value) {
                value++; 
                while (*value == ' ') value++; 
                strcpy(info->model_name, value);
                char *newline = strchr(info->model_name, '\n');
                if (newline) *newline = '\0';
                found_model = 1;
            }
        }
        else if (strncmp(line, "processor", 9) == 0) {
            info->num_threads++;
        }
        else if (strncmp(line, "cpu cores", 9) == 0) {
            sscanf(strchr(line, ':') + 1, "%d", &info->num_cores);
        }
    }
    fclose(fp);
    
    fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
    if (fp) {
        if (fscanf(fp, "%lf", &info->current_freq) == 1) {
            info->current_freq /= 1000.0; 
        }
        fclose(fp);
    }
    
    // Get architecture
    #ifdef __x86_64__
    strcpy(info->architecture, "x86_64");
    #elif defined(__i386__)
    strcpy(info->architecture, "x86");
    #elif defined(__aarch64__)
    strcpy(info->architecture, "ARM64");
    #elif defined(__arm__)
    strcpy(info->architecture, "ARM");
    #else
    strcpy(info->architecture, "Unknown");
    #endif
    
    info->cpu_usage = calculate_cpu_usage();
    
    return 0;
}

void cleanup_cpu_monitoring(void) {
    // Nothing to clean up for CPU monitoring
} 