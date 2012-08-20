KNAME=	mykernel

# Configuring Makefile & output options
MAKEFLAGS+=	-rR --no-print-directory
RED_COLOR=	"\x1b[31;25m"
GREEN_COLOR=	"\x1b[32;25m"
ORANGE_COLOR=	"\x1b[33;25m"
NO_COLOR=	"\x1b[0m"

# Platform data
RM=	@rm -rf
GCC=	@gcc
GPP=	@g++
NASM=	@nasm

$(KNAME):
		make -C bootsect/

all:		$(KNAME)

clean:
		make clean -C bootsect/

fclean:
		make fclean -C bootsect/

re:
		make re -C bootsect/