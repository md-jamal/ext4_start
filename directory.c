#include "ext4.h"

struct ext4_dir_entry_2 ext4_dir ;

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
	int itable ;
	struct ext4_inode ext4_inode;
	struct ext4_group_desc gd ;

	if (argc < 2) {
		printf ("Throw me an Inode boss !!\n");
		return 1;
	}

	f = open(DISK_NAME, O_RDONLY);

	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}

	int inode = atoi(argv[1]) -1;

	int group = inode/8192 ;

	int index_1 = inode % 8192;

	int offset = index_1 * 256;

	int gd_count = get_gd_count(f);

	//printf ("gd size %d\n", gd_count);

	lseek(f, 1024+1024, SEEK_CUR);

	// leave out these x86 boot sector and padding && //readout superblock 

	lseek(f, 32 * (sizeof(struct ext4_group_desc)), SEEK_CUR);

	int size_of_gd = get_desc_size(f);

	for (; index < gd_count; index++){

		read(f, &gd, size_of_gd);

		if  (gd.bg_checksum && index == group) {

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

			itable = (((gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo);

			memset(&gd, 0, sizeof (gd));
		}
	}

	lseek(f, 0, SEEK_SET);

	lseek(f, itable*4096L, SEEK_CUR);

	lseek(f, offset, SEEK_CUR);

	read(f, &ext4_inode, sizeof (struct ext4_inode));

#if 1
	int loop_index = 0 ;

	for (index=1; index < EXT4_N_BLOCKS ; index++) {
		printf (" block pointer %d = %d\n", index, ext4_inode.i_block[index]);
		if (ext4_inode.i_block[index] > 99999) {
			lseek(f, 0, SEEK_SET);
			lseek(f, ext4_inode.i_block[index]*4096L, SEEK_CUR);

			for (loop_index = 0; loop_index < 5 ; loop_index++) {
				read(f, &ext4_dir, sizeof (ext4_dir));
				printf ("%.*s \n", ext4_dir.name_len, ext4_dir.name);
				lseek(f, -(sizeof (ext4_dir) - ext4_dir.rec_len), SEEK_CUR);
				//printf ("%llx\n", lseek(f, 0, SEEK_CUR));
			}

		}
	}
#endif

	printf (" pid %d \n", getpid());

	while(1);
}

