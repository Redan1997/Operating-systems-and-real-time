/*GetNumComp.c*/
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<string.h>
#include<unistd.h>

int main()
{
    char str[128], path[50]="/home/braude/Black_Friday/camp_partic.txt";
    int fd,rbytes,i,cnt=0;
    if ((fd = open(path, O_RDONLY)) == -1)
	{
		perror("open 1");
		return(-1);
	}
	if ((rbytes = read(fd, str, 128)) == -1)
	{
		perror("read 1");
		return -1;
	}
    for(i=0;i<strlen(str);i++)
    {
        if(str[i]=='\n')//count comp
            cnt++;
    }
    printf("%d companies take part in the Campaign\n",cnt);
 	return 0;
}
