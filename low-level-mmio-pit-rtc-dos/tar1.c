#include <stdio.h>
#include <dos.h>
#include <string.h>

int Screenserver_flag, X = 0, Y = 0, savedX = 0, savedY = 0, xflag = 0;
char far* b800h;
char screenMatrix[25][40], screenATT[25][40];//2 matrix that save every bit in the screen ascii and att
/*function declaration*/
void interrupt(*int9save)(void);
void my_cursor_show();
void my_cursor_movement(int x, int y);
void press(unsigned int c, char a);
char keystate();
void presenter();

void interrupt myISR9(void)
{
    char key_state;
    int9save();
    key_state = keystate();
    if (key_state) {//check if shift key has been pressed
        press(42, 0);
    }
}

void main(void)
{
    char c;
    int9save = getvect(9);
    setvect(9, myISR9);
    printf("cursor_showing ---\n");

    my_cursor_show();

    printf("Terminating ...\n");
    setvect(9, int9save);
} /* main */
/*function that move the corusor on the screen*/
void my_cursor_movement(int x, int y) {
    int cal = x * 40 + y;

    asm{
        MOV    BX ,cal
        MOV    DX,3D4H  // POINT TO 3D4H - 3D5H PORT PAIR
        MOV    AL,14    //ADDRESS OF CURSOR REGISTER POS HIGH BYTE
        MOV    AH,BH    // GET DESIRED VALUE OF CURSOR POS HIGH BYTE
        OUT    DX,AX    // PORT(3D4h) = 14, PORT(3D5h) = VALUE OF BH

        MOV    AL,15    // ADDRESS OF CURSOR REGISTER POS LOW BYTE
        MOV    AH,BL    // GET DESIRED VALUE OF CURSOR POS LOW BYTE
        OUT    DX,AX    // PORT(3D4h) = 15, PORT(3D5h) = VALUE OF BL
    } // asm
}

/*function that track useable button*/
void press(unsigned int c, char a) {
    int cal;
    if ((c >= 1 && c <= 13)//scan code from 1/! to =/+ buttons on the key board
        || (c >= 16 && c <= 27)//scan code from q/Q to ]/} buttons on the key board
        || (c >= 30 && c <= 40)//scan code from a/A to '/" buttons on the key board
        || (c >= 42 && c <= 54)//scan code from Lshift to //? buttons on the key board
        || (c >= 59 && c <= 62)//scan code from F1 to F4 buttons on the key board
        || (c >= 71 && c <= 83))//scan code of arrows buttons on the key board
    switch (c)
    {
    case 1://case of Esc

        Screenserver_flag = 0;
        break;
    case 72://case of up
        X--;
        b800h = b800h - 80;
        if (X < 0) {
            X++;
            b800h = b800h + 80;
        }
        break;
    case 77://case of right
        Y++;
        b800h = b800h + 2;
        if (Y > 39) {
            Y--;
            b800h = b800h - 2;
        }
        break;
    case 75://case of left
        Y--;
        b800h = b800h - 2;
        if (Y < 0) {
            Y++;
            b800h = b800h + 2;
        }
        break;
    case 80://case of down
        X++;
        b800h = b800h + 80;
        if (X > 24) {
            X--;
            b800h = b800h - 80;
        }
        break;
    case 59://F1

        *(b800h + 1) = screenATT[X][Y] = (screenATT[X][Y] + 1) % 255;
        break;
    case 60://F2
        c = screenMatrix[X][Y] = (screenMatrix[X][Y] + 2) % 128;
        *b800h = c;
        break;
    case 61://F3
        asm{
      MOV             DX,3D4h  // POINT TO 3D4H - 3D5H PORT PAIR
      MOV             AH, 1
      MOV             AL,0Ah // CURSOR START ADDRESS -
      OUT             DX,AX   // PORT(3D4h) = 0Bh, PORT(3D5h) = 0Eh
        } // asm
        break;
    case 62://F4
        asm{
      MOV             DX,3D4h  // POINT TO 3D4H - 3D5H PORT PAIR
      MOV             AH, 14
      MOV             AL,0Ah // CURSOR START ADDRESS -
      OUT             DX,AX   // PORT(3D4h) = 0Bh, PORT(3D5h) = 0Eh
        } // asm
        break;
    case 45://X
        if (xflag && a=='x') {
            X = savedX;
            Y = savedY;
            cal = (X * 40 + Y) * 2;
            asm{
             MOV  AX, WORD PTR cal
             MOV  WORD PTR b800h, AX
             MOV  WORD PTR b800h[2], 0B800h
            }
            xflag = 0;
        }
        else
            goto con;
        break;
    case 42://shift
    case 54:
        if (X != 24 || Y != 39) {//do shift all on screen only dont do in the right down area        
            savedX = X;
            savedY = Y;
            X = 24;
            Y = 39;
            cal = 2 * (X * 40 + Y);
            xflag = 1;
            asm{
             MOV  BX, WORD PTR cal
             MOV  WORD PTR b800h,BX
             MOV  WORD PTR b800h[2], 0B800h
            }
        }
        break;
    default:
    con://print ascii keys
        *b800h = a;
        *(b800h + 1) = screenATT[X][Y];
        screenMatrix[X][Y] = a;
        X++;
        b800h = b800h + 80;
        if (X > 24) {
            X--;
            b800h = b800h - 80;
        }
        break;
    }
    my_cursor_movement(X, Y);//update the coruser
}
/*function the print to center of the screen the names of presenter*/
void presenter() {
    // char far* b800h;
    char presenter1[] = "Redan Ganim";
    int i,j;
    
    asm{
         MOV  WORD PTR b800h,2 * (12 * 40 + 8)
         MOV  WORD PTR b800h[2],0B800h
    } // asm
    for (i = 0; i < strlen(presenter1); i++)
    {
        screenMatrix[12][8+i] = presenter1[i];
        *b800h = presenter1[i];
        *(b800h + 1) = screenATT[X][Y];
        b800h = b800h + 2;
     //   printf("%c", screenMatrix[12 + i][8]);
    }
    asm{
         MOV  WORD PTR b800h,2 * (13 * 40 + 8)
         MOV  WORD PTR b800h[2],0B800h }
}

