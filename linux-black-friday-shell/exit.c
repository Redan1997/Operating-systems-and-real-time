/*exit.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
	int id;
	if (!(id = fork()))
	{
		execlp("rm","rm","-rf","/home/braude/Black_Friday",0);//remove folder
	}
    if (id == -1)
       	perror("fork");
	wait();//wait to child to finsh
	printf("GoodBye...\n");
	
	
	return 0;
}

