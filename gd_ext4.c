#include "ext4.h"

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

	lseek(f, 32 * (sizeof(struct ext4_group_desc)), SEEK_CUR);

	int size_of_gd = get_desc_size(f);

	for (; index < gd_count; index++){

		read(f, &gd, size_of_gd);

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

