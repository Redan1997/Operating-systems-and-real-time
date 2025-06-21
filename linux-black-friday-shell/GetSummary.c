/*GetSummary.c*/
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<string.h>
#include<unistd.h>


int main()
{
    char str[128], path[50]="/home/braude/Black_Friday/camp_partic.txt";
	char name[15];
    int fd,rbytes,i,count=0,id,j;
    if ((fd = open(path, O_RDONLY)) == -1)
	{
		perror("open 1");
		exit(1);
	}
    if ((rbytes = read(fd, str, 128)) == -1)
	{
		perror("read 1");
		exit(1);
	}
	for(i=0;i<strlen(str);i++)//count comp
    {
        if(str[i]=='\n')
            count++;
    }
    printf("%d companies take part in the Campaign\n",count);
	i=0;
	j=0;
	while(str[i]!='\0')
	{
		if(str[i]!='\n')
		{
			name[j]=str[i];
			i++;
			j++;
		}
		else
		{
		name[j]='\0';
			if (!(id = fork()))
			{
        		execl("GetOrderNum", "GetOrderNum", name, 0);//using other functions
				j=0;
			}
    		if (id == -1)
        		perror("fork");
			wait();
			i++;
			j=0;
		}
	}

 	return 0;
}	
