/*Advance_Shell.c*/
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<string.h>
#include<unistd.h>


int main(int argc, char* argv[])
{
	int id ,fd, i = 0 , j = 0 , crn=0,inputNum=0,c;
	char str[128] , temp[128] , strTemp[128];
	char* arg[3];
	char path1[128] = "/bin/", path2[128] = "./";
    	if(mkdir("/home/braude/Black_Friday", S_IRWXU | S_IRWXG | S_IRWXO)==-1)//make black friday folder
		perror("Error creating a new folder.");
        if((fd=open("/home/braude/Black_Friday/camp_partic.txt", O_WRONLY | O_CREAT, 0666))==-1)
	{
		perror("open 1");
		return (-1);
	}
    	printf("// Black_Friday dir created, camp_partic.txt created\n");
	do {
        printf("AdvShell> ");
        inputNum = scanf("%[^\n]%*c", str);//scanf till get enter from keyboard 
        if(!inputNum)//check if return value of scanf is !=0
            while ((c = getchar()) != '\n' && c != -1) {}
        }
    while (inputNum==0);//the loop is to clean buffer
    while(strcmp(str,"exit") != 0)
	{
		while(str[i] != ' ' && str[i] != '\0')
		{
			temp[j] = str[i];
			i++;
			j++;
		}
		temp[j]='\0';
		strcpy(strTemp,temp);
		strcat(path1,temp);
		strcat(path2,temp);
        j=0;
		while(str[i] != '\0')
			{
			i++;
			while(str[i] != ' ' && str[i] != '\0')
			{
				temp[j] = str[i];
				i++;
				j++;
			}
			temp[j] = '\0';//define end of string
			
			if(j != 0)
			{
				arg[crn] = (char*)malloc(sizeof(char)*20);//allocation mem
				strcpy(arg[crn],temp);	
				crn++;
			}
			j=0;
				}
        switch(id = fork())//create a thread
		{
			case 0:
				if(crn == 0)
				{
					execl(path1,strTemp,0);
					execl(path2,strTemp,0);
				}
				else if(crn == 1)
				{
					execl(path1,strTemp,arg[0],0);
					execl(path2,strTemp,arg[0],0);
				}
                else if(crn == 2)
				{
					execl(path1,strTemp,arg[0],arg[1],0);
					execl(path2,strTemp,arg[0],arg[1],0);
				}
                else if(crn == 3)//will suuport only upto 3 parameters
				{
					execl(path1,strTemp,arg[0],arg[1],arg[2],0);
					execl(path2,strTemp,arg[0],arg[1],arg[2],0);
                    }
			case -1:
				printf("Not Supported\n");//fork error then exit
				exit(1);
			default:
				break;
				
		}
		wait();//wait to chiled to finsh his exe		
        do {
             printf("AdvShell> ");
            inputNum = scanf("%[^\n]%*c", str);//input till \n
            if(!inputNum)
            while ((c = getchar()) != '\n' && c != -1) {}
        }
        while (inputNum==0);
		i=0;
		j=0;
		//build paths
		path1[0]='/';
		path1[1]='b';
		path1[2]='i';
		path1[3]='n';
		path1[4]='/';
		path1[5]='\0';
		path2[0]='.';
		path2[1]='/';
		path2[2]='\0';
		crn=0;
	}	
    strcat(path1,str);
	strcat(path2,str);

	execl(path1,str,0);
	execl(path2,str,0);

    for(i=0;i<3;i++)//free allocation mem
	    free(arg[i]);
	return 0;
}
