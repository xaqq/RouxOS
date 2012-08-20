OUTPUT_FORMAT(elf32-i386)
ENTRY (_start)

SECTIONS
{
    . = 0x100000;
    kernel_start = .;
    .text ALIGN (0x1000) :
    {
	/* MUST be aligned */
        start_ctors = .;
        *(.ctor*)
        end_ctors = .;

        start_dtors = .;
        *(.dtor*)
        end_dtors = .;

        *(.text)
        *(.gnu.linkonce.t*)

	*(.rodata*) /* <---- ELF Cross Compiler or ELF *NIX (eg. Linux) */
    	*(.rdata*)  /* <---- COFF/PE MinGW or Cygwin on Windows */

        *(.data)
        *(.gnu.linkonce.d*)
        *(.gnu.linkonce.r*)
    }


    .bss :
    {
        sbss = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        ebss = .;
    }


/* discard this, unless you are implementing runtime support for C++ exceptions. */
/*    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame) 
    }*/
    kernel_end = .;
}