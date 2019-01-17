#include "ext4.h"

int main (int argc, char *argv[])
{
	int f;
	int index = 0;
	struct ext4_group_desc gd ;

	if (argc < 2) {
		printf ("Give me Disk name \n");
		return -1;
	}

	f = open(DISK_NAME, O_RDONLY);

	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}

	int gd_count = get_gd_count(f);

	memset(&gd, 0 , (sizeof(struct ext4_group_desc)));

	//printf ("gd size %d\n", gd_count);

	lseek(f, 1024+1024, SEEK_CUR);

	// leave out these x86 boot sector and padding && //readout superblock 

	lseek(f, 32 * (sizeof(struct ext4_group_desc)), SEEK_CUR);

	int size_of_gd = get_desc_size(f);

	for (; index < gd_count; index++){

		read(f, &gd, size_of_gd);

		if  (gd.bg_checksum) {

			print_gd_info(index, &gd) ;

			memset(&gd, 0, sizeof (gd));
		}
	}

	return 0;
}