void my_cursor_show()
{
    int i, j, cal;
    char c, a;
    memset(screenMatrix, ' ', sizeof(screenMatrix));
    memset(screenATT, 0xE, sizeof(screenATT));
    asm{
      MOV              AH,0      // SELECT FUNCTION = 'SET MODE'
      MOV              AL,1      // 40 BY 25 COLOR IMAGE
      INT              10H       // ADAPTER INITIALIZED. PAGE 0 DISPLAYED
    } // asm

        asm{
         MOV  WORD PTR b800h,0
         MOV  WORD PTR b800h[2],0B800h
    } // asm

    for (i = 0; i < 1000; i++)
    {
        *b800h = ' ';
        *(b800h + 1) = screenATT[X][Y];             //color of the cursor of each pixel
        b800h = b800h + 2;
    } // for  

    my_cursor_movement(X, Y);//set cursor in middle
    presenter();


    asm{
      MOV             DX,3D4h  // POINT TO 3D4H - 3D5H PORT PAIR
      MOV             AH,14
      MOV             AL,0Ah // CURSOR START ADDRESS -
      OUT             DX,AX   // PORT(3D4h) = 0Bh, PORT(3D5h) = 0Eh
      MOV             AL,0Bh  // CURSOR END ADDRESS (0Bh)
      MOV             AH,15
      OUT             DX,AX    // PORT(3D4h) = 0Ah, PORT(3D5h) = 01h
    }
    cal = X * 40 + Y;
    asm{
     MOV  AX, WORD PTR cal
     MOV  WORD PTR b800h,AX
     MOV  WORD PTR b800h[2],0B800h
    } // asm
    Screenserver_flag = 1;
    while (Screenserver_flag == 1)
    {
        asm{
               MOV AH,0				// BIOS read char from buffer option 
               INT 16h					// BIOS read char from buffer        
               MOV  c,AH		// Transfer scan code to program     
               MOV  a,AL			// Transfer char to program          
        }
        c = (unsigned int)c;
        press(c, a);
        
        
    } // while
    asm{
      MOV              AH,0      // SELECT FUNCTION = 'SET MODE'
      MOV              AL,2      // 80 BY 25 COLOR IMAGE
      INT              10H       // ADAPTER INITIALIZED. PAGE 0 DISPLAYED
    } // asm

}  /* my_cursor_show */

char keystate() {
    char key_state;
    asm{
        PUSH AX
        MOV AH,2
        INT 16h
        AND AL,0x03
        MOV key_state,AL
        POP AX
    }
    return key_state;
}