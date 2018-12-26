#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/fs.h>
#include <linux/types.h>

#define __nonstring

#define DISK_NAME "/dev/sda3"
//#define DISK_NAME "uma.ext4"

struct ext4_group_desc
{
	__le32  bg_block_bitmap_lo; /* Blocks bitmap block */
	__le32  bg_inode_bitmap_lo; /* Inodes bitmap block */
	__le32  bg_inode_table_lo;  /* Inodes table block */
	__le16  bg_free_blocks_count_lo;/* Free blocks count */
	__le16  bg_free_inodes_count_lo;/* Free inodes count */
	__le16  bg_used_dirs_count_lo;  /* Directories count */
	__le16  bg_flags;       /* EXT4_BG_flags (INODE_UNINIT, etc) */
	__le32  bg_exclude_bitmap_lo;   /* Exclude bitmap for snapshots */
	__le16  bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bbitmap) LE */
	__le16  bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+ibitmap) LE */
	__le16  bg_itable_unused_lo;    /* Unused inodes count */         
	__le16  bg_checksum;        /* crc16(sb_uuid+group+desc) */
	__le32  bg_block_bitmap_hi; /* Blocks bitmap block MSB */
	__le32  bg_inode_bitmap_hi; /* Inodes bitmap block MSB */
	__le32  bg_inode_table_hi;  /* Inodes table block MSB */
	__le16  bg_free_blocks_count_hi;/* Free blocks count MSB */
	__le16  bg_free_inodes_count_hi;/* Free inodes count MSB */
	__le16  bg_used_dirs_count_hi;  /* Directories count MSB */
	__le16  bg_itable_unused_hi;    /* Unused inodes count MSB */
	__le32  bg_exclude_bitmap_hi;   /* Exclude bitmap block MSB */
	__le16  bg_block_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bbitmap) BE */
	__le16  bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+ibitmap) BE */
	__u32   bg_reserved;
};


int get_gd_count (int fd)
{
	struct stat dev;
	long long int size ;
	int extra = 0 ;
	
	long int group_size = (128*1024*1024L);

	if (ioctl(fd, BLKGETSIZE64, &size)) {
		if ( -1 == fstat(fd, &dev)) {
			return -1 ;
		} else {
			size = dev.st_size ;
		}
	}

	extra = size % (128*1024*1024L);
	
	return (size/(group_size)) + (extra?1:0) ;
}


int main (int argc, char *argv[])
{
	int f;
	int index = 0;
	struct ext4_group_desc gd ;

	f = open(DISK_NAME, O_RDONLY);

	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}

	int gd_count = get_gd_count(f);

	//printf ("gd size %d\n", gd_count);

	lseek(f, 1024+1024, SEEK_CUR);

	// leave out these x86 boot sector and padding && //readout superblock 

	lseek(f, 64 * (sizeof(struct ext4_group_desc)/2), SEEK_CUR);

	for (; index < gd_count; index++){

		read(f, &gd, sizeof(struct ext4_group_desc)/2);

		if  (gd.bg_checksum) {

			printf("Group %d: ", index);

			printf("block bitmap at %d, inode bitmap at %d, inode table at %d\n",
					(((gd.bg_block_bitmap_hi) << 32) | gd.bg_block_bitmap_lo),
					(((gd.bg_inode_bitmap_hi) << 32) | gd.bg_inode_bitmap_lo),
					(((gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo));

			printf("\t %d free blocks, %d free inodes, %d used directories,"
					" %d unused inodes\n", 
					(((gd.bg_free_blocks_count_hi) << 16) | gd.bg_free_blocks_count_lo),
					(((gd.bg_free_inodes_count_hi) << 16) | gd.bg_free_inodes_count_lo),
					(((gd.bg_used_dirs_count_hi) << 16) | gd.bg_used_dirs_count_lo),
					-1);

			printf("\t Checksum %x\n\n", gd.bg_checksum);

			memset(&gd, 0, sizeof (gd));
		}
	}

	return 0;
}

