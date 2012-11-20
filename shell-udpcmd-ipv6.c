
/**
 * \file
 *         Source file for udpcmd ipv6-specific Contiki shell command
 * \author
 *         Lucas Lira Gomes <x8lucas8x@gmail.com>
 */

#include "contiki.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-debug.h"
#include "simple-udp.h"

#include "shell-udpcmd-ipv6.h"

#include <stdio.h>
#include <string.h>

#define UDP_PORT 1235
#define MAX_SERVERLEN 100
#define MAX_COMMANDLEN 25

static uip_ipaddr_t serveraddr;
static char server[MAX_SERVERLEN + 1];
static char command[MAX_COMMANDLEN + 1];

static struct simple_udp_connection unicast_connection;
/*---------------------------------------------------------------------------*/
PROCESS(shell_udpcmd_ipv6_process, "udpcmd6");
PROCESS(shell_udpcmd_ipv6_server_process, "udpcmd6 server");
SHELL_COMMAND(udpcmd_ipv6_command,
               "udpcmd6",
               "udpcmd6 <host> <command>: run a command on one specific node in the network",
               &shell_udpcmd_ipv6_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_udpcmd_ipv6_server_process, ev, data)
{
  static struct process *child_command;
  int err;
  PROCESS_BEGIN();

  /* XXX: direct output to null. */
  printf("udpcmd6 got command string '%s'\n", (char *)data);
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
PROCESS_THREAD(shell_udpcmd_ipv6_process, ev, data)
{
  char *next;
  int len;
  //uip_ipaddr_t *addr;
  
  PROCESS_BEGIN();

  /* Get the length of the command line, excluding a terminating NUL character. */
  len = strlen((char *)data);

  next = strchr(data, ' ');
  if((next == NULL) || (len == 0)) {
    shell_output_str(&udpcmd_ipv6_command, "usage: ", udpcmd_ipv6_command.description);
    PROCESS_EXIT();
  }
  *next = 0;
  ++next;
  strncpy(server, data, sizeof(server));
  //strcpy (server, "fe80:0000:0000:0000:c30c:0000:0000:006f");
  strncpy(command, next, sizeof(command));
  printf ("dado %s\nip %s\n", command, server);

  uiplib_ipaddrconv(server, &serveraddr);

  printf("udpcmd sending '%s'\n", command);

	uiplib_ipaddrconv(server, &serveraddr); //mote 11
  simple_udp_sendto(&unicast_connection, command, strlen(command) + 1, &serveraddr);
  //Escrever código que envie para um endereço fixo! -- viana
  
 // uiplib_ipaddrconv("fe80:0000:0000:0000:c30c:0000:0000:006f", &serveraddr);
 // uip_udp_packet_sendto(client_conn, " blink 10", strlen(" blink 10"),
 //                       &serveraddr, UIP_HTONS(1235));
  
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
  printf("Data received from ");
  uip_debug_ipaddr_print(sender_addr);
  printf(" on port %d from port %d with length %d: '%s'\n",
         receiver_port, sender_port, datalen, data);
    
    /* First ensure that the old process is killed. */
    process_exit(&shell_udpcmd_ipv6_server_process);
    
    /* Start the server process with the incoming command. */
    process_start(&shell_udpcmd_ipv6_server_process, (char *)data);
}
/*---------------------------------------------------------------------------*/
void
shell_udpcmd_ipv6_init(void)
{
  simple_udp_register(&unicast_connection, UDP_PORT,
                      NULL, UDP_PORT, receiver);

  shell_register_command(&udpcmd_ipv6_command);
}
/*---------------------------------------------------------------------------*/
