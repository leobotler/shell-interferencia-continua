/**
 * \file
 *         Source file for channel assignment in a Contiki shell command
 * \author
 *         Lucas Lira Gomes <x8lucas8x@gmail.com>
 */

#include "contiki.h"
#include "shell.h"

#include "dev/cc2420.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
PROCESS(shell_channel_process, "channel");
SHELL_COMMAND(channel_command,
	      "channel",
	      "channel [num]: change to channel [num] or just see the current channel",
	      &shell_channel_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_channel_process, ev, data)
{
  const char *nextptr;
  unsigned long num;

  PROCESS_BEGIN();

  if(data != NULL) {
    num = shell_strtolong(data, &nextptr);
    if(nextptr != data) {
      if ( (num >= 11) && (num <= 26) )
        cc2420_set_channel(num);
      PROCESS_EXIT();
    }
  }

  char msg[5];
  num = cc2420_get_channel();

  sprintf(msg, "%lu", num);
  shell_output(&channel_command, msg, strlen(msg), "", 0);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_channel_init(void)
{
  shell_register_command(&channel_command);
}
/*---------------------------------------------------------------------------*/
