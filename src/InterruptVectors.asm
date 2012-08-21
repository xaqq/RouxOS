extern InterruptDefault
extern InterruptClock
extern InterruptKeyboard

global AsmInterruptDefault
global AsmInterruptClock
global AsmInterruptKeyboard

AsmInterruptDefault:
	call InterruptDefault
	mov al, 0x20
	out 0x20, al
	iret

AsmInterruptClock:
	call InterruptClock
	mov al, 0x20
	out 0x20, al
	iret

AsmInterruptKeyboard:
	call InterruptKeyboard
	mov al, 0x20
	out 0x20, al
	iret
	