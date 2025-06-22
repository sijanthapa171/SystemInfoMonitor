#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <mntent.h>
#include "../include/storage.h"

int init_storage_monitoring(void) {
    FILE *fp = fopen("/proc/mounts", "r");
    if (!fp) {
        return -1;
    }
    fclose(fp);
    return 0;
}

static int is_valid_fs(const char *fs_type) {
    const char *valid_types[] = {
        "ext2", "ext3", "ext4", "xfs", "btrfs", "zfs",
        "ntfs", "vfat", "exfat", NULL
    };
    
    for (const char **type = valid_types; *type; type++) {
        if (strcmp(fs_type, *type) == 0) {
            return 1;
        }
    }
    return 0;
}

int get_storage_info(StorageInfo *info) {
    FILE *mtab;
    struct mntent *mount_entry;
    struct statvfs fs_stats;
    
    info->num_partitions = 0;
    
    mtab = setmntent("/proc/mounts", "r");
    if (!mtab) {
        return -1;
    }
    
    while ((mount_entry = getmntent(mtab)) != NULL && info->num_partitions < MAX_PARTITIONS) {
        if (!strncmp(mount_entry->mnt_fsname, "/dev/", 5) && is_valid_fs(mount_entry->mnt_type)) {
            PartitionInfo *part = &info->partitions[info->num_partitions];
            
            strncpy(part->mount_point, mount_entry->mnt_dir, MAX_PATH_LEN - 1);
            strncpy(part->device, mount_entry->mnt_fsname, MAX_PATH_LEN - 1);
            
            if (statvfs(mount_entry->mnt_dir, &fs_stats) == 0) {
                unsigned long block_size = fs_stats.f_bsize;
                part->total_space = (unsigned long)fs_stats.f_blocks * block_size;
                part->free_space = (unsigned long)fs_stats.f_bfree * block_size;
                part->used_space = part->total_space - part->free_space;
                
                if (part->total_space > 0) {
                    part->usage_percent = ((double)part->used_space / part->total_space) * 100.0;
                } else {
                    part->usage_percent = 0.0;
                }
                
                info->num_partitions++;
            }
        }
    }
    
    endmntent(mtab);
    return 0;
}

void cleanup_storage_monitoring(void) {
    // Nothing to clean up for storage monitoring
} 