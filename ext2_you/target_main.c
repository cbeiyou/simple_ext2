/*************************************************************************
	> File Name: target_main.c
	> Author: bei 
	> Mail: 
	> Created Time: 2016年07月07日 星期四 16时14分14秒
 ************************************************************************/

#include <stdio.h>
#include "/home/bei/ext2_you/include/file.h"
#include "/home/bei/ext2_you/include/tou.h"


int main(int argc,char *agrv){
    init();
    printf("1 create file \n 2 create dir \n 3 open file \n 4 write file \n 5 read file \n 6 close file \n 7 delete file \n 8 change dir \n 9 ls \n 10 quit \n");
    int num;
    char com[1000];
    int fd=-1;
    char *cur_dir = "root";
    while (1){
        printf("%s>>",cur_dir);
        scanf ("%d",&num);
        if (num==1){
            printf("输入文件名：");
            scanf ("%s",com);
            sys_create(com,strlen(com),0x0666);
        }
        else if (num==2){
            printf("输入文件夹名：");
            scanf ("%s",com);
            sys_mkdir(com,strlen(com),0x0666);
        }
        else if(num==3){
            printf("输入文件路径：");
            scanf ("%s",com);
            fd = sys_openfile(com);
        }
        else if (num==4){
            if (fd==-1){
                printf("文件尚未打开\n");
                continue;
            }
            printf("输入要写如入的数据：");
            scanf ("%s",com);
            sys_lseek(fd,0,0);
            sys_write(fd,com,strlen(com));
        }
        else if(num == 5){
            sys_lseek(fd,0,0);
            sys_read(fd,com,300);
            printf("%s\n",com);
        }
        else if (num==6){
            sys_close(fd);
        }
        else if(num==7){
            printf("输入删除的文件名：");
            scanf ("%s",com);
            delete_file(&pwd,com);
        }
        else if(num==8){
            scanf ("%s",com);
            change_dir(com);
	    strcpy(cur_dir,com);
        }
        else if(num==9){
            list_all(&pwd);
        }
        else if(num==10){
            return 0;
        }
	else {
	    printf("输入的操作数不符\n");
            
	}
    }
    return 0;
}
