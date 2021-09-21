# Copyright (c) 2021 Hugues Larrive <hlarrive@pm.me>
#
# SPDX-License-Identifier: CECILL-2.1
#

APPLICATION = am2301-wifi-sensor
BOARD = esp8266-esp-12x

# Wifi configuration
include $(CURDIR)/Makefile.wificonf

# permit modules to include config files
INCLUDES += -I$(CURDIR)

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# DEVELHELP enabled by default for all tests, set 0 to disable
DEVELHELP ?= 1

FEATURES_REQUIRED += periph_spi

# Modules to include:
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += core_idle_thread
# Uncomment this to enable scheduler statistics for ps:
USEMODULE += schedstatistics

# enable debug output via UART
ifneq (native, $(BOARD))
  FEATURES_OPTIONAL += periph_uart
endif

# lwIP
USEMODULE += ipv4_addr
USEMODULE += lwip_arp
USEMODULE += lwip_ipv4
USEMODULE += lwip_dhcp_auto
CFLAGS += -DETHARP_SUPPORT_STATIC_ENTRIES=1
USEMODULE += lwip lwip_netdev
USEMODULE += lwip_udp
USEMODULE += lwip_tcp
USEMODULE += sock_async_event
USEMODULE += sock_ip
USEMODULE += sock_tcp
USEMODULE += sock_udp
USEMODULE += sock_util
USEMODULE += netdev_default

# lwip_netdev_mux will need more than the default 1024 bytes
CFLAGS += -DTHREAD_STACKSIZE_DEFAULT=2048

#~ CFLAGS += -DMEM_DEBUG=LWIP_DBG_ON
# The javascript queries the sensor data every two seconds but after
# about on minute the requests failed due to memory saturation.
# LwIP uses dynamic memory allocation and after netconn_delete() memory
# is not released until the LAST WAIT timer expires (2 MSF of 60s) so we
# set the MSF to 1 second to prevent memory saturation.
CFLAGS += -DTCP_MSL=1000

# lwIP httpd
CFLAGS += -DLWIP_HTTPD_CGI=1
CFLAGS += -DLWIP_HTTPD_SSI=1
CFLAGS += -DLWIP_HTTPD_FILE_STATE=1
CFLAGS += -DLWIP_HTTPD_CGI_SSI=1
CFLAGS += -D'HTTPD_FSDATA_FILE="$(CURDIR)/webui/fsdata.h"'
#~ CFLAGS += -DHTTPD_DEBUG=LWIP_DBG_ON
TCPIP_PATCH_EXISTS = $(shell [ -e $(RIOTBASE)/pkg/lwip/patches/0001-Enable-apps-http-and-add-httpd_init-into-tcpip_threa.patch ] && echo 1 || echo 0)
ifneq ($(TCPIP_PATCH_EXISTS), 1)
  $(shell cp $(CURDIR)/webui/0001-Enable-apps-http-and-add-httpd_init-into-tcpip_threa.patch $(RIOTBASE)/pkg/lwip/patches/)
endif
EXTERNAL_MODULE_DIRS += $(RIOTBASE)/build/pkg/lwip/src/apps
USEMODULE += http
$(shell cd $(CURDIR)/webui && ./makefsdata)

# AM2301 (DHT21)
USEMODULE += dht
CFLAGS += -DDHT_PARAM_PIN=2
CFLAGS += -DDHT_PARAM_TYPE=1

EXTERNAL_MODULE_DIRS += $(CURDIR)
INCLUDES += -I$(CURDIR)
USEMODULE += am2301
USEMODULE += webui

include $(RIOTBASE)/Makefile.include
