
APPLICATION = simple

# Hardware
BOARD ?= native

# OS path
RIOTBASE ?= $(CURDIR)/../riot

### Modules
# shell
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += gnrc_txtsnd

# net
USEMODULE += gnrc
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_pktdump



### compile
DEVELHELP ?= 1
QUIET ?= 1

### OS
include $(RIOTBASE)/Makefile.include
