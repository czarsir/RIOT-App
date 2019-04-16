
APPLICATION = simple

# Hardware
BOARD ?= stm32f4discovery

# OS path
RIOTBASE ?= $(CURDIR)/../riot


### Modules

# base
#USEMODULE += posix
USEMODULE += xtimer

# shell
USEMODULE += shell
USEMODULE += shell_commands
#USEMODULE += gnrc_txtsnd
#USEMODULE += gnrc_pktdump

# net
#USEMODULE += gnrc
#USEMODULE += gnrc_netdev_default
#USEMODULE += auto_init_gnrc_netif

# at86rf215
USEMODULE += at86rf2xx
#USEMODULE += at86rf215



### compile
DEVELHELP ?= 1
QUIET ?= 1

### OS
include $(RIOTBASE)/Makefile.include
