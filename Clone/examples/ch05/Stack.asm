TITLE Program Template     (Stack.asm)

; Testing PUSH and POP, 16-bit and 32-bit operands
; Last update: 06/01/2006

Include Irvine32.inc

.code
main PROC

	call	DumpRegs
	push	1
	call DumpRegs
	push eax
	call DumpRegs
	push eax
	call DumpRegs

	exit
main ENDP
END main