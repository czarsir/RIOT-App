
/*** Hardware ***/
#include <board.h>
#include "periph/gpio.h"
#include "periph/uart.h"

/*** Base ***/
#include <stdio.h>
//#include <unistd.h>

/*** System ***/
#include "thread.h"
#include "net/gnrc.h"

/*** Module ***/
#include "xtimer.h"
#include "shell.h"

/****** Macro ******/
#define MAIN_QUEUE_SIZE     (8)


/*** Variables ************************************************/
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
char rf_stack[THREAD_STACKSIZE_MAIN];
char stkSend[THREAD_STACKSIZE_MAIN];
static kernel_pid_t pidSend;


/*** Functions ************************************************/
int show(int cnt, char **arg);
int trigger(int cnt, char **arg);
int freqloop(int cnt, char **arg);
extern int _gnrc_netif_send(int argc, char **argv);
extern int udp_send(int argc, char **argv);
extern int udp_server(int argc, char **argv);
/* threads */
void *thread_handler_rf(void *arg);
void *thread_handler_send(void *arg);
/* interrupt */
static void _isr_button(void *arg);


/*** Special Variables ***/
const shell_command_t sh_cmd[] = {
	{ "udp", "send udp packets", udp_send },
	{ "udps", "start udp server", udp_server },
	{"show", "Los geht's.", show},
	{"trigger", "trigger InPhase-System", trigger},
	{"freqloop", "loop of sending diff freq", freqloop},
	{ NULL, NULL, NULL }
};
static char *txtsnd[4] = {
	"txtsnd",
	"6",
	"10:10:64:2d:1a:08:11:3e",
	"dfdfdf"
};
static char *udp_param[4] = {
	"udp",
	"fe80::1210:642d:1a08:113e",
	"8888",
	"net-udp-test"
};


/*** Application **********************************************/

int main(void)
{
	int i = 4;
	msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

	/*** create thread for rf ***/
	thread_create(rf_stack, sizeof(rf_stack),
				  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
				  thread_handler_rf, NULL, "thread rf");
	/*** create thread for send frame ***/
	pidSend = thread_create(stkSend, sizeof(stkSend),
				  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
				  thread_handler_send, NULL, "thread send frame");

	/*** Interrupt: blue Button ***/
	gpio_init_int(GPIO_PIN(PORT_A, 0), GPIO_IN, GPIO_RISING, _isr_button, NULL);

	/*** UART: UART_DEV(1) ***/
	uint8_t bufByte='D';
	bufByte = uart_init(UART_DEV(1), 115200, NULL, NULL);
	//printf("uart_init: %u\n", bufByte);
	bufByte='D';
	uart_write(UART_DEV(1), (const uint8_t *)&bufByte, (size_t)1);

	/*** main process ***/
    printf("Go:\n");
	
	/*** LED ***/
	//puts("It's a simple app!");
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

	//puts("--->>> Thread [rf] starts.");

	while(1)
	{
		msg_receive(&m);
		puts("Got one Message.");
	}

	return NULL;
}

void *thread_handler_send(void *arg)
{
	(void)arg;
	msg_t m;

	//puts("--->>> Thread [send] starts.");

	while(1)
	{
		msg_receive(&m);
		//puts("Send one frame.");
		//_gnrc_netif_send(4, txtsnd);
		(void)txtsnd;
		//udp_send(4, udp_param);
		(void)udp_param;

		puts("InPhase test.");
		netopt_enable_t enable = NETOPT_DISABLE;
		gnrc_netapi_get(6, NETOPT_IPS_START, 0, &enable, sizeof(enable));

		/*** debouncing ***/
		xtimer_sleep(1);
	}

	return NULL;
}

static void _isr_button(void *arg)
{
	(void) arg;
	msg_t m;

	//puts("[_isr_button]");
	//_gnrc_netif_send(4, txtsnd);

	msg_send(&m, pidSend);
}

//extern void dingtest(void);
//extern void dingpmu(void);
//extern void dingpmustart(void);
//extern void dingpmustop(void);

int show(int cnt, char **arg)
{
	/* unused parameter */
	(void)arg;
	
	switch (cnt) {
		case 1:
//			printf("dingtest\n");
//			dingtest();
//			break;
		case 2:
//			printf("dingpmu\n");
//			dingpmu();
//			break;
		case 3:
//			dingpmustart();
//			break;
		case 4:
//			dingpmustop();
//			break;
		default:
			printf("my show time ~ %d\n", cnt);
	}
	
	return 0;
}

int trigger(int cnt, char **arg)
{
	msg_t m;
	cnt = sizeof(*arg);
	(void)cnt;

	msg_send(&m, pidSend);

    return 0;
}

int freqloop(int cnt, char **arg)
{
	uint16_t channel;
	/* unused parameter */
	cnt = sizeof(*arg);
	(void)cnt;

	udp_param[1] = arg[1];
	udp_param[3] = "0000000000000000";

	printf("freqloop: start\n");

	for (channel=0; channel<100; channel++) {
		gnrc_netapi_set(6, NETOPT_CHANNEL, 0, &channel, sizeof(channel));
		udp_send(4, udp_param);
	}

	printf("freqloop: stop\n");

	channel = 26;
	gnrc_netapi_set(6, NETOPT_CHANNEL, 0, &channel, sizeof(channel));
	udp_send(4, udp_param);

	printf("freqloop: recheck\n");

	return 0;
}
