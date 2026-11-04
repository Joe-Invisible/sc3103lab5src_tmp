# Default build tree: /lib/modules/<ver>/build
# Supply make with BUILD=... if intended build 
# tree location differs

CC = gcc
CFLAGS = -Wall -Wextra
VER := $(shell uname -r)
BUILD ?= /lib/modules/$(VER)/build

SRC = cdev_test.c
obj-m := $(SRC:.c=.o)

.PHONY: all clean

all:
	@echo "Module object: $(obj-m)"
	make -C $(BUILD) M=$(PWD) modules

clean:
	make -C $(BUILD) M=$(PWD) clean


