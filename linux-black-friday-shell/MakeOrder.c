/*MakeOrder.c*/
#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include<string.h>
#include<time.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
char* pointer, ordersFile[256]="", * itemName, input[256]
        , cmpName[128]="", printcmd[9] = "GetFlyer", * temp,
        amountofitem[128], totalprice[256], buff[256], * discoundString, fileName[128]="", str[1000], counttemp[10], ch[10],day[3],month[3],year[5],path[50]="/home/braude/Black_Friday/";
    double count=0, dis;
    int id, priceOfItem, amount, fd_open, rbytes, wbytes, to, flag = 0, flagNum = 0, flagtok = 0, breakflag = 0, i, j = 0;
    time_t s, val = 1;
    struct tm* current_time;
    if (argc != 3) {
        printf("Input Error\n");
        exit(1);
    }
	fd_open = open("/home/braude/Black_Friday/camp_partic.txt", O_RDONLY);//open file for reading
    if (fd_open == -1) { perror("Open file"); return 2; }//2 is read code
	while (rbytes = read(fd_open, &buff, 256)) {//read from file to buff
    	if (strstr(buff, argv[1]) != NULL) {
            flag = 1;
            break;
        }
    }
	close(fd_open);
    if (flag==1) {
  		if (!(id = fork()))//print sale using getFlyer
        	execl(printcmd, printcmd, argv[1], 0);
    	if (id == -1)
        	perror("fork");
		wait();
		strcpy(cmpName,path);
		strcat(cmpName,argv[1]);
		strcat(cmpName,"_Order/");		
        strcat(cmpName, argv[1]);
        strcat(cmpName, ".txt");
     	fd_open = open(cmpName, O_RDONLY);//open file for reading
        if (fd_open == -1) { perror("Open file"); return 2; }//2 is read code
        while (rbytes = read(fd_open, &buff, 256)) {//read from file to buff
            strcpy(str, buff);
            strtok(str, "\n");//compney name not needed
            discoundString = strtok(NULL, "\n");
            dis = atoi(discoundString);//get the discount
            dis = dis / 100;
			strcpy(fileName,path);
			strcat(fileName,argv[1]);
			strcat(fileName,"_Order/");		
            strcat(fileName, argv[2]);
            strcat(fileName, ".txt");
            to = open(fileName, O_RDWR | O_CREAT, 0446);//open file for reading
            if (to == -1) { perror("Open file"); return 7; }//2 is read code
            strcpy(ordersFile, argv[1]);
            strcat(ordersFile, " Order\n\n");
            printf("Insert your order (STOP to finish):\n");
            scanf("%[^\n]%*c", input);
            i = strlen(input) - 1;
            while (input[i] != ' ') {
                if (input[i] >= '0' && input[i] <= '9')
                {
                    counttemp[j] = input[i];
                    j++;
                    i--;
                }
           }
            counttemp[j] = '\0';
            j = 0;
            for (i = strlen(counttemp) - 1; i >= 0; i--)
            {
                ch[j] = counttemp[i];
                j++;
            }
            ch[j] = '\0';
            amount = atoi(ch);
        	input[strlen(input) - j-1] = '\0';
            itemName = strtok(NULL, "\n");
            pointer=itemName;
            while ((strcmp(input, "STOP"))) {//if input contan stop then the while stop
                while (!(strstr(itemName, input))) {
                    itemName = strtok(NULL, "\n");
                    if (itemName == NULL) {
                        strcpy(input, "STOP");
                        breakflag = 1;
                        break;
                    }
                }
                while (!flagNum&&!breakflag) {
                    if (flagtok == 0) {
                        pointer = strtok(itemName, " " ".");
                        flagtok = 1;
                    }
                    else pointer = strtok(NULL, " " ".");
                    if (strstr(pointer, "NIS")) {
                        flagNum = 1;
                    }
                }
                if (breakflag)
                    break;
                 else {
                    flagNum = 0;
                    flagtok = 0;
                    priceOfItem = atoi(pointer);
                    strcat(input, " - ");
					sprintf(amountofitem,"%d",amount);
                    strcat(ordersFile, input);
                    strcat(ordersFile, amountofitem);
                    strcat(ordersFile, "\n\n");
                    count += (priceOfItem * amount - priceOfItem * amount * dis);
                    scanf("%[^\n]%*c", input);
					if(!(strcmp(input,"STOP")))
                        break;
                    flagtok = 0;
                    i = strlen(input)-1;
                    j = 0;
                    while (input[i] != ' ') {
                        if (input[i]>= '0' && input[i] <= '9')
                        {
                            counttemp[j] = input[i];
                            j++;
                            i--;
                        }
                    }
					counttemp[j] = '\0';
                    j = 0;
                    for (i = strlen(counttemp)-1; i >= 0 ; i--)
                    {
                        ch[j] = counttemp[i];
                        j++;
                    }
 					ch[j] = '\0';
                    amount = atoi(ch);
                    input[strlen(input) - j - 1] = '\0';
                    itemName = strtok(str, "\n");
                    strcpy(str, buff);
                    strtok(str, "\n");//compney name not needed
                    strtok(NULL, "\n");//discount not nedded
                }
            }
        }
		strcat(ordersFile, "Total Price: ");
        sprintf(totalprice, "%f", count);
		i=0;
        while (totalprice[i] != '\0')
        {
            if (totalprice[i] == '.')
            {
                totalprice[i + 2] = '\0';
                break;
            }
            i++;
        }
		strcat(ordersFile, totalprice);
        strcat(ordersFile, " NIS\n\n");
        s = time(NULL);
        current_time = localtime(&s);
        sprintf(day,"%d",current_time->tm_mday);
        sprintf(month, "%d", current_time->tm_mon + 1);
        sprintf(year, "%d", current_time->tm_year + 1900);
        strcat(ordersFile, day);
		strcat(ordersFile, "/");
        strcat(ordersFile, month);
        strcat(ordersFile, "/");
        strcat(ordersFile, year);	
        wbytes = write(to, ordersFile, strlen(ordersFile));
		printf("Total Price: %sNIS (Confirm to approve/else Cancel)\n",totalprice );
		scanf("%s", str);
		if (strcmp(str, "Cancel") == 0) {
			unlink(fileName);
		}
		else
		{
			printf("Order created! //%s.txt Created in %s_Order Dir with Read Mode\n",argv[2],argv[1]);
		}close(to);
    }
	else
		printf("Company Not Found!\n");
	to = open(fileName, O_RDONLY);//open file for reading
        if (fd_open == -1) { perror("Open file"); return 2; }
    return 0;
}
