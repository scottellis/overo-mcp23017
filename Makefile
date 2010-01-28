#
# Use the OE built cross-tools to compile for an Overo 
#

# OETMP = ${OVEROTOP}/tmp

OETMP = /gum2/tmp

TOOLDIR = /$(OETMP)/cross/armv7a/bin

STAGEDIR = $(OETMP)/staging/armv7a-angstrom-linux-gnueabi/usr

CC = $(TOOLDIR)/arm-angstrom-linux-gnueabi-gcc

CFLAGS = -Wall

LIBDIR = $(STAGEDIR)/lib

INCDIR = $(STAGEDIR)/include

TARGET = mcptest

OBJS = main.o 

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L $(LIBDIR) -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -I $(INCDIR) -c main.c


clean:
	$(RM) $(TARGET) $(OBJS)

