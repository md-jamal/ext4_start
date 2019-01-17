#include "ext4.h"

struct ext4_dir_entry_2 ext4_dir ;

#define DISK_NAME "/dev/sda3"

char path[] = "saraboju/mIsc/ext4/gd_ext4.c" ;

int main (int argc, char *argv[])
{
	int f;

	int tinode = 2;

	f = open(DISK_NAME, O_RDONLY);

	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}

	char* token = strtok(path, "/"); 

	while (token != NULL) { 
		tinode = parent_I(f,tinode,token);
		if (tinode == -1) {
			printf("seems wrong \n");
			exit(1);	
		}
		printf("got %d\n", tinode);
		token = strtok(NULL, "/"); 
	} 

}

int parent_I(int f, int inode_p, char *component_p)
{
	int index = 0;
	int itable ;
	struct ext4_inode ext4_inode;
	struct ext4_group_desc gd ;

	memset(&gd, 0, sizeof (gd));
	memset(&ext4_inode, 0, sizeof (struct ext4_inode));

	int inode = inode_p -1;

	int group = inode/8192 ;

	int index_1 = inode % 8192;

	int offset = index_1 * 256;

	int gd_count = get_gd_count(f);

	//printf ("gd size %d\n", gd_count);

	lseek(f, 0, SEEK_SET);

	lseek(f, 1024+1024, SEEK_CUR);

	// leave out these x86 boot sector and padding && //readout superblock 

	lseek(f, 32 * (sizeof(struct ext4_group_desc)), SEEK_CUR);

	int size_of_gd = get_desc_size(f);

	lseek(f, group*size_of_gd, SEEK_CUR);

	index = group ;

	//for (; index < gd_count; index++){

		read(f, &gd, size_of_gd);

		if  (gd.bg_checksum && index == group) {

			//print_gd_info(index, &gd) ;

			itable = (((ext4_64_t)(gd.bg_inode_table_hi) << 32) | gd.bg_inode_table_lo);

			memset(&gd, 0, sizeof (gd));
		}
	//}

	lseek(f, 0, SEEK_SET);

	lseek(f, itable*4096L, SEEK_CUR);

	lseek(f, offset, SEEK_CUR);

	read(f, &ext4_inode, sizeof (struct ext4_inode));

	if ( ! S_ISDIR(ext4_inode.i_mode)) {
			printf ("Not a Directory\n");
			return -1 ;
	}

#if 1
	int loop_index = 0 ;

	int size = 0;

	int size_of_dir = 0 ;

	int tot_size = 0;

	size_of_dir= ((ext4_inode.i_size_lo) |(ext4_64_t)(ext4_inode.i_size_high) <<32) ;

	for (index=1; index < EXT4_N_BLOCKS  && tot_size < size_of_dir; index++) {
		DEBUG_EXT4 (" block pointer %d = %d\n", index, ext4_inode.i_block[index]);
		if (ext4_inode.i_block[index] > 30) {
			lseek(f, 0, SEEK_SET);
			lseek(f, ext4_inode.i_block[index]*4096L, SEEK_CUR);

			//for (loop_index = 0; loop_index < 5 ; loop_index++) {
			size = 0 ;
			while( size < 4096 ) {
				read(f, &ext4_dir, sizeof (ext4_dir));
				//printf ("%.*s \n", ext4_dir.name_len, ext4_dir.name);
				if ( !strncmp(ext4_dir.name, component_p, ext4_dir.name_len)) {
					return  ext4_dir.inode ;
				} 
				lseek(f, -(sizeof (ext4_dir) - ext4_dir.rec_len), SEEK_CUR);
				size+=ext4_dir.rec_len;
			}
		 	tot_size += size ;
				//printf ("%llx\n", lseek(f, 0, SEEK_CUR));
			//}

		}
	}
#endif

}

