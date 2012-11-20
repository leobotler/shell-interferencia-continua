CONTIKI_PROJECT = udp-sink
CONTIKI = /home/user/contiki
APPS = serial-shell powertrace collect-view
PROJECT_SOURCEFILES += collect-common.c shell-netcmd-ipv6.c shell-channel.c shell-udpcmd-ipv6.c

SMALL=1
WITH_UIP6=1
UIP_CONF_IPV6=1
CFLAGS+= -DUIP_CONF_IPV6_RPL
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

ifdef PERIOD
CFLAGS=-DPERIOD=$(PERIOD)
endif

all: $(CONTIKI_PROJECT)

include $(CONTIKI)/Makefile.include

