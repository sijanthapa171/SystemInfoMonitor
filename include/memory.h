#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    unsigned long total_ram;
    unsigned long used_ram;
    unsigned long free_ram;
    unsigned long cached_ram;
    unsigned long swap_total;
    unsigned long swap_used;
    unsigned long swap_free;
} MemoryInfo;

int init_memory_monitoring(void);

int get_memory_info(MemoryInfo *info);

void cleanup_memory_monitoring(void);

#endif 