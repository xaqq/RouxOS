KNAME	=	RouxOS
TYPE	=	32
vpath %.c ./src/
vpath %.asm ./src/

vpath %.cpp ./CXXABI/
vpath %.cpp ./src/

ASRC=	Boot.asm		\
	BootUtils.asm

CSRC=

CPPSRC=	\
	GccGuards.cpp		\
	PureVirtual.cpp		\
	\
	BootProcess.cpp		\
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

objs/%.o : %.c
		@printf $(GREEN_COLOR)"\t - GCC %s"$(RED_COLOR)"\n" $<
		$(GCC) $(CFLAGS) $(CINCLUDES) -c $< -o $@

objs/%.o : %.cpp
		@printf $(GREEN_COLOR)"\t - G++ %s"$(RED_COLOR)"\n" $<
		$(GXX) $(CXXFLAGS) $(CXXINCLUDES) -c $< -o $@

objs/%.o : %.asm
		@printf $(GREEN_COLOR)"\t - NASM %s"$(RED_COLOR)"\n" $<
		$(ASM) $(ASMFLAGS) $(ASMINCLUDES) $< -o $@

re:		fclean \
		all

.PHONY: all re clean fclean

# Reset color in term.
reset:
		@printf $(NO_COLOR)

################## SPECIAL... USE IT WITH CAUTION ###################
GRUBPATH	=	/mnt/dump/boot/grub/
FLOPPYNAME	=	floppyA
TESTPATH	=	/mnt/loop/
LOOPPATH	=	/dev/loop0
init_floppy:
	@./scripts/grub_floppy_install.sh $(TESTPATH) $(LOOPPATH) \
	$(GRUBPATH) $(FLOPPYNAME)

test:	re
	@mkdir -p $(TESTPATH)
	@./scripts/mount.sh $(TESTPATH) $(LOOPPATH) $(FLOPPYNAME)
	@./scripts/update.sh $(TESTPATH) $(LOOPPATH) $(FLOPPYNAME) $(KNAME)
	@./scripts/umount.sh $(TESTPATH) $(LOOPPATH)
	@./scripts/test.sh $(LOOPPATH) $(FLOPPYNAME)

mount:
	@./scripts/mount.sh $(TESTPATH) $(LOOPPATH) $(FLOPPYNAME)

umount:
	@./scripts/umount.sh $(TESTPATH) $(LOOPPATH)