#ifndef STORAGE_H
#define STORAGE_H

#define MAX_PARTITIONS 16
#define MAX_PATH_LEN 256

typedef struct {
    char mount_point[MAX_PATH_LEN];
    char device[MAX_PATH_LEN];
    unsigned long total_space;
    unsigned long used_space;
    unsigned long free_space;
    double usage_percent;
} PartitionInfo;

typedef struct {
    PartitionInfo partitions[MAX_PARTITIONS];
    int num_partitions;
} StorageInfo;

int init_storage_monitoring(void);

int get_storage_info(StorageInfo *info);

void cleanup_storage_monitoring(void);

#endif 