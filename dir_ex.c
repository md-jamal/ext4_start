#include "ext4.h"

unsigned long long size_of_dir = 0;
unsigned long long cur_size = 0;

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

void hexdump(char *buf, int size)
{
	int index = 0 ;
#if 0
	printf("entered %d\n", size);
	printf("%u %u|n", file_size, cur_size);
	return ;
#endif
	for (; size ; size--)
		printf("%c",*buf++);
}

struct ext4_dir_entry_2 ext4_dir ;

char ft(char t)
{
	if (EXT4_FT_REG_FILE == t)
		return 'f';
	else if (EXT4_FT_DIR == t)
		return 'd';
	else if (EXT4_FT_SYMLINK == t)
		return 'l';
	else if (EXT4_FT_CHRDEV == t)
		return 'c';
	else if (EXT4_FT_BLKDEV == t)
		return 'b';
	else
		return' ';
}
		

void print_content_dir(int f, int length, unsigned long long off)
{
	unsigned long long _off = lseek(f, 0, SEEK_CUR);

	lseek(f, off*4096L, SEEK_SET);

	char buffer[4096] = {} ;
	
	int j = 0 ;

	int length_1 = 0; 

	{

		int loop_index = 0 ;

		int size = 0;

		int size_of_dir = 0 ;

		int tot_size = 0;

		size = 0 ;
		while( size < 4096 ) {
			read(f, &ext4_dir, sizeof (ext4_dir));
			printf ("%10d %c %.*s \n", ext4_dir.inode,ft(ext4_dir.file_type),
											ext4_dir.name_len, ext4_dir.name);
			lseek(f, -(sizeof (ext4_dir) - ext4_dir.rec_len), SEEK_CUR);
			size+=ext4_dir.rec_len;
		}
		tot_size += size ;

	}

	lseek(f, _off, SEEK_SET);
	
}

void parse_ex_tree(int f, unsigned long long off) 
{
	struct ext4_extent_header eh ;
	struct ext4_extent_idx    ehid;
	struct ext4_extent        ex ;
	struct ext4_extent        *exp ;

	if (off)
		lseek(f, off*4096L, SEEK_SET);

	read(f, &eh, sizeof (eh));

	if (eh.eh_depth == 0 ) {
		int j = 0;
		for (; j<eh.eh_entries; j++) {
			read(f, &ex, sizeof(ex));
			
			print_content_dir(f, ex.ee_len, ex.ee_start_lo | (ex.ee_start_hi << 32));
		}
	}else {
		int j = 0;
		for (; j<eh.eh_entries; j++) {
			read(f, &ehid, sizeof(ehid));
			parse_ex_tree( f, ehid.ei_leaf_lo| (ehid.ei_leaf_hi <<32) );
		}

	}
}
int main (int argc, char *argv[])
{
	int f;
	int index = 0;
	int itable ;
	char content[4096] = {} ;
	struct ext4_inode ext4_inode;
	struct ext4_group_desc gd ;

	struct ext4_extent_header eh ;
	struct ext4_extent_idx    ehid;
	struct ext4_extent        ex ;
	struct ext4_extent        *exp ;

	int f_size = 0;

	if (argc < 3) {
		printf ("Throw me an Disk name and Inode boss !!\n");
		return 1;
	}

	f = open(DISK_NAME, O_RDONLY);

	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}

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

			//print_gd_info(index, &gd) ;

			itable = (((ext4_64_t)(gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo);

			memset(&gd, 0, sizeof (gd));
		}
	}

	int constant = 1 ;

	lseek(f, 0, SEEK_SET);

	lseek(f, itable*4096L, SEEK_CUR);

	lseek(f, offset, SEEK_CUR);

	memset(&ext4_inode, 0, sizeof (struct ext4_inode));

#define offsetof(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)

	unsigned long long _off = read(f, 0, SEEK_CUR);

	read(f, &ext4_inode, sizeof (struct ext4_inode));

	if (  !S_ISDIR(ext4_inode.i_mode) ) {
		printf ("Not a Directory\n");
		return -1 ;
	}

		int loop_index = 0 ;
		lseek(f, -sizeof (struct ext4_inode), SEEK_CUR);
		lseek(f, offsetof(struct ext4_inode, i_block), SEEK_CUR);

		size_of_dir = ((ext4_inode.i_size_high << 32) | ext4_inode.i_size_lo);
		parse_ex_tree(f, 0);

	return ;
	printf ("pid %d \n", getpid());

	while(1);
}

