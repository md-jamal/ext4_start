#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/fs.h>
#include <linux/types.h>

#define __nonstring

#define DISK_NAME "/dev/sda3"
//#define DISK_NAME "uma.ext4"

struct ext4_group_desc
{
	__le32  bg_block_bitmap_lo; /* Blocks bitmap block */
	__le32  bg_inode_bitmap_lo; /* Inodes bitmap block */
	__le32  bg_inode_table_lo;  /* Inodes table block */
	__le16  bg_free_blocks_count_lo;/* Free blocks count */
	__le16  bg_free_inodes_count_lo;/* Free inodes count */
	__le16  bg_used_dirs_count_lo;  /* Directories count */
	__le16  bg_flags;       /* EXT4_BG_flags (INODE_UNINIT, etc) */
	__le32  bg_exclude_bitmap_lo;   /* Exclude bitmap for snapshots */
	__le16  bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bbitmap) LE */
	__le16  bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+ibitmap) LE */
	__le16  bg_itable_unused_lo;    /* Unused inodes count */         
	__le16  bg_checksum;        /* crc16(sb_uuid+group+desc) */
	__le32  bg_block_bitmap_hi; /* Blocks bitmap block MSB */
	__le32  bg_inode_bitmap_hi; /* Inodes bitmap block MSB */
	__le32  bg_inode_table_hi;  /* Inodes table block MSB */
	__le16  bg_free_blocks_count_hi;/* Free blocks count MSB */
	__le16  bg_free_inodes_count_hi;/* Free inodes count MSB */
	__le16  bg_used_dirs_count_hi;  /* Directories count MSB */
	__le16  bg_itable_unused_hi;    /* Unused inodes count MSB */
	__le32  bg_exclude_bitmap_hi;   /* Exclude bitmap block MSB */
	__le16  bg_block_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bbitmap) BE */
	__le16  bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+ibitmap) BE */
	__u32   bg_reserved;
};

#define EXT4_N_BLOCKS 15

struct ext4_inode { 
	__le16  i_mode;     /* File mode */ 
	__le16  i_uid;      /* Low 16 bits of Owner Uid */ 
	__le32  i_size_lo;  /* Size in bytes */ 
	__le32  i_atime;    /* Access time */ 
	__le32  i_ctime;    /* Inode Change time */ 
	__le32  i_mtime;    /* Modification time */ 
	__le32  i_dtime;    /* Deletion Time */ 
	__le16  i_gid;      /* Low 16 bits of Group Id */ 
	__le16  i_links_count;  /* Links count */ 
	__le32  i_blocks_lo;    /* Blocks count */ 
	__le32  i_flags;    /* File flags */ 
	union { 
		struct { 
			__le32  l_i_version; 
		} linux1; 
		struct { 
			__u32  h_i_translator; 
		} hurd1; 
		struct { 
			__u32  m_i_reserved1; 
		} masix1; 
	} osd1;             /* OS dependent 1 */ 
	__le32  i_block[EXT4_N_BLOCKS];/* Pointers to blocks */ 
	__le32  i_generation;   /* File version (for NFS) */ 
	__le32  i_file_acl_lo;  /* File ACL */ 
	__le32  i_size_high; 
	__le32  i_obso_faddr;   /* Obsoleted fragment address */ 
	union { 
		struct { 
			__le16  l_i_blocks_high; /* were l_i_reserved1 */ 
			__le16  l_i_file_acl_high; 
			__le16  l_i_uid_high;   /* these 2 fields */ 
			__le16  l_i_gid_high;   /* were reserved2[0] */ 
			__le16  l_i_checksum_lo;/* crc32c(uuid+inum+inode) LE */ 
			__le16  l_i_reserved; 
		} linux2; 
		struct { 
			__le16  h_i_reserved1;  /* Obsoleted fragment number/size which are removed in ext4 */ 
			__u16   h_i_mode_high; 
			__u16   h_i_uid_high; 
			__u16   h_i_gid_high; 
			__u32   h_i_author; 
		} hurd2; 
		struct { 
			__le16  h_i_reserved1;  /* Obsoleted fragment number/size which are removed in ext4 */ 
			__le16  m_i_file_acl_high; 
			__u32   m_i_reserved2[2]; 
		} masix2; 
	} osd2;             /* OS dependent 2 */ 
	__le16  i_extra_isize; 
	__le16  i_checksum_hi;  /* crc32c(uuid+inum+inode) BE */ 
	__le32  i_ctime_extra;  /* extra Change time      (nsec << 2 | epoch) */ 
	__le32  i_mtime_extra;  /* extra Modification time(nsec << 2 | epoch) */ 
	__le32  i_atime_extra;  /* extra Access time      (nsec << 2 | epoch) */ 
	__le32  i_crtime;       /* File Creation time */ 
	__le32  i_crtime_extra; /* extra FileCreationtime (nsec << 2 | epoch) */ 
	__le32  i_version_hi;   /* high 32 bits for 64-bit version */ 
	__le32  i_projid;   /* Project ID */ 
}; 

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

	lseek(f, 64 * (sizeof(struct ext4_group_desc)/2), SEEK_CUR);

	for (; index < gd_count; index++){

		read(f, &gd, sizeof(struct ext4_group_desc)/2);

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

			printf ("itable %d\n", itable);

			memset(&gd, 0, sizeof (gd));
		}
	}

	lseek(f, 0, SEEK_SET);

	lseek(f, itable*4096L, SEEK_CUR);

	lseek(f, offset, SEEK_CUR);

	read(f, &ext4_inode, sizeof (struct ext4_inode));

	printf (" pid %d \n", getpid());

	while(1);
}

