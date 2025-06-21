/*GetOrderNum.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char* argv[]) {

int pid, status,fd,rbytes,num=0;
char temp[256]="/home/braude/Black_Friday/",temp2[2]="",buff[128];

strcat(temp,argv[1]);
strcat(temp, "_Order");
if ((fd = open("/home/braude/Black_Friday/camp_partic.txt", O_RDONLY)) == -1)
{
	perror("open 1");
	return(-1);
}
if ((rbytes = read(fd, buff, 128)) == -1)
{
	perror("read 1");
	return -1;
}
if(strstr(buff,argv[1])!=NULL){
	if ( (pid = fork()) == -1) {
		perror("fork() failed.");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {//executescript'
		execlp("./count.sh",temp,temp,NULL);
		perror("execl() failed.");
		exit(EXIT_FAILURE);
	}
	pid = wait(&status);	//dad waiting for the procces to return
	fd=open("result.txt",O_RDONLY, 0);
	rbytes =read(fd, temp2, 1);
	num=atoi(temp2);
	printf("%s ---- %d orders\n",argv[1],num-1);
	close(fd);

	  //remove list.txt to clear it
		if ( (pid = fork()) == -1) {
			perror("fork() failed.");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			execl("/bin/rm", "rm", "result.txt", NULL);
			perror("execl() failed.");
			exit(EXIT_FAILURE);
		}
	
		pid = wait(&status);
}
else
	printf("Company Not Found!\n");
return 0;
}
