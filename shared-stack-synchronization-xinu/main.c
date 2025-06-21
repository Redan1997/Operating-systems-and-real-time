#include <conf.h>
#include <kernel.h>
#define N 10 //define var

void LocalPush(int i);
int LocalPop();

	int mutex;//lock stack when there is change (mutex)
	int stackEmpty;//check if stack if empty and disallow entering num with no place in stack
	int stackFull;//DisAllow entering number more than N
	/*gobal data for stack*/
	int INDEX=0;
	int stack[N];
void xmain()
{
	int temp;
	int i;
	void func();
	mutex = screate(1);
	stackEmpty = screate(0);//Disallow pop from empty stack
	stackFull = screate(N);//allow N push num
	resume(create(func, INITSTK, INITPRIO, "proc 1", 0));
	for (i = 0; i < N; i++)
	{
		temp = LocalPop();
		printf("pop#%d = %d\n", i, temp);
	}
} /* xmain */

void func()
{
	int i;

	for (i = 0; i < N; i++)
	{
		if (0 == i % 2)sleep(3);
		LocalPush(i);
		printf("push %d \n", i);
	} /* for */
	return;
}
int LocalPop() {
	int temp;
	wait(stackEmpty);//check if stack empty
	wait(mutex);
	temp = stack[--INDEX];//delete num from stack
	signal(mutex);//unlock the stack
	signal(stackFull);
	return temp;
}

void LocalPush(int i) {
	wait(stackFull);//CHECK IF STACK IS FULL
	wait(mutex);//lock stack for action
	stack[INDEX++] = i;//insert num to stack and update the index
	signal(mutex);//unlock stack
	signal(stackEmpty);
}