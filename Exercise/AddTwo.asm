; AddTwo.asm - adds two 32-bit integers.
; Chapter 3 example
INCLUDE Irvine32.inc

.386
.model flat,stdcall

.data

.stack 4096

ExitProcess proto,dwExitCode:dword
;prototype 原型

.code
main proc
	mov	eax,5				
	add	eax,6				
	invoke DumpRegs

    invoke WaitMsg

	invoke ExitProcess,0
main endp
end main