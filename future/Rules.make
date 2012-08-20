# Configuring Makefile & output options
MAKEFLAGS	+=	-rR --no-print-directory
RED_COLOR	=	"\x1b[31;25m"
GREEN_COLOR	=	"\x1b[32;25m"
ORANGE_COLOR	=	"\x1b[33;25m"
NO_COLOR	=	"\x1b[0m"

ifndef $(RM)
RM		=	@rm -rf
endif
ifndef $(ASM)
ASM		=	@nasm
endif
#ASMFLAGS	+=	-f elf # elf64 if in kernel (elf is just for boot)
ifndef $(GCC)
GCC		=	gcc
endif
#CFLAGS		+=	-Wextra -Wall -ffreestanding -nostdlib -mcmodel=large \
		-mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 \
		-mno-3dnow -fno-common #Some will be deleted
#CINCLUDES=	-I includes/
ifndef $(GXX)
GXX	=	@g++
endif
# Exceptions need RTTI. RTTI is OS-dependent.
#CXXFLAGS=	-Wextra -Wall -nostartfiles -fno-exceptions -fno-rtti
#CXXINCLUDES=	-I includes/
ifndef $(LD)
LD	=	@ld
endif
#LDFLAGS	=	-T ./scripts/linker.ld # For kernel

$(NAME):	$(OBJ)
		@printf $(GREEN_COLOR)"\t====> Linking %s..."$(RED_COLOR)"\n" $(NAME)
		$(LD) $(LDFLAGS) -o $(KNAME) $(AOBJ) $(COBJ) $(CPPOBJ)
		@printf $(NO_COLOR)

all:		$(NAME)

clean:
		@printf $(GREEN_COLOR)"Cleaning directory %s..."$(NO_COLOR)"\n" `pwd`
		$(RM) *~
		$(RM) \#*#
		$(RM) $(AOBJ) $(CPPOBJ) $(COBJ)

fclean:		clean
		$(RM) $(NAME)

%/%.o : %.c
		@printf $(GREEN_COLOR)"\t - GCC %s"$(RED_COLOR)"\n" $<
		$(GCC) $(CFLAGS) $(CINCLUDES) -c $< -o $@

%/%.o : %.cpp
		@printf $(GREEN_COLOR)"\t - G++ %s"$(RED_COLOR)"\n" $<
		$(GXX) $(CXXFLAGS) $(CPPINCLUDES) -c $< -o $@

%/%.o : %.asm
		@printf $(GREEN_COLOR)"\t - NASM %s"$(RED_COLOR)"\n" $<
		$(ASM) $(ASMFLAGS) $(ASMINCLUDES) $< -o $@

re:		fclean \
		all

# Reset color in term.
reset:
		@printf $(NO_COLOR)