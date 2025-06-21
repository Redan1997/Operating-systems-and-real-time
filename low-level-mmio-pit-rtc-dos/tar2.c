#include <dos.h>
#include <stdio.h>

#define ON (1)
#define OFF (0)
volatile int count;
volatile int change = 0;
volatile int KeyInput = 1;
int latch = 200;
int SoundTime = 3;

void interrupt(*int8save)(void);
void interrupt(*int9save)(void);

void interrupt myISR9(void)
{
    char c;
    volatile int pressed;//fag

    asm{
        PUSH AX
        MOV AH,1
        INT 16h
        PUSHF
        POP AX
        AND AX,64  //zero flag
        MOV pressed,AX
        POP AX
    } /* checking if button was presed*/

        if (pressed == 0) {
            asm{
                PUSH AX
                MOV AH,0
                INT 16h
                MOV c,AH
                POP AX
            }/* reading the butten*/
            switch (c)
            {
            case 1://case of Esc

                KeyInput = 0;
                change = 1;
                break;
            case 2:
                latch = 100;
                count = 0;
                change = 1;
                break;

            case 3:
                latch = 200;
                count = 0;
                change = 1;
                break;

            case 4:
            case 81:
                latch = 300;
                count = 0;
                change = 1;
                break;

            case 5:
                latch = 400;
                count = 0;
                change = 1;
                break;

            case 6:
                latch = 500;
                count = 0;
                change = 1;
                break;

            case 7:
                latch = 600;
                count = 0;
                change = 1;
                break;

            case 8:
                latch = 700;
                count = 0;
                change = 1;
                break;

            case 9:
                latch = 800;
                count = 0;
                change = 1;
                break;

            case 10:
                latch = 900;
                count = 0;
                change = 1;
                break;
                //up
            case 72:
                SoundTime++;
                count = 0;
                change = 1;
                break;
                //down
            case 80:
                SoundTime--;
                count = 0;
                change = 1;
                if (SoundTime < 0)
                    SoundTime = 0;
                break;

            default:
                break;
            }
        }
    int9save();
}
/*function that help convert from dismal to binary used for watch*/
int  convert_to_binary(int x)
{
    int i;
    int temp, scale, result;


    temp = 0;
    scale = 1;
    for (i = 0; i < 4; i++)
    {
        temp = temp + (x % 2) * scale;
        scale *= 2;
        x = x >> 1;
    } // for

    result = temp;
    temp = 0;

    scale = 1;
    for (i = 0; i < 4; i++)
    {
        temp = temp + (x % 2) * scale;
        scale *= 2;
        x = x >> 1;
    } // for

    temp *= 10;
    result = temp + result;
    return result;

} // convert_to_binary

void readclk()
{
    int i;
    int hour, min, sec;


    hour = min = sec = 0;

    asm{
     PUSH AX
     MOV AL,0
     OUT 70h,AL
     IN AL,71h
     MOV BYTE PTR sec,AL
  ;
     MOV AL,2
     OUT 70h,AL
     IN AL,71h
     MOV BYTE PTR min,AL
  ;
     MOV AL,4
     OUT 70h,AL
     IN AL,71h
     MOV BYTE PTR hour,AL
  ;
     POP AX
    } // asm

    sec = convert_to_binary(sec);
    min = convert_to_binary(min);
    hour = convert_to_binary(hour);

    printf("%2d:%2d:%2d", hour, min, sec);

} // readclk

void interrupt newint8(void)
{
    count++;
    asm{
         MOV AL,20h
         OUT 20h,AL
    }

} // newint8(void)

void mydelay(int n)//delay the program n seconds
{
    asm{
      CLI
      PUSH AX
      MOV AL,036h
      OUT 43h,AL
      MOV AX,11932
      OUT 40h,AL
      MOV AL,AH
      OUT 40h,AL
      POP AX
    } // asm

    int8save = getvect(8);
    setvect(8, newint8);
    asm{ STI };
    count = 0;
    while (count <= n * 100 && !change)
        ;

    asm{
      CLI
      PUSH AX
      MOV AL,036h
      OUT 43h,AL
      MOV AX,0
      OUT 40h,AL
      MOV AL,AH
      OUT 40h,AL
      POP AX
    } // asm


    setvect(8, int8save);//return the inturpt of the clk


} //mydelay

void ChangeSpeaker(int status)
{
    int portval;
    //   portval = inportb( 0x61 );

    portval = 0;
    asm{
         PUSH AX
         IN AL,61h
         MOV byte ptr portval,AL
         POP AX
    }

        if (status == ON)
            portval |= 0x03;
        else
            portval &= ~0x03;
    // outportb( 0x61, portval );
    asm{
      PUSH AX
      MOV AX,portval
      OUT 61h,AL
      POP AX
    } // asm

} /*--ChangeSpeaker( )----------*/

void Sound(int hertz)
{
    unsigned divisor = 1193180L / hertz;

    ChangeSpeaker(ON);

    //        outportb( 0x43, 0xB6 );
    asm{
      PUSH AX
      MOV AL,0B6h
      OUT 43h,AL
      POP AX
    } // asm


 //       outportb( 0x42, divisor & 0xFF ) ;
        asm{
          PUSH AX
          MOV AX,divisor
          AND AX,0FFh
          OUT 42h,AL
          POP AX
    } // asm


 //        outportb( 0x42, divisor >> 8 ) ;

        asm{
          PUSH AX
          MOV AX,divisor
          MOV AL,AH
          OUT 42h,AL
          POP AX
    } // asm

} /*--Sound( )-----*/

void NoSound(void)
{
    ChangeSpeaker(OFF);
} /*--NoSound( )------*/

int main() {
    int9save = getvect(9);
    setvect(9, myISR9);
    do
    {
        change = 0;//TRACKING WHEN THERE IS CHANGE WHILE PRESSING
        if (SoundTime > 0) {
            Sound(latch);
            mydelay(SoundTime);//SOUND time running
        }
        NoSound();
        mydelay(2);//sleep 2 seconds
    } while (KeyInput);//TILL PRESSED Esc
    setvect(9, int9save);//rest int9
    readclk();
    return 0;
}