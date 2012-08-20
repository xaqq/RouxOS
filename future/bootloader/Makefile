NAME	=	boot.bin
SRC	=	boot.asm
OBJ	=	$(SRC:.asm=.o)

RM	=	@rm -rf
ASM	=	@nasm
ASMFLAGS=	-f elf
LD	=	@ld
LDFLAGS	=	-melf_i386 -Ttext 0x100000

hack:	re

all:	$(OBJ)
	@printf "Making bootloader...\n"
	$(LD) $(LDFLAGS) $(OBJ) -o $(NAME)
	@printf "Done.\n"

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@