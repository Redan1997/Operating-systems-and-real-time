/* game1.c - main, prntr */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <bios.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>

#define ARROW_NUMBER 30
#define TARGET_NUMBER 5
#define ARRSIZE 1000
#define MAX_SCREEN_X  80  // Assuming your screen width is 80 characters
#define TARGET_WIDTH   1  // Assuming your target representation is 3 characters wide
#define POINTS_PER_TARGET 1
int score = 0, value;
char score_text[2 ^ 5];
int far* b800h;
void resetGame();               //set status for next level
void gameOver();                //function of end game
int gameOverFlag = 0,nextGameFlag=0;           //flag for to end game
int target_movement_delay = 15;  // Adjust this value to control the speed of the targets
typedef struct position
{
    int x;
    int y;

}  POSITION;
char display_draft[25][80];
POSITION target_pos[TARGET_NUMBER];
POSITION arrow_pos[ARROW_NUMBER];
extern SYSCALL sleept(int);
extern struct intmap far* sys_imp;

void interrupt(*int8save)(void);

char entered_ascii_codes[ARRSIZE];
int tail = -1;
char display[2001], screenchar;

char ch_arr[ARRSIZE];
int front = -1;
int rear = -1;

int point_in_cycle;
int gcycle_length;
int gno_of_pids;

int initial_run = 1;
int targetsHit = 0;
int level = 1;
int gun_position;
int no_of_arrows;
int target_disp = 80 / TARGET_NUMBER;
char ch;

int no_of_targets;

int sched_arr_pid[5] = { -1 };
int sched_arr_int[5] = { -1 };
int receiver_pid;
int shdpid;


INTPROC new_int9(int mdevno)
{
    char result = 0;
    int scan = 0;
    int ascii = 0;

    asm{//port60h
    IN AL,60h
    MOV BYTE PTR scan,AL
    PUSH AX
    IN AL,61h
    OR AL,80h
    OUT 61h,Al
    AND Al,7Fh
    OUT 61h,Al
    POP AX
    } asm
        ascii = 0;
    if (scan == 75)
        result = 'a';
    else
        if (scan == 72)
            result = 'w';
        else
            if (scan == 77)
                result = 'd';
    if (scan == 1)
        asm INT 27; // terminate xinu

    if ((result != 0) && (tail < ARRSIZE))
    {
        entered_ascii_codes[++tail] = result;
    } // if

    send(receiver_pid, result);

Skip1:
} // new_int9

void set_new_int9_newisr()
{
    int i;
    for (i = 0; i < 32; i++)
        if (sys_imp[i].ivec == 9)
        {
            sys_imp[i].newisr = new_int9;
            return;
        }

} // set_new_int9_newisr

void interrupt newint8(void)
{
    // sleep(1);
     /*asm{
          MOV AL,20h
          OUT 20h,AL
     }*/
     // int8save();
} // newint8(void)

void displayer(void)
{
    int i;
    while (1)
    {
        receive();
        if(!gameOverFlag && !nextGameFlag)
         for (i = 0; i < 2000; i++)
             b800h[i] = display[i] + 256 * 126;
    } //while
} // prntr

void receiver()
{
    char temp;
    // CHANGE

    while (1)
    {
        receive();
        while (tail > -1)
        {
            temp = entered_ascii_codes[tail];
            rear++;
            tail--;
            if (rear < ARRSIZE)
                ch_arr[rear] = temp;
            if (front == -1)
                front = 0;
        } // while
    } // while
    // END OF CHANGE
} //  receiver


