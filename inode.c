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
	struct ext4_inode ext4_inode;
	struct ext4_group_desc gd ;

	if (argc < 3) {
		printf ("Throw me an Disk name Inode boss !!\n");
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
	
	for (; index < gd_count; index++){

		read(f, &gd, size_of_gd);

		if  (gd.bg_checksum && index == group) {

			print_gd_info(index, &gd) ;

			itable = (((ext4_64_t)(gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo);

			DEBUG_EXT4 ("Itable at %d\n", itable);

			memset(&gd, 0, sizeof (gd));
		}
	}

	lseek(f, 0, SEEK_SET);

	lseek(f, itable*4096L, SEEK_CUR);

	lseek(f, offset, SEEK_CUR);

	read(f, &ext4_inode, sizeof (struct ext4_inode));

	printf ("pid %d \n", getpid());

	convert_epoch(ext4_inode.i_atime);
	convert_epoch(ext4_inode.i_mtime);
	convert_epoch(ext4_inode.i_ctime);
	convert_epoch(ext4_inode.i_dtime);

	while(1);
}

