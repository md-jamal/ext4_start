#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/types.h>
#include <stdio.h>
#include <time.h>

#define __nonstring

struct ext4_super_block {
	/*00*/	__le32	s_inodes_count;		/* Inodes count */
	__le32	s_blocks_count_lo;	/* Blocks count */
	__le32	s_r_blocks_count_lo;	/* Reserved blocks count */
	__le32	s_free_blocks_count_lo;	/* Free blocks count */
	/*10*/	__le32	s_free_inodes_count;	/* Free inodes count */
	__le32	s_first_data_block;	/* First Data Block */
	__le32	s_log_block_size;	/* Block size */
	__le32	s_log_cluster_size;	/* Allocation cluster size */
	/*20*/	__le32	s_blocks_per_group;	/* # Blocks per group */
	__le32	s_clusters_per_group;	/* # Clusters per group */
	__le32	s_inodes_per_group;	/* # Inodes per group */
	__le32	s_mtime;		/* Mount time */
	/*30*/	__le32	s_wtime;		/* Write time */
	__le16	s_mnt_count;		/* Mount count */
	__le16	s_max_mnt_count;	/* Maximal mount count */
	__le16	s_magic;		/* Magic signature */
	__le16	s_state;		/* File system state */
	__le16	s_errors;		/* Behaviour when detecting errors */
	__le16	s_minor_rev_level;	/* minor revision level */
	/*40*/	__le32	s_lastcheck;		/* time of last check */
	__le32	s_checkinterval;	/* max. time between checks */
	__le32	s_creator_os;		/* OS */
	__le32	s_rev_level;		/* Revision level */
	/*50*/	__le16	s_def_resuid;		/* Default uid for reserved blocks */
	__le16	s_def_resgid;		/* Default gid for reserved blocks */
	/*
	 * These fields are for EXT4_DYNAMIC_REV superblocks only.
	 *
	 * Note: the difference between the compatible feature set and
	 * the incompatible feature set is that if there is a bit set
	 * in the incompatible feature set that the kernel doesn't
	 * know about, it should refuse to mount the filesystem.
	 *
	 * e2fsck's requirements are more strict; if it doesn't know
	 * about a feature in either the compatible or incompatible
	 * feature set, it must abort and not try to meddle with
	 * things it doesn't understand...
	 */
	__le32	s_first_ino;		/* First non-reserved inode */
	__le16  s_inode_size;		/* size of inode structure */
	__le16	s_block_group_nr;	/* block group # of this superblock */
	__le32	s_feature_compat;	/* compatible feature set */
	/*60*/	__le32	s_feature_incompat;	/* incompatible feature set */
	__le32	s_feature_ro_compat;	/* readonly-compatible feature set */
	/*68*/	__u8	s_uuid[16];		/* 128-bit uuid for volume */
	/*78*/	char	s_volume_name[16];	/* volume name */
	/*88*/	char	s_last_mounted[64] __nonstring;	/* directory where last mounted */
	/*C8*/	__le32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT4_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
	__le16	s_reserved_gdt_blocks;	/* Per group desc for online growth */
	/*
	 * Journaling support valid if EXT4_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
	/*D0*/	__u8	s_journal_uuid[16];	/* uuid of journal superblock */
	/*E0*/	__le32	s_journal_inum;		/* inode number of journal file */
	__le32	s_journal_dev;		/* device number of journal file */
	__le32	s_last_orphan;		/* start of list of inodes to delete */
	__le32	s_hash_seed[4];		/* HTREE hash seed */
	__u8	s_def_hash_version;	/* Default hash version to use */
	__u8	s_jnl_backup_type;
	__le16  s_desc_size;		/* size of group descriptor */
	/*100*/	__le32	s_default_mount_opts;
	__le32	s_first_meta_bg;	/* First metablock block group */
	__le32	s_mkfs_time;		/* When the filesystem was created */
	__le32	s_jnl_blocks[17];	/* Backup of the journal inode */
	/* 64bit support valid if EXT4_FEATURE_COMPAT_64BIT */
	/*150*/	__le32	s_blocks_count_hi;	/* Blocks count */
	__le32	s_r_blocks_count_hi;	/* Reserved blocks count */
	__le32	s_free_blocks_count_hi;	/* Free blocks count */
	__le16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__le16	s_want_extra_isize; 	/* New inodes should reserve # bytes */
	__le32	s_flags;		/* Miscellaneous flags */
	__le16  s_raid_stride;		/* RAID stride */
	__le16  s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__le64  s_mmp_block;            /* Block for multi-mount protection */
	__le32  s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_per_flex;  /* FLEX_BG group size */
	__u8	s_checksum_type;	/* metadata checksum algorithm used */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__le64	s_kbytes_written;	/* nr of lifetime kilobytes written */
	__le32	s_snapshot_inum;	/* Inode number of active snapshot */
	__le32	s_snapshot_id;		/* sequential ID of active snapshot */
	__le64	s_snapshot_r_blocks_count; /* reserved blocks for active
										  snapshot's future use */
	__le32	s_snapshot_list;	/* inode number of the head of the
								   on-disk snapshot list */
#define EXT4_S_ERR_START offsetof(struct ext4_super_block, s_error_count)
	__le32	s_error_count;		/* number of fs errors */
	__le32	s_first_error_time;	/* first time an error happened */
	__le32	s_first_error_ino;	/* inode involved in first error */
	__le64	s_first_error_block;	/* block involved of first error */
	__u8	s_first_error_func[32] __nonstring;	/* function where the error happened */
	__le32	s_first_error_line;	/* line number where error happened */
	__le32	s_last_error_time;	/* most recent time of an error */
	__le32	s_last_error_ino;	/* inode involved in last error */
	__le32	s_last_error_line;	/* line number where error happened */
	__le64	s_last_error_block;	/* block involved of last error */
	__u8	s_last_error_func[32] __nonstring;	/* function where the error happened */
#define EXT4_S_ERR_END offsetof(struct ext4_super_block, s_mount_opts)
	__u8	s_mount_opts[64];
	__le32	s_usr_quota_inum;	/* inode for tracking user quota */
	__le32	s_grp_quota_inum;	/* inode for tracking group quota */
	__le32	s_overhead_clusters;	/* overhead blocks/clusters in fs */
	__le32	s_backup_bgs[2];	/* groups with sparse_super2 SBs */
	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32	s_prj_quota_inum;	/* inode for tracking project quota */
	__le32	s_checksum_seed;	/* crc32c(uuid) if csum_seed set */
	__u8	s_wtime_hi;
	__u8	s_mtime_hi;
	__u8	s_mkfs_time_hi;
	__u8	s_lastcheck_hi;
	__u8	s_first_error_time_hi;
	__u8	s_last_error_time_hi;
	__u8	s_pad[2];
	__le32	s_reserved[96];		/* Padding to the end of the block */
	__le32	s_checksum;		/* crc32c(superblock) */
};

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
#define EXT4_NAME_LEN 255
struct ext4_dir_entry_2 {
    __le32  inode;          /* Inode number */
    __le16  rec_len;        /* Directory entry length */
    __u8    name_len;       /* Name length */
    __u8    file_type;
    char    name[EXT4_NAME_LEN];    /* File name */
};


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

#define EXT4_FT_UNKNOWN     0
#define EXT4_FT_REG_FILE    1
#define EXT4_FT_DIR     2
#define EXT4_FT_CHRDEV      3
#define EXT4_FT_BLKDEV      4
#define EXT4_FT_FIFO        5
#define EXT4_FT_SOCK        6
#define EXT4_FT_SYMLINK     7

#define EXT4_FT_MAX     8

#define EXT4_FT_DIR_CSUM    0xDE


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
	 struct tm  ts;

	if (argc < 2) {
		printf ("Throw me an Inode boss !!\n");
		return 1;
	}

	//f = open("uma.ext4", O_RDONLY);
	f = open("/dev/sda3", O_RDONLY);
	if ( f == -1  ) {
		printf ("run with root permissions %x\n", sizeof (in));
		return -1;
	}

	int inode = atoi(argv[1]) -1;

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
					10);
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

