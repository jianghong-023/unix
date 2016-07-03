/*
 * Linux_C实现将文件导出至U盘 
 * 只支持文件
 * 时间： 2011－11－8 
 */

#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
 {
	 int fdr,fdw;	 /* 文件描述符 fdr->源文件 fdw->目标文件 */
	 char buffer[BUFFER_SIZE];	/* 定义缓冲区 */
	 int cntr, cntw;
	 char *ptr; 
	 char FileName[20];
	 	 
	 /* 获得要复制文件名 */
	 if(argc < 2)
	 {
		 printf("输入要复制文件名: ");
		 exit(1);
	 }
	 	 
	 /*只读方式打开源文件*/
	 fdr = open(argv[1], O_RDONLY);		
	 if(fdr < 0)
	 {
		 perror("Open fdr failed");
		 exit(-1);
	 }
	 
	  /* 扗载U盘至/mnt目录 */
	 if(system("mount /dev/sda1 /mnt") < 0)
	 {
		 printf("mount USB failed\n");
		 exit(0);
	 }
	
	 sprintf(FileName,"/mnt/%s",argv[1]);
	 /*以写的方式打开文件，如果文件不存在则创建，其权限 可读写*/
	 fdw = open(FileName, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
	 if(fdw < 0)
	 {
		 perror("Open fdw failed");
		 exit(-1);
	 }
	 
	 /* 文件拷贝 */
	 while((cntr = read(fdr, buffer, BUFFER_SIZE)) != 0) /* 读文件 */
	 {
		 if(cntr < 0) /*文件读取是否出错*/
		 {
			 perror("read error"); 
			 break;
		 }
		 else if(cntr > 0) /* 读取文件成功，数据长度为cntr*/
		 {
			 ptr = buffer;
			 while((cntw = write(fdw, ptr, cntr)) != 0) /* 将读取的长度写入到目标文件中 */
			 {
				if(cntw < 0) /*文件读取是否出错*/
				 {
					 perror("write error"); 
					 break;
				 }
				else if(cntr == cntw)  /* 写入字节和读取字节相同，数据全部写入 */
					break;
				else if(cntw > 0) 	/* 写入字节小于读取字节，只有部分数据写入 */
				{
					ptr += cntw; 	/* 指向未写入的数据，接下来再次执行写入操作 */
					cntr -= cntw;					
				}
			 }
			 if(cntw < 0) /*文件读取是否出错*/
			 {
				 perror("write error"); 
				 break;
			 }
		 }		 
	 }
			 
	 /* 关闭文件 */
	close(fdr);
	close(fdw);
	
	 /* 制载U盘至/mnt目录 */
	 if(system("umount /mnt") < 0)
	 {
		 printf("umount USB failed\n");
		 exit(0);
	 }
	
	printf(" Done!\n");
	 
	return 0;
 }