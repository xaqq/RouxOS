KNAME	=	RouxOS
TYPE	=	32
vpath %.c ./src/
vpath %.asm ./src/

vpath %.cpp ./CXXABI/
vpath %.cpp ./src/

ASRC=	Boot.asm		\
	BootUtils.asm		\
	InterruptVectors.asm

CSRC=

CPPSRC=	\
	GccGuards.cpp		\
	PureVirtual.cpp		\
	\
	BootProcess.cpp		\
	Interrupts.cpp		\
	Main.cpp		\
	PrintEngine.cpp		\
	\
	Debug.cpp		\
	Maths.cpp		\
	\
	String.cpp		\
	StaticString.cpp	\
	\
	CString.cpp		\
	\
	BLMalloc.cpp		\
	\
	Spinlock.cpp

AOBJ=	$(patsubst %.asm,objs/%.o,$(ASRC))
COBJ=	$(patsubst %.c,objs/%.o,$(CSRC))
CPPOBJ=	$(patsubst %.cpp,objs/%.o,$(CPPSRC))

# Configuring Makefile & output options
MAKEFLAGS+=	-rR --no-print-directory
RED_COLOR=	"\x1b[31;25m"
GREEN_COLOR=	"\x1b[32;25m"
ORANGE_COLOR=	"\x1b[33;25m"
NO_COLOR=	"\x1b[0m"

# Platform data
RM	=	@rm -rf
ASM	=	@nasm
ASMFLAGS=	
GCC	=	@gcc
CFLAGS	=	-Wextra -Wall -ffreestanding -nostdlib \
		-mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 \
		-mno-3dnow -fno-common
GXX	=	@g++

# See AMD64 ABI for red zone
# (In short, GCC generate interrupt-unsafe code using the 128b above stack pointer.)
# http://forum.osdev.org/viewtopic.php?t=21720
CXXFLAGS=	-Wextra -Wall -nostartfiles -fno-exceptions -fno-rtti -mno-red-zone\
		-fno-common -nostdlib -fno-builtin -nodefaultlibs -fno-stack-protector\
		-std=c++0x
CXXINCLUDES=	-I includes/
LD	=	@ld
LDFLAGS	=
CINCLUDES=	-I includes/

ifeq ($(TYPE), 32)
ASMFLAGS +=	-f elf
CFLAGS +=	-m32 -D_PTRSIZE=32
CXXFLAGS +=	-m32 -D_PTRSIZE=32
LDFLAGS +=	-T ./scripts/linker_32.ld -melf_i386
endif

ifeq ($(TYPE), 64)
ASMFLAGS +=	-f elf64
CFLAGS +=	-mcmodel=large -D_PTRSIZE=64 -Werror
CXXFLAGS +=	-mcmodel=large -D_PTRSIZE=64 -Werror
LDFLAGS +=	-T ./scripts/linker_64.ld
endif

all:		$(KNAME)

$(KNAME):	$(AOBJ) $(COBJ) $(CPPOBJ)
		@printf $(GREEN_COLOR)"\t====> Linking %s..."$(RED_COLOR)"\n" $(KNAME)
		$(LD) $(LDFLAGS) -o $(KNAME) $(AOBJ) $(COBJ) $(CPPOBJ)
		@printf $(NO_COLOR)

clean:
		@printf $(GREEN_COLOR)"Cleaning directory %s..."$(NO_COLOR)"\n" `pwd`
		$(RM) *~
		$(RM) \#*#
		$(RM) $(AOBJ) $(CPPOBJ) $(COBJ)

fclean:		clean
		$(RM) $(KNAME)

objs:
		@mkdir -p $@

objs/%.o : %.c objs
		@printf $(GREEN_COLOR)"\t - GCC %s"$(RED_COLOR)"\n" $<
		$(GCC) $(CFLAGS) $(CINCLUDES) -c $< -o $@

objs/%.o : %.cpp objs
		@printf $(GREEN_COLOR)"\t - G++ %s"$(RED_COLOR)"\n" $<
		$(GXX) $(CXXFLAGS) $(CXXINCLUDES) -c $< -o $@

objs/%.o : %.asm objs
		@printf $(GREEN_COLOR)"\t - NASM %s"$(RED_COLOR)"\n" $<
		$(ASM) $(ASMFLAGS) $(ASMINCLUDES) $< -o $@

re:		fclean \
		all

# Reset color in term.
reset:
		@printf $(NO_COLOR)

################## For testing ###################
GRUBPATH	=	/boot/grub
FLOPPYNAME	=	floppyA
TESTPATH	=	loop
# NB: Heredoc not working on Makefile...
GRUBINSTALLSCRIPT =\#!/bin/sh\n grub --device-map=$(TESTPATH)/grub/device.map $(FLOPPYNAME) <<EOF\nroot (fd0) floppyA\nsetup (fd0)\nquit\nEOF
GRUBMENULST	=title RouxOS!\nkernel /RouxOS\nboot

init_floppy:
	@dd if=/dev/zero of=$(FLOPPYNAME) bs=512 count=2880
	@mke2fs $(FLOPPYNAME)
	@mkdir -p $(TESTPATH)
	@mount -t ext2 $(FLOPPYNAME) $(TESTPATH)
	@mkdir -p $(TESTPATH)/grub
	@cp $(GRUBPATH)/stage{1,2} $(TESTPATH)/grub/
	@echo "(fd0) $(FLOPPYNAME)" > $(TESTPATH)/grub/device.map
	@echo -e '$(GRUBINSTALLSCRIPT)' > .tmp.sh && chmod +x .tmp.sh && ./.tmp.sh && rm -rf .tmp.sh
	@echo -e "$(GRUBMENULST)" > $(TESTPATH)/grub/menu.lst
	@umount $(TESTPATH)


$(FLOPPYNAME):	$(KNAME)
	@test -f $@ || $(MAKE) init_floppy
	@mkdir -p $(TESTPATH)
	@mount -t ext2 -o loop $@ $(TESTPATH)
	@cp -v $< $(TESTPATH)
	@umount $(TESTPATH)

test:	$(FLOPPYNAME)
	@qemu-kvm -m 512 -fda $(FLOPPYNAME)

mount:
	@mkdir -p $(TESTPATH)
	@mount -t ext2 -o loop $(FLOPPYNAME) $(TESTPATH)

umount:
	@umount $(TESTPATH)

.PHONY: all re clean fclean $(FLOPPYNAME)