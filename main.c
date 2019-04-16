
/*** Hardware ***/
#include <board.h>

/*** Base ***/
#include <stdio.h>
//#include <unistd.h>

/*** System ***/
#include "thread.h"

/*** Module ***/
#include "xtimer.h"
#include "shell.h"

/****** Macro ******/
#define MAIN_QUEUE_SIZE     (8)


/*** Variables ************************************************/
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
char rf_stack[THREAD_STACKSIZE_MAIN];


/*** Functions ************************************************/
int show(int cnt, char **arg);
extern int udp_send(int argc, char **argv);
extern int udp_server(int argc, char **argv);
/* threads */
void *thread_handler_rf(void *arg);


/*** Special Variables ***/
const shell_command_t sh_cmd[] = {
	{ "udp", "send udp packets", udp_send },
	{ "udps", "start udp server", udp_server },
	{"show", "Los geht's.", show},
	{ NULL, NULL, NULL }
};


/*** Application **********************************************/

int main(void)
{
	int i = 10;
	msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
	
    puts("It's a simple app!");

	/*** create thread for rf ***/
	thread_create(rf_stack, sizeof(rf_stack),
				  THREAD_PRIORITY_MAIN - 1,
				  THREAD_CREATE_STACKTEST,
				  thread_handler_rf,
				  NULL, "thread rf");

	/*** main process ***/
    printf("Go:\n");
	
	/*** LED ***/
	puts("It's a simple app!");
	LED0_ON;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	while(i--)
	{
//		usleep(1000000);
		xtimer_sleep(1);
		LED0_TOGGLE;
		LED1_TOGGLE;
		LED2_TOGGLE;
		LED3_TOGGLE;
	}

	/*** shell ***/
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	shell_run(sh_cmd, line_buf, SHELL_DEFAULT_BUFSIZE);
	
    return 0;
}

void *thread_handler_rf(void *arg)
{
	(void)arg;
	msg_t m;

	puts("--->>> Thread rf starts.");

	while(1)
	{
		msg_receive(&m);
		puts("Got one Message.");
	}

	return NULL;
}

int show(int cnt, char **arg)
{
	/* unused parameter */
	cnt = sizeof(*arg);
	
	(void)cnt;
	
	printf("my show time ~\n");
	
	return 0;
}
