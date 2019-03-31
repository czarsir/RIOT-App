
/*** Base ***/
#include <stdio.h>

/*** Module ***/
#include "shell.h"
#include "net/gnrc.h"
#include "net/gnrc/pktdump.h"

/*** Variables ***/

/*** Functions ***/
int show(int cnt, char **arg);

/*** Special Variables ***/
const shell_command_t sh_cmd[] = {
	{"show", "Los geht's.", show},
	{ NULL, NULL, NULL }
};



int main(void)
{
	/*** Net dump ***/
	gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL,
														  gnrc_pktdump_pid);
	gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &dump);
	
	/*** Welcome ***/
	puts("It's a simple app!");
	printf("Go:\n");
	
	/*** shell ***/
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	shell_run(sh_cmd, line_buf, SHELL_DEFAULT_BUFSIZE);

	return 0;
}

int show(int cnt, char **arg)
{
	/* unused parameter */
	cnt = sizeof(*arg);
	(void)cnt;
	
	
	printf("my show time ~\n");
	
	return 0;
}
