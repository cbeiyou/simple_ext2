//对于位图的操作
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define ext2_disk "/home/bei/ext2_you/ext2_disk"
#include "/home/bei/ext2_you/include/file.h"
#include "/home/bei/ext2_you/include/tou.h"
//#define __u32 unsigned int 
//bitmap 都是32位的，一个位用来判断一个块或者一个inode是否被占用，总共有8192个块，需要256个bitmap
__u32 inodemap[256];
__u32 blockmap[256];
struct ext2_super_block super_block;

int get_super_block_data(struct ext2_super_block *super_block){
    int fd;
    if((fd = open(ext2_disk,O_RDONLY))==-1){
        printf("read ext2_super_block error\n");
        return 0;
    }
    if(lseek(fd,1024,SEEK_SET)==-1){
        printf("set seek ext2_super_block error\n");
        return 0;
    }
    int num=read(fd,super_block,sizeof(struct ext2_super_block));
    close(fd);
    return num;
}

int set_super_block_data(struct ext2_super_block * super_block){
    int fd;
    if((fd = open(ext2_disk,O_RDWR))==-1){
        printf("set ext2_super_block error\n");
        return 0;
    }
    lseek (fd,1024,SEEK_SET);
    int num=write(fd,super_block,sizeof(struct ext2_super_block));
    close(fd);
    return num;
}

int set_bitmap(__u32 *bitmap,int block_num,int flag){
    int i=0;
    if(block_num >= 32*256 || block_num<0){
        printf("没有这样的块\n");
        return -1;
    }
    int num=block_num/32;
    int offset=block_num%32;
    if (flag=1){
        bitmap[num] |= (1<<(31-offset));
        return 1;
    }
    else {
        bitmap[num] &= ((0<<(31-offset)));
        return 1;
    }
}
int get_free_bitmap(__u32 *bitmap){
    int i,j;
    for (i=0;i<256;i++){
        for (j=0;j<=31;j++){
            if((!(bitmap[i]<<j))){
               // printf("bitmap[%d] : %X  %d \n",i,bitmap[i],i*32+j);
                return i*32+j;
            }
        }
    }
    return -1;
}

int get_inode_bitmap(char *bitmap){
    int fd=open(ext2_disk,O_RDONLY);
    if(fd==-1){
        printf("open ext2_disk error\n");
        return -1;
    }
    if(lseek(fd,1024+1024+1024,SEEK_SET)==-1){
	    printf("lseek inode error\n");
    	return -1;
    }
    if(read(fd,bitmap,1024)==-1) {
        printf("read ext2_disk error\n");
        return -1;
    }
    close(fd);
    return 1;
}

int set_inode_bitmap(char *bitmap){
    int fd;
    if((fd=open(ext2_disk,O_RDWR))==-1){
        printf("open ext2_disk error\n");
        return -1;
    }

    lseek(fd,1024*3,SEEK_SET);
    int num=write(fd,bitmap,1024);
    close(fd);
    return 1;
}

int get_block_bitmap(char *bitmap){
    int fd;
    if((fd = open(ext2_disk,O_RDONLY))==-1){
        printf("open ext2_disk error\n");
        return -1;
    }
    if (lseek(fd,1024*4,SEEK_SET)==-1){
	printf("lseek error \n");
	return -1;
    }
    int num;
    if((num=read(fd,bitmap,1024))==-1) {
        printf("read ext2_disk error\n");
        return -1;
    }
    close(fd);
    return num;
}
int set_block_bitmap(char *bitmap){
    int fd;
    if((fd=open(ext2_disk,O_RDWR))==-1){
        printf("open ext2_disk error\n");
        return -1;
    }

    lseek(fd,1024*4,SEEK_SET);
    int num=write(fd,bitmap,1024);
    close(fd);
    return num;
}
int free_block(int block_num){
    if(get_block_bitmap((char *)blockmap)==-1) {
        printf("get_block_bitmap error\n");
        return -1;
    }
    if(set_bitmap(blockmap,block_num,0)==-1){
        printf("set 0 bitmap error\n");
        return -1;
    }
    if(set_block_bitmap((char *)blockmap)==-1){
        printf("set_block_bitmap error\n");
        return -1;
    }
    get_super_block_data(&super_block);
    //printf("super_block free block : %d\n",super_block.s_free_blocks_count);
    super_block.s_free_blocks_count++;
    set_super_block_data(&super_block);
    return 1;
}

int new_block(){
    if(get_block_bitmap((char *)blockmap)==-1) {
        printf("get_block_bitmap error\n");
        return -1;
    }
    int block_num;
    if((block_num=get_free_bitmap((__u32 *)blockmap))==-1){
        printf("get_free_bitmap error\n");
        return -1;
    }
    printf("block_num :%d\n",block_num);
    if(set_bitmap(blockmap,block_num,1)==-1){
        printf("set 1 bitmap error\n");
        return -1;
    }
    if(set_block_bitmap((char *)blockmap)==-1){
        printf("set_block_bitmap error\n");
        return -1;
    }
    get_super_block_data(&super_block);
    //printf("super_block free block : %d\n",super_block.s_free_blocks_count);
    super_block.s_free_blocks_count--;
    set_super_block_data(&super_block);
    return block_num;
    
}
int free_inode(int inode_num){
    if(get_inode_bitmap((char *)inodemap)==-1) {
        printf("get_inode_bitmap error\n");
        return -1;
    }
    if(set_bitmap(inodemap,inode_num,0)==-1){
        printf("set 0 inodemap error\n");
        return -1;
    }
    if(set_inode_bitmap((char *)inodemap)==-1){
        printf("set_inode_bitmap error\n");
        return -1;
    }
    get_super_block_data(&super_block);
    //printf("super_block free_inode ;%d \n ",super_block.s_free_inodes_count);
    super_block.s_free_inodes_count++;
    set_super_block_data(&super_block);
    return 1;
}

int new_inode(){
    if(get_inode_bitmap((char *)inodemap)==-1) {
        printf("get_inode_bitmap error\n");
        return -1;
    }
    int inode_num;
    if((inode_num=get_free_bitmap((__u32 *)inodemap))==-1){
        printf("get_free_bitmap error\n");
        return -1;
    }
    //printf("inode_num :%d \n",inode_num);
    if(set_bitmap(inodemap,inode_num,1)==-1){
        printf("set 1 bitmap error\n");
        return -1;
    }
    if(set_inode_bitmap((char *)inodemap)==-1){
        printf("set_inode_bitmap error\n");
        return -1;
    }
    get_super_block_data(&super_block);
    //printf("super_block free inode : %d\n",super_block.s_free_inodes_count);
    super_block.s_free_inodes_count--;
    set_super_block_data(&super_block);
    return inode_num;
}

/*
int main(){
    get_super_block_data(&super_block);
    printf("free_block :%d \n",super_block.s_free_blocks_count);
    printf("free_inode :%d \n",super_block.s_free_inodes_count);
    get_inode_bitmap((char *)inodemap);
    new_block();
    new_inode();
    int i,j;
    for (i=0;i<10;i++){
        for(j=0;j<=31;j++){
            if((inodemap[i]<<j)) 
            printf("已占用inode:%d\n",i*32+j);
        }
    }
    get_block_bitmap((char *)blockmap);
    for (i=0;i<10;i++){
        for (j=0;j<=31;j++){
            if (blockmap[i]<<j)
            printf("已占用 block：%d\n",i*32+j);
        }
    }
    return 0;

}*/