void updater()
{

    int i, j;
    int score_text_len;
    int target_movement_counter = 0;
    char MSG[20];

    if (initial_run == 1)
    {
        initial_run = 0;
        no_of_arrows = 0;
        no_of_targets = TARGET_NUMBER;
        gun_position = 39;
        target_pos[0].x = 3;
        target_pos[0].y = 1;
        for (i = 1; i < TARGET_NUMBER; i++)
        {
            target_pos[i].x = i * target_disp;
            target_pos[i].y = 1;

        } // for
        for (i = 0; i < ARROW_NUMBER; i++)
            arrow_pos[i].x = arrow_pos[i].y = -1;
    } // if (initial_run ==1)

 // CHANGE 
    while (1)
    {
        receive();


        while (front != -1)
        {
            ch = ch_arr[front];
            if (front != rear)
                front++;
            else
                front = rear = -1;

            if ((ch == 'a') || (ch == 'A'))
                if (gun_position >= 2)
                    gun_position--;
                else;
            else if ((ch == 'd') || (ch == 'D'))
                if (gun_position <= 78)
                    gun_position++;
                else;
            else if ((ch == 'w') || (ch == 'W'))
                if (no_of_arrows < ARROW_NUMBER)
                {
                    arrow_pos[no_of_arrows].x = gun_position;
                    arrow_pos[no_of_arrows].y = 23;
                    no_of_arrows++;

                } // if
        } // while(front != -1)

        ch = 0;
        for (i = 0; i < 25; i++)
            for (j = 0; j < 80; j++)
                display_draft[i][j] = ' ';  // blank

        display_draft[22][gun_position] = '^';
        display_draft[23][gun_position - 1] = '/';
        display_draft[23][gun_position] = '|';
        display_draft[23][gun_position + 1] = '\\';
        display_draft[24][gun_position] = '|';

        for (i = 0; i < ARROW_NUMBER; i++)
        {
            if (arrow_pos[i].x != -1)
            {
                if (arrow_pos[i].y > 0)
                {
                    arrow_pos[i].y--;
                    display_draft[arrow_pos[i].y][arrow_pos[i].x] = '^';
                    display_draft[arrow_pos[i].y + 1][arrow_pos[i].x] = '|';
                }
                else
                {
                    // If the arrow has reached the top, don't update its position
                    // and skip displaying it
                    arrow_pos[i].x = -1;
                }
            }
        }


        // Bullet-Target Collision Detection
        for (i = 0; i < ARROW_NUMBER; i++)
        {
            for (j = 0; arrow_pos[i].x != -1 && j < TARGET_NUMBER; j++)
            {
                if (target_pos[j].x != -1 &&
                    arrow_pos[i].x >= target_pos[j].x &&
                    arrow_pos[i].x <= target_pos[j].x &&
                    arrow_pos[i].y >= target_pos[j].y &&          // Check if arrow y is equal or below the target y
                    arrow_pos[i].y <= target_pos[j].y + 1)        // Increase the collision check area by 1 pixel downward
                {
                    // Collision detected
                    arrow_pos[i].x = -1;  // Remove the arrow
                    target_pos[j].x = -1; // Remove the target
                    score++;
                    targetsHit++;
                }
            }
        }

        // Display score
        score_text_len = sprintf(score_text, "Level: %d Score:%d Arrow:%d",level, score,ARROW_NUMBER-no_of_arrows);  // Get string length
        if (score_text_len > 0) {  // Check if string was formatted successfully
            for (i = 0; i < score_text_len && i < 70; i++) {  // Limit display length
                display_draft[0][i] = score_text[i];  // Assuming top-left corner
            }
        }
        //moving target with deley
        target_movement_counter++;
        if (target_movement_counter >= target_movement_delay) {
            target_movement_counter = 0; // Reset the counter
            for (i = 0; i < TARGET_NUMBER; i++) {
                if (target_pos[i].x != -1) {
                    if (target_pos[i].y < 22) {
                        target_pos[i].y++;
                    }
                    display_draft[target_pos[i].y][target_pos[i].x] = '*';
                }
            }
        }
        else {
            for (i = 0; i < TARGET_NUMBER; i++)
                if (target_pos[i].x != -1)
                    display_draft[target_pos[i].y][target_pos[i].x] = '*';
        }

        for (i = 0; i < 25; i++)
            for (j = 0; j < 80; j++)
                display[i * 80 + j] = display_draft[i][j];
        display[2000] = '\0';

        // Check if all targets have been hit
        if (targetsHit == TARGET_NUMBER)
        {
            nextGameFlag = 1;
            sprintf(MSG, "Level %d completed!", level);
            level++;
            b800h = NULL;
            ((unsigned long int)b800h) = 0xB800 * 65536;
            value = ' ' + 256 * 126;//clean screen for msg
            for (i = 0; i < 990; i++)
                b800h[i] = value;
            for (i, j = 0; j < 19; i++, j++) {
                value = MSG[j] + 256 * 126;
                b800h[i] = value;
            }
            value = ' ' + 256 * 126;
            for (i; i < 2000; i++)
                b800h[i] = value;
            sleept(250);
            nextGameFlag = 0;
            // update for the next level
            resetGame();
            // Reset targets_hit for the next level
            targetsHit = 0;
        }
        //end of game if target reach end
        for (i = 0; i < TARGET_NUMBER; i++)
        {
            if (target_pos[i].x != -1 && target_pos[i].y == 22)
                gameOver();
        }
    } // while

    //  END OF CHANGE
} // updater 


