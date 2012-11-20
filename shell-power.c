/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#include "contiki.h"
#include "shell.h"

#include "dev/cc2420.h"
#include "dev/cc2420_const.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
PROCESS(shell_power_process, "power");
SHELL_COMMAND(power_command,
	      "power",
	      "power [num]: change to power [num] or just see the current power",
	      &shell_power_process);
	      
	      /*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_power_process, ev, data)
{
  const char *nextptr;
  unsigned long num;

  PROCESS_BEGIN();
  
  CC2420_WRITE_REG(CC2420_MAIN, 0xF800);
  CC2420_STROBE(CC2420_SXOSCON);
  CC2420_WRITE_REG(CC2420_MDMCTRL1, 0X000C); // 0X000C
  CC2420_WRITE_REG(CC2420_DACTST, 0X1800); // TESTAR WHITE NOISE COM 0X4800


  if(data != NULL) {
    num = shell_strtolong(data, &nextptr);
    if(nextptr != data) {
      if ( (num >= 0) && (num <= 31) )
        cc2420_set_txpower(num);
      PROCESS_EXIT();
    }
  }

  char msg[5];
  num = cc2420_get_txpower();

  sprintf(msg, "%lu", num);
  shell_output(&power_command, msg, strlen(msg), "", 0);
  
   CC2420_STROBE(CC2420_STXONCCA);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
shell_power_init(void)
{
  shell_register_command(&power_command);
}
/*---------------------------------------------------------------------------*/

//PROCESS_THREAD(udp_client_process, ev, data)
//{
//  PROCESS_BEGIN();
//
////RESET
//  //CC2420_WRITE_REG(CC2420_MAIN, 0x0000);
//  CC2420_WRITE_REG(CC2420_MAIN, 0xF800);
//  printf("CC2420_MAIN: %X\n", getreg(CC2420_MAIN));
//
//  //SETA O OSCILADOR E ESPERA ESTABILIZAR
//  CC2420_STROBE(CC2420_SXOSCON);
//  NOP_DELAY();
//
///*
//  CC2420_WRITE_REG(CC2420_MANOR, 0X0100);
//  printf("CC2420_MANOR: %X\n", getreg(CC2420_MANOR));
//  CC2420_WRITE_REG(CC2420_TOPTST, 0X0004);
//  printf("CC2420_TOPTST: %X\n", getreg(CC2420_TOPTST));
//*/
//  //MODO DE TRANSMISSAO 3
//  CC2420_WRITE_REG(CC2420_MDMCTRL1, 0X000C); // 0X000C
//  printf("CC2420_MDMCTRL1: %X\n", getreg(CC2420_MDMCTRL1));
//  CC2420_WRITE_REG(CC2420_DACTST, 0X1800); // TESTAR WHITE NOISE COM 0X4800
//  printf("CC2420_DACTST: %X\n", getreg(CC2420_DACTST));
//
//  NOP_DELAY();
//  NOP_DELAY();
//  NOP_DELAY();
//  NOP_DELAY();
//
//
///*  
//  CC2420_WRITE_REG(CC2420_MANOR, 0X0100);
//  CC2420_WRITE_REG(CC2420_TOPTST, 0X0004);
//  CC2420_WRITE_REG(CC2420_MDMCTRL1, 0X0508);
//  CC2420_WRITE_REG(CC2420_DACTST, 0X1800);
//  CC2420_STROBE(CC2420_STXON);
//
//  CC2420_WRITE_REG(CC2420_MANOR, 0X000C);
//  CC2420_WRITE_REG(CC2420_TOPTST, 0X0004);
//  CC2420_WRITE_REG(CC2420_MDMCTRL1, 0X0508);
//  CC2420_WRITE_REG(CC2420_DACTST, 0X1800);
//  CC2420_STROBE(CC2420_STXON);
//*/
//  //leo
//  cc2420_set_txpower(pot);
//  cc2420_set_channel(26);
//  
//  printf("ok\n");
//
//  while(1) {
////    PROCESS_YIELD();
//    CC2420_STROBE(CC2420_STXONCCA);
//  }
//  
//  PROCESS_END();
//}
///*---------------------------------------------------------------------------*/
//
