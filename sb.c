#include "ext4.h"

int main (int argc, char *argv[])
{
	struct ext4_super_block es;
	int f;
	char but[1024];

	if (argc < 2)
		return -1 ;
	f = open(DISK_NAME, O_RDONLY);
	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}
	read(f, but, 1024);  
	read(f, &es, sizeof(struct ext4_super_block));
	
	if (es.s_feature_incompat & 0x40) {
			printf("ext4\n");
	}else if(es.s_feature_incompat & 0x4) {
			printf ("ext3\n");	
	}else {
		printf ("maybe ext2\n");
	}

	return 0;
}

