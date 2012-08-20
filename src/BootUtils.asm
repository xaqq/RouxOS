;; ---------------------------------------------------------
;;  Synopsis:	Affiche une chaine de caracteres se terminant par 0x0
;;  Entree:	ds:si -> pointe sur la chaine a afficher
;;  Made by Mr Michelizza. Merci a vous!
;; ---------------------------------------------------------
[BITS 32]
global bios_kprint

bios_kprint:
	push ax
	push bx
.debut:
	lodsb			; ds:si -> al
	cmp al,0		; fin chaine ?
	jz .fin
	mov ah,0x0E		; appel au service 0x0e, int 0x10 du bios
	mov bx,0x07		; bx -> attribut, al -> caractere ascii
	int 0x10
        jmp .debut

.fin:
	pop bx
	pop ax
	ret