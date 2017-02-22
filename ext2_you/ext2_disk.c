//对于磁盘的操作。

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "/home/bei/ext2_you/include/file.h"
#include "/home/bei/ext2_you/include/tou.h"

//从num块中读取数据存放到buffer中
int get_block_data(unsigned int  num,char *buffer){
    if(!buffer){
        printf("get block data error\n");
        return 0;
    }
    //确定文件当前位置:引导块 1kb，超级块1kb，组描述符1kb，inode位图1kb，块位图1kb，inode节点表1024kb，块位置
    unsigned int  file_cur = 1024 *5 + 1024 * 1024 + 1024 * num;
    int fd;
    if ((fd = open("/home/bei/ext2_you/ext2_disk",O_RDONLY )) == -1) {
        printf("get ext2_disk open error \n");
        return 0;
    }
    if (lseek(fd,file_cur,SEEK_SET)==-1){
        printf("get ext2_disk lseek error\n");
        return 0;
    }
    int count;
    if ((count = read(fd,buffer,1024))==0){
        printf("get ext2_disk read error\n");
        return 0;
    }
    close (fd);
    return count;
}


int set_block_data(unsigned int  num,char *buffer){
    if(!buffer){
        printf("set block data error\n");
        return 0;
    }
    unsigned int file_cur = 1024*5 + 1024*1024 + 1024*num;
    int fd;
    if ((fd = open("/home/bei/ext2_you/ext2_disk" , O_RDWR )) == -1) {
        printf("set ext2_disk open error \n");
        return 0;
    }
    if (lseek(fd,file_cur,SEEK_SET)==-1){
        printf("set ext2_disk lseek error\n");
        return 0;
    }
    int count;
    if ((count = write(fd,buffer,1024))==0){
        printf("set ext2_disk read error\n");
        return 0;
    }
    close (fd);
    return count;
}

int get_inode_data(unsigned int  num,void * inode){
    int fd;
    __u32 file_cur=1024*5 + 128 * num;
    if ((fd = open("/home/bei/ext2_you/ext2_disk" , O_RDONLY )) == -1) {
        printf("get ext2_disk inode  open error \n");
        return 0;
    }
    if (lseek(fd,file_cur,SEEK_SET)==-1){
        printf("get ext2_disk inode  lseek error\n");
        return 0;
    }
    int count ;
    if( (count = read(fd,inode,128))==0){
        printf("get ext2_disk inode read error\n");
	return 0;
    }
    ((struct inode *)inode)->i_number = num;
    close (fd);
    return count;
}

int set_inode_data(unsigned int  num,void *inode){
    int fd;
    __u32 file_cur=1024*5 + 128 * num;
    if ((fd = open("/home/bei/ext2_you/ext2_disk" , O_RDWR )) == -1) {
        printf("set ext2_disk inode  open error \n");
        return 0;
    }
    if (lseek(fd,file_cur,SEEK_SET)==-1){
        printf("set ext2_disk inode  lseek error\n");
        return 0;
    }
    int count ;
    count = write(fd,inode,128);
    close (fd);
    return count;
}
/*
int main(){	
    char *buffer  = "hello world";
    char df[1024];
    int num = set_block_data(10,buffer);
    printf("num = %d\n ",num);
    get_block_data(10,df);
    printf("df == %s \n",df);

    struct inode* m_inode;
//char buf[128];
//m_inode = (struct inode *)buf;

    m_inode->i_number = 10;
    num = set_inode_data(m_inode->i_number,(char *)m_inode);
	printf("num = %d\n",num);

	//get_inode_data(m_inode->i_number,m_inode);
    return 0;
}
*/
