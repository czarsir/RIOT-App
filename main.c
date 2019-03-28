
/*** Base ***/
#include <stdio.h>

/*** Module ***/
#include "shell.h"


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
	
	
	printf("my show time ~\n");
	
	return 0;
}
