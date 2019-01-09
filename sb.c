#include "ext4.h"

int main (int argc, char *argv[])
{
	struct ext4_super_block es;
	int f;
	char but[1024];

	f = open("/dev/sda3", O_RDONLY);
	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}
	read(f, but, 1024);  
	read(f, &es, sizeof(struct ext4_super_block));

	return 0;
}

