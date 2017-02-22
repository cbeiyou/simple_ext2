
/*建立一个ext2 磁盘文件 */

#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#include <fcntl.h>
#include "/home/bei/ext2_you/include/file.h"
#include "/home/bei/ext2_you/include/tou.h"
#define EXT2_VALID_FS 1 /*干净未挂载状态*/
#define EXT2_ERROR_FS 2 /*文件系统处在挂载状态*/
int main()
{
	int fd;
	if ((fd=open("/home/bei/ext2_you/ext2_disk",O_RDWR|O_CREAT,0666)) == -1){
		printf("creat or read ext2_disk error\n");
		return 0;
	}
	//printf("%d \n",fd);
	//把ext2_disk文件偏移往后1024b，预留出1kb的引导块空间
	if (lseek(fd,1024,SEEK_CUR)==-1){
		printf("ext2_disk lseek error\n");
		return 0;
	}

	struct ext2_super_block super_block;
	memset(&super_block,'\0',sizeof(struct ext2_super_block));
	super_block.s_inodes_count=8192;//inode节点总数
	super_block.s_blocks_count=8192;//1kb一个block ，总共是8M的block
	super_block.s_r_blocks_count=10;//为超级用户保留块数
	super_block.s_free_blocks_count=8182;//空闲block
	super_block.s_free_inodes_count=8192;//空闲inode
	super_block.s_first_data_block=1;//第一个block
	time_t timenow;
	time(&timenow);
	super_block.s_mtime=timenow;//最后一次安装操作时间
	super_block.s_wtime=timenow;//最后一次对该超级块写操作时间
	super_block.s_mnt_count=0;//安装计数
	super_block.s_max_mnt_count=100;//最大安装计数
	super_block.s_state=EXT2_VALID_FS;//文件系统状态
	super_block.s_errors=1;
	super_block.s_lastcheck=timenow;
	super_block.s_checkinterval=10;
	super_block.s_rev_level=1;
	super_block.s_first_ino=0;
	super_block.s_inode_size=128;
	super_block.s_block_group_nr=0;
	//...先要把super_block加载到disk里面
	if ((write(fd,&super_block,sizeof(struct ext2_super_block)))==-1 ){
		printf("ext2_disk super_block write error\n");
		perror("write super block");
		exit(EXIT_FAILURE);
		return 0;
	}
	
	if (lseek(fd,1024+1024,SEEK_SET)==-1){
		printf("lseek error\n")	;
		return 0;
	}
	struct ext2_group_desc group_desc;
	group_desc.bg_block_bitmap=4;
	group_desc.bg_inode_bitmap=3;
	group_desc.bg_inode_table=5;
	group_desc.bg_free_blocks_count=super_block.s_free_blocks_count;
	group_desc.bg_free_inodes_count=super_block.s_free_inodes_count;
	//加载group_desc
	if (write(fd,&group_desc,sizeof(group_desc))==-1 ){
		printf("ext2_disk group_desc write error\n");
		return 0;
	}
	if (lseek(fd,1024+1024+1024,SEEK_SET)==-1){
		printf("lseek error\n")	;
		return 0;
	}
	char ok[1024];
	memset(ok,'\0',sizeof(ok));
	int i;
	for (i=0;i<=1+1+1024+8192;i++){
		if(write(fd,ok,sizeof(ok))==-1){
			printf("ext2_disk write bitmap inode block error\n");
			return 0;
		}
	}
	close(fd);
	return 0;
}












