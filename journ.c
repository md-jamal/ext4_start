#include "ext4.h"

int main (int argc, char *argv[])
{
	int f;

	journal_superblock_t jorn ;

	memset(&jorn,0,sizeof (jorn));

	f = open("/dev/sda3", O_RDONLY);
	if ( f == -1  ) {
		printf ("run with root permissions \n");
		return -1;
	}
	long long int block = 17858560*4096L; //_val(argv[1]);

	lseek(f, block , SEEK_CUR);
	
	read(f, &jorn, sizeof(jorn));

	printf ("Getpid %d\n", getpid());
	
	while(1);

	return 0;
}

