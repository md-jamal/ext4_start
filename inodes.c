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
	int itable ;
	int Ibitmap = 0 ;
	struct ext4_inode ext4_inode;
	struct ext4_group_desc gd ;

	unsigned char _Ibitmap[8192/8] = {} ;

	if (argc < 3) {
		printf ("Throw me an Disk name and Inode boss !!\n");
		return 1;
	}

	f = open(DISK_NAME, O_RDONLY);

	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}

	memset(&ext4_inode, 0, sizeof (ext4_inode));

	memset(&gd, 0, sizeof (gd));

	int inode = atoi(argv[2]) -1;

	int group = inode/8192 ;

	int index_1 = inode % 8192;

	int offset = index_1 * 256;

	int gd_count = get_gd_count(f);

	//printf ("gd size %d\n", gd_count);

	lseek(f, 1024+1024, SEEK_CUR);

	// leave out these x86 boot sector and padding && //readout superblock 

	lseek(f, 32 * (sizeof(struct ext4_group_desc)), SEEK_CUR);

	int size_of_gd = get_desc_size(f);

	printf ("%d group\n", group);

	lseek(f, group*size_of_gd, SEEK_CUR);

#if 1
	
	//for (; index < gd_count; index++){

	read(f, &gd, size_of_gd);

	if  (gd.bg_checksum /*&& index == group*/) {

		printf("Group %d: ", group);

		printf("block bitmap at %d, inode bitmap at %d, inode table at %d\n",
				(((gd.bg_block_bitmap_hi) << 32) | gd.bg_block_bitmap_lo),
				(((gd.bg_inode_bitmap_hi) << 32) | gd.bg_inode_bitmap_lo),
				(((gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo));

		printf("\t %d free blocks, %d free inodes, %d used directories,"
				" %d unused inodes\n", 
				(((gd.bg_free_blocks_count_hi) << 16) | gd.bg_free_blocks_count_lo),
				(((gd.bg_free_inodes_count_hi) << 16) | gd.bg_free_inodes_count_lo),
				(((gd.bg_used_dirs_count_hi) << 16) | gd.bg_used_dirs_count_lo),
				(((gd.bg_itable_unused_hi) << 16) | gd.bg_itable_unused_lo));

		printf("\t Checksum %x\n\n", gd.bg_checksum);

		itable = (((gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo);

		{

			Ibitmap = (((gd.bg_inode_bitmap_hi) << 32) | gd.bg_inode_bitmap_lo) ;

			lseek(f, Ibitmap*4096L,SEEK_SET);

			read(f, _Ibitmap, sizeof (_Ibitmap));

			printf ("Itable at %d\n", itable);
			printf ("Ibitmap at %d\n", Ibitmap);

			int loop_i = 0 ;
			int loop_j = 0 ;

			for(; loop_i < sizeof (_Ibitmap); loop_i++) {
				if (_Ibitmap[loop_i]) {
					//							printf("%x\n",_Ibitmap[loop_i]);
					for(loop_j=0; loop_j < sizeof(char)*8; loop_j++){
						if (_Ibitmap[loop_i] & (1<<loop_j)) {
							printf ("Inode busy %d\n", loop_i*8 + loop_j +1+(8192*group));
						}
					}
				}
			}

		}

		memset(&gd, 0, sizeof (gd));
	}
	//}

#endif

	lseek(f, 0, SEEK_SET);

	lseek(f, itable*4096L, SEEK_CUR);

	lseek(f, offset, SEEK_CUR);

	read(f, &ext4_inode, sizeof (struct ext4_inode));

	printf ("pid %d \n", getpid());

	while(1);
}

