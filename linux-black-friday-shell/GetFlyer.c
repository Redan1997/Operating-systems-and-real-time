/*GetFlyer.c*/
#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<string.h>
#include<unistd.h>
 int main(int argc, char* argv[])
{
	char path[50]="/home/braude/Black_Friday/";
	char buff[256];
	int fd,rbytes;
	strcat(path,argv[1]);
	strcat(path,"_Order/");
	strcat(path,argv[1]);
	strcat(path,".txt");//build path
		if ((fd=open(path,O_RDONLY))==-1)
	{
		perror("Open from.");
		return -1;
	}
	if((rbytes=read(fd,buff,256))==-1)
	{
		perror("Read1");
		return -1;	
	}
	buff[rbytes]='\0';
	printf("%s\n",buff);//print file
	return 0;

}
