#ifndef CPU_H
#define CPU_H

typedef struct {
    char model_name[256];
    int num_cores;
    int num_threads;
    double current_freq;
    double min_freq;
    double max_freq;
    double cpu_usage;
    char architecture[64];
} CPUInfo;

// Initialize CPU monitoring
int init_cpu_monitoring(void);

// Get current CPU information
int get_cpu_info(CPUInfo *info);

// Clean up CPU monitoring resources
void cleanup_cpu_monitoring(void);

#endif // CPU_H 