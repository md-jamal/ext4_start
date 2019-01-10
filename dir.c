#include "ext4.h"

void hexdump(char *buf, int size)
{
	int i = 0;
	for (; size; i++)
		printf("%x%c", buf[size--]&0xff, (i%128)?' ':'\n');
}

char 
is_group_special (int group)
{
	return 0;
}

struct ext4_dir_entry_2 dir ;



int main (int argc, char *argv[])
{
	struct ext4_super_block es;
	struct ext4_group_desc gd ;
	struct ext4_inode in;
	int f;
	char but[1024];
	int i=0;
	long int x = 0;
	long long int y =0;
	char       buf[80];

	if (argc < 3) {
		printf ("Throw me an Disk name and Inode boss !!\n");
		return 1;
	}

	//f = open("uma.ext4", O_RDONLY);
	f = open("/dev/sda3", O_RDONLY);
	if ( f == -1  ) {
		printf ("run with root permissions %x\n", sizeof (in));
		return -1;
	}

	int inode = atoi(argv[2]) -1;

	int group = inode/8192 ;

	int index = inode % 8192;

	int offset = index * 256;

	if (!is_group_special(group)) {
	}

	int block = (5*5*5*5) *128 * 1024 * 1024 / 4096; //_val(argv[1]);

	lseek(f, block*4096, SEEK_CUR);
	lseek(f, 1024, SEEK_CUR);

	for ( i=0; i<1118+64; i++){
		read(f, &gd, sizeof(struct ext4_group_desc)/2);
		if  ( i > 95 && gd.bg_checksum && (i-96) == group) {

#if 1				
printf("Group %d: ", i-96);
			printf("block bitmap at %d, inode bitmap at %d, inode table at %d\n",
					(((gd.bg_block_bitmap_hi) << 32) | gd.bg_block_bitmap_lo),
					(((gd.bg_inode_bitmap_hi) << 32) | gd.bg_inode_bitmap_lo),
					(((gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo));
			printf("\t %d free blocks, %d free inodes, %d used directories, %d unused inodes\n",(((gd.bg_free_blocks_count_hi) << 16) | gd.bg_free_blocks_count_lo),
					(((gd.bg_free_inodes_count_hi) << 16) | gd.bg_free_inodes_count_lo),
					(((gd.bg_used_dirs_count_hi) << 16) | gd.bg_used_dirs_count_lo),
					(((gd.bg_itable_unused_hi) << 16) | gd.bg_itable_unused_lo));
#endif
			printf("\t Checksum %x index %d\n\n", gd.bg_checksum,i-96);

			//x = (((gd.bg_free_inodes_count_hi) << 16) | gd.bg_free_inodes_count_lo);
			//x = (((gd.bg_free_blocks_count_hi) << 16) | gd.bg_free_blocks_count_lo);
			x = (((gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo);
			//x = (((gd.bg_block_table_hi) << 32) | gd.bg_block_table_lo);
			//x = (((gd.bg_inode_bitmap_hi) << 32) | gd.bg_inode_bitmap_lo);
			//x = (((gd.bg_block_bitmap_hi) << 32) | gd.bg_block_bitmap_lo);
			//printf(" %d %d\n", x,i-64);
			y+=x;
			memset(&gd, 0, 64);
		}
	}

	lseek(f, 0, SEEK_SET);
	lseek(f, x*4096, SEEK_CUR);
	lseek(f, offset, SEEK_CUR);
	read(f, &in, sizeof (struct ext4_inode));

	int an_i = 0 ;

	for (i=1; i < EXT4_N_BLOCKS ; i++) {
		printf (" block pointer %d = %d\n", i, in.i_block[i]);
		if (in.i_block[i] > 99999) {
			lseek(f, 0, SEEK_SET);
			lseek(f, in.i_block[i]*4096L, SEEK_CUR);

			for (an_i = 0; an_i < 20 ; an_i++) {
				read(f, &dir, sizeof ( dir ));
				//printf ("name %.*s inode %d reclen %d name_len %d ft %d\n", dir.name_len, dir.name, dir.inode, dir.rec_len, dir.name_len, dir.file_type);
				printf ("%.*s \n", dir.name_len, dir.name);
				lseek(f, -(sizeof (dir) - dir.rec_len), SEEK_CUR);
				printf ("%llx\n", lseek(f, 0, SEEK_CUR));
			}

		}
	}

#if 0
    ts = *localtime(&in.i_atime);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    printf("%s\n", buf);
	memset(buf, 0, 80);

    ts = *localtime(&in.i_ctime);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    printf("%s\n", buf);
	memset(buf, 0, 80);

    ts = *localtime(&in.i_mtime);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    printf("%s\n", buf);
#endif

	printf (" pid %d \n", getpid());
	while(1);

	return 0;
}

