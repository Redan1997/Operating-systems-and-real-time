/* game1.c redan- main, prntr */
#include <dos.h>
#include <stdio.h>


#define ARROW_NUMBER 30
#define TARGET_NUMBER 5
#define ARRSIZE 1000

void interrupt (*old_int9)(void);
void interrupt(*old_int8)(void);

char entered_ascii_codes[ARRSIZE];
int tail = -1;
char display[2001];

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
int target_disp = 80/TARGET_NUMBER;
char ch;
int no_of_targets;
//new 
int score = 0, value;
char score_text[100];
int far* b800h;
void resetGame();               //set status for next level
void gameOver();                //function of end game
int gameOverFlag = 0, nextGameFlag = 0;           //flag for to end game
int target_movement_delay = 40;  // Adjust this value to control the speed of the targets
int target_movement_counter = 0;
void my_halt()
{
   setvect(9, old_int9);
   setvect(8, old_int8);
   asm {CLI}
   exit();
   
} // my_halt()

void interrupt new_int9(void)
{
 int scan = 0;
 int ascii = 0;

 (*old_int9)();
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
   ascii = 'a';
 else
   if (scan == 72)
     ascii = 'w';
   else
   if (scan == 77)
      ascii = 'd';
// if ((scan == 46)&& (ascii == 3)) // Ctrl-C?
   if (scan == 1) // Esc?
     my_halt(); // terminate program

  if ((ascii != 0) && (tail < ARRSIZE))
   {
      entered_ascii_codes[++tail] = ascii;
   } // if

Skip1:
  return;
} // new_int9

// Timer interrupt handler
void interrupt new_int8(void)
{
    old_int8(); // Call old interrupt handler
}

typedef struct position
{
  int x;
  int y;

}  POSITION;



void displayer( void )
{
    int i;
    if (!gameOverFlag && !nextGameFlag)
        for (i = 0; i < 2000; i++)
            b800h[i] = display[i] + 256 * 126;
} // prntr

void receiver()
{
    char temp;
    int i;

    i = 0;
    while (i <= tail)
    {
      temp = entered_ascii_codes[i];
      rear++;
      i++;

      if (rear < ARRSIZE)
          ch_arr[rear] = temp;
      if (front == -1)
         front = 0;
   } // while
   tail = 0;

} //  receiver


char display_draft[25][80];
POSITION target_pos[TARGET_NUMBER];
POSITION arrow_pos[ARROW_NUMBER];


void updater()
{

    int i, j;
    int score_text_len;
    char MSG[20];
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
    score_text_len = sprintf(score_text, "Level: %d Score:%d Arrow:%d", level, score, ARROW_NUMBER - no_of_arrows);  // Get string length
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
        for (i=990, j = 0; j < 19; i++, j++) {
            value = MSG[j] + 256 * 126;
            b800h[i] = value;
        }
        value = ' ' + 256 * 126;
        for (; i < 2000; i++)
            b800h[i] = value;
        sleep(3);
        target_movement_counter = 0;
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
} // updater 


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
}

main()
{
    int uppid, dispid, recvpid;


    old_int9 = getvect(9);
    old_int8 = getvect(8);
    setvect(9, new_int9);
    setvect(8, new_int8);
    outportb(0x43, 0x34);
    outportb(0x40, 0x9C);
    outportb(0x40, 0x2E);
    startup();
    while (1)
    {
        receiver();
        updater();
        displayer();
    } // while
} // main'


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
    target_movement_delay = 40 - level;
}

void gameOver() {
    char MSG1[] = "!GAME OVER!";
    int i, j;
    gameOverFlag++;
    b800h = NULL;
    ((unsigned long int)b800h) = 0xB800 * 65536;
    value = ' ' + 256 * 126;
    for (i = 0; i < 994; i++)
        b800h[i] = value;
    for (i=994, j = 0; j < 11; i++, j++) {
        value = MSG1[j] + 256 * 126;
        b800h[i] = value;
    }
    value = ' ' + 256 * 126;
    for (; i < 2000; i++)
        b800h[i] = value;
    sleep(5);
    my_halt();
}