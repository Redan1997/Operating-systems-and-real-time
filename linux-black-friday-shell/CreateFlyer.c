/*CreateFlyer.c*/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{	
	int fd,fd2,wbytes,rbytes,i,l,k,x,j=0;
	char str[128]="",name[50]="",tempname[20],strTemp[30]="",temp[15]="",price[15]="",path[50]="/home/braude/Black_Friday/";
	strcat(path, argv[1]);
	strcat(path, "_Order");
	if(mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO)==-1)
		perror("Error creating a new folder.");
	strcat(path, "/");
	strcat(path,argv[1]);
	strcat(path,".txt");
	if((fd=open(path, O_WRONLY | O_CREAT, 0446))==-1)
	{
		perror("open 1");
		return (-1);
	}
	strcpy(tempname,"\t\t");
	strcat(tempname,argv[1]);
	strcat(tempname," SALE\n");
	if((wbytes = write(fd, tempname, strlen(tempname)) ) ==-1)
	{
		perror("Write 1");
		return (-1);
	}
	strcpy(tempname,"\t\t");
	strcat(tempname,argv[2]);
	strcat(tempname,"% off\n");
	if((wbytes = write(fd, tempname, strlen(tempname)) ) ==-1)
	{
		perror("Write 2");
		return (-1);
	}
	printf("Insert item and item cost:\n");
	scanf("%[^\n]%*c", str);
	while(strcmp(str,"Stop")!=0)
	{
		i=0;
		k=0;
		l=0;
		j=0;
		memset(price,0,15);
		memset(temp,0,15);
		for (i = strlen(str); i > 0; i--)
		{
			if (str[i] >= '0' && str[i] <= '9')
			{
				temp[k] = str[i];
				k++;
			}
			if (str[i] == ' ')
				break;
		}
		for (i = strlen(temp); i >= 0; i--)
		{
			if (temp[i] >= '0' && temp[i] <= '9')
			{
				price[j] = temp[i];
				j++;
			}
		}
		l = strlen(str) - strlen(price);
		str[l] = '\0';;
		if(strcmp(price,"")!=0)
			strcat(price,"NIS\n");
		for(i=strlen(str); i<30; i++)
			strcat(str, ".");
		if((wbytes = write(fd, str, strlen(str)) ) ==-1)
		{
			perror("Write 3");
			return (-1);
		}
		if((wbytes = write(fd, price, strlen(price)) ) ==-1)
		{
			perror("Write 4");
			return (-1);
		}
		memset(str,0,128);
		scanf("%[^\n]%*c", str);
	
	}
	printf("What is your choice?\n");
	printf("1.Enjoyed\n2.Shop Now and have fun\n3.Have Fun\n4.strTemp\n");
	scanf("%d",&x);
	if(x==1)
	{
		if((wbytes = write(fd, "\t\tEnjoyed", 24) ) ==-1)
		{
			perror("Write 5");
			return (-1);
		}	
	}
	else if(x==2)
	{
		if((wbytes = write(fd, "\t\tShop Now and have fun",33 ) ) ==-1)
		{
			perror("Write 6");
			return (-1);
		}	
	}
	else if(x==3)
	{
		if((wbytes = write(fd, "\t\tHave Fun",25 ) ) ==-1)
		{
			perror("Write 7");
			return (-1);
		}	
	}
	else
	{
		printf("Insert your word:\n");
		strcpy(name,"");
		getchar();
		scanf("%[^\n]%*c", name);
		strcpy(strTemp,"\t\t");
		strcat(strTemp,name);
		if((wbytes = write(fd, strTemp, strlen(strTemp)) ) ==-1)
		{
			perror("Write 8");
			return (-1);
		}
	}
	printf("Successfully created // %s.txt created , %s_Order dir created\n",argv[1],argv[1]);
	if((fd2=open("/home/braude/Black_Friday/camp_partic.txt", O_RDONLY, 0446))==-1)
	{
		perror("open 2");
		return (-1);
	}
	memset(str,0,128);
	rbytes = read(fd2, &str, 128);
	strcat(str,argv[1]);
	strcat(str,"\n");
	close(fd2);
	if((fd2=open("/home/braude/Black_Friday/camp_partic.txt", O_WRONLY, 0446))==-1)
	{
		perror("open 2");
		return (-1);
	}
	if((wbytes = write(fd2, str, strlen(str)) ) ==-1)
	{
		perror("Write 9");
		return (-1);
	}
	
	close(fd);
	close(fd2);
	return 0;
	
}

