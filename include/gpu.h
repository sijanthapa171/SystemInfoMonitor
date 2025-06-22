#ifndef GPU_H
#define GPU_H

typedef struct {
    char model_name[256];
    char driver_version[64];
    unsigned long memory_total;
    unsigned long memory_used;
    double temperature;
    int has_temperature;
} GPUInfo;

int init_gpu_monitoring(void);

int get_gpu_info(GPUInfo *info);

void cleanup_gpu_monitoring(void);

#endif 