global _start
extern kmain
extern boot_kernel

extern start_ctors	; beginning and end
extern end_ctors	; of the respective
extern start_dtors	; ctors and dtors section,
extern end_dtors	; declared by the linker script

;; http://www.uruk.org/orig-grub/boot-proposal.html
;; Ca a l'air powerfull.
	
%define MULTIBOOT_HEADER_MAGIC  0x1BADB002
;; Page-aligned, mem_* infos
%define MULTIBOOT_HEADER_FLAGS	0x00000003
%define CHECKSUM -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

;-- Entry point
_start:
	jmp start

;-- Multiboot header --
align 4

multiboot_header:
dd MULTIBOOT_HEADER_MAGIC
dd MULTIBOOT_HEADER_FLAGS
dd CHECKSUM     
;; Ca pourrait etre cool de rajouter des trucs ici ( Voir lien ).
;--/Multiboot header --

start:
	;; So far we're in 32 bits mode.
	mov esp, 0x1000000	; See kbase.hh
	push eax
	push ebx

	mov  ebx, start_ctors	; call the constructors
	jmp  .ctors_until_end
.call_constructor:
	call [ebx]
	add  ebx,4
.ctors_until_end:
	cmp  ebx, end_ctors
	jb   .call_constructor

	call boot_kernel	; DO NOT RETURN!

	cli ; stop interrupts
	hlt ; halt the CPU

