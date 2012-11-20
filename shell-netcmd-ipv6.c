/**
 * \file
 *         Source file for netcmd ipv6-specific Contiki shell command
 * \author
 *         Lucas Lira Gomes <x8lucas8x@gmail.com>
 */

#include "contiki.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "simple-udp.h"

#include "shell-netcmd-ipv6.h"

#include <stdio.h>
#include <string.h>

#define UDP_PORT 1234

static struct simple_udp_connection broadcast_connection;
/*---------------------------------------------------------------------------*/
PROCESS(shell_netcmd_ipv6_process, "netcmd6");
PROCESS(shell_netcmd_ipv6_server_process, "netcmd6 server");
SHELL_COMMAND(netcmd_ipv6_command,
               "netcmd6",
               "netcmd6 <command>: run a command on all nodes in the network",
               &shell_netcmd_ipv6_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_netcmd_ipv6_server_process, ev, data)
{
  static struct process *child_command;
  int err;
  PROCESS_BEGIN();

  /* XXX: direct output to null. */
  printf("netcmd6 got command string '%s'\n", (char *)data);
  err = shell_start_command(data, strlen((char * )data), NULL, &child_command);
  if(err == SHELL_FOREGROUND && process_is_running(child_command)) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_EXIT ||
			     (ev == PROCESS_EVENT_EXITED &&
			      data == child_command));
    if(ev == PROCESS_EVENT_EXIT) {
      process_exit(child_command);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_netcmd_ipv6_process, ev, data)
{
  int len;
  uip_ipaddr_t addr;
  
  PROCESS_BEGIN();

  /* Get the length of the command line, excluding a terminating NUL character. */
  len = strlen((char *)data);

  if(len == 0) {
    shell_output_str(&netcmd_ipv6_command, "usage: ", netcmd_ipv6_command.description);
    PROCESS_EXIT();
  }

  printf("netcmd sending '%s'\n", data);
  uip_create_linklocal_allnodes_mcast(&addr);
  simple_udp_sendto(&broadcast_connection, data, len, &addr);
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  printf("Data received on port %d from port %d with length %d\n",
         receiver_port, sender_port, datalen);
    
    /* First ensure that the old process is killed. */
    process_exit(&shell_netcmd_ipv6_server_process);
    
    /* Start the server process with the incoming command. */
    process_start(&shell_netcmd_ipv6_server_process, (char *)data);
}
/*---------------------------------------------------------------------------*/
void
shell_netcmd_ipv6_init(void)
{
  simple_udp_register(&broadcast_connection, UDP_PORT,
                      NULL, UDP_PORT,
                      receiver);

  shell_register_command(&netcmd_ipv6_command);
}
/*---------------------------------------------------------------------------*/
