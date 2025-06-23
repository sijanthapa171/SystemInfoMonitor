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

int init_cpu_monitoring(void);

int get_cpu_info(CPUInfo *info);

void cleanup_cpu_monitoring(void);

#endif 