SYSCALL schedule(int no_of_pids, int cycle_length, int pid1, ...)
{
    int i;
    int ps;
    int* iptr;

    disable(ps);

    gcycle_length = cycle_length;
    point_in_cycle = 0;
    gno_of_pids = no_of_pids;

    iptr = &pid1;
    for (i = 0; i < no_of_pids; i++)
    {
        sched_arr_pid[i] = *iptr;
        iptr++;
        sched_arr_int[i] = *iptr;
        iptr++;
    } // for
    restore(ps);

} // schedule 

void scheduler()
{
    int interval_ticks, pid_index;

    point_in_cycle++;
    if (point_in_cycle == gcycle_length)
        point_in_cycle = 0;

    interval_ticks = gcycle_length / gno_of_pids;
    pid_index = 0;

    while (1)
    {
        sleept(interval_ticks);

        send(sched_arr_pid[pid_index], 11);
        pid_index++;
        if (pid_index == gno_of_pids)
            pid_index = 0;
    } // while

} // scheduler

void startup() {
    union REGS regs;
    int i;
    // struct REGPACK regpack;

    regs.h.ah = 3;
    regs.h.al = 1;
    int86(0x10, &regs, &regs);
    b800h = NULL;
    ((unsigned long int)b800h) = 0xB800 * 65536;
    value = ' ' + 256 * 126;
    for (i = 0; i < 2000; i++)
        b800h[i] = value;
}

xmain()
{
    int uppid, dispid, recvpid;
    set_new_int9_newisr();
    outportb(0x43, 0x34);
    outportb(0x40, 0x9C);
    outportb(0x40, 0x2E);
    startup();
    resume(dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0));
    resume(recvpid = create(receiver, INITSTK, INITPRIO + 3, "RECIVEVER", 0));
    resume(uppid = create(updater, INITSTK + 5, INITPRIO, "UPDATER", 0));
    receiver_pid = recvpid;

    schedule(2, 10, uppid, 0, dispid, 6);

    resume(shdpid = create(scheduler, INITSTK, INITPRIO + 1, "DISPLAYER", 0));

    return 0;
} // main


void resetGame() {
    int i;
    no_of_arrows = 0;

    target_pos[0].x = 3;
    target_pos[0].y = 1;

    for (i = 1; i < TARGET_NUMBER; i++)
    {
        target_pos[i].x = i * target_disp;
        target_pos[i].y = 1;
    }
    gun_position = 39;
    arrow_pos[0].x = -1;
    // Update the number of targets for the new level
    no_of_targets = TARGET_NUMBER;
    targetsHit = 0;
    target_movement_delay = 20 - level;
}

void gameOver() {
    char MSG1[] = "!GAME OVER!";
    int i, j;
    gameOverFlag++;
    b800h = NULL;
    ((unsigned long int)b800h) = 0xB800 * 65536;
    value = ' ' + 256 * 126;
    for (i=0; i < 994; i++)
        b800h[i] = value;
    for (i,j=0; j< 11; i++,j++) {
        value = MSG1[j] + 256 * 126;
        b800h[i] = value;
    }
        value = ' ' + 256 * 126;
    for (i; i < 2000; i++)
        b800h[i] = value;
    sleept(200);
    asm INT 27;
}