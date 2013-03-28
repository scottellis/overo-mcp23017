#
# Use the cross-tools built by the Yocto project to compile for an Overo 
#

### cross-build defs ###

ifeq ($(strip $(OETMP)),)
# make things fail if OETMP not defined, point to an invalid location
	OETMP=/tmp
endif


TOOLDIR = $(OETMP)/sysroots/`uname -m`-linux/usr/bin
STAGEDIR = ${OETMP}/sysroots/overo/usr

CC = ${TOOLDIR}/armv7a-vfp-neon-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc
CFLAGS = -Wall

LIBDIR = $(STAGEDIR)/lib
INCDIR = $(STAGEDIR)/include

### end cross-build defs ###


TARGET = mcptest

$(TARGET) : main.c
	$(CC) $(CFLAGS) -I $(INCDIR) -L $(LIBDIR) main.c -o $(TARGET)


install:
	scp -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no $(TARGET) root@192.168.10.112:/home/root


clean:
	rm -f $(TARGET)

