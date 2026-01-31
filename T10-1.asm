;程序名：T10-1.asm
;功能：演示实模式和保护模式的切换
;运行环境：386以上处理器，DOS操作系统。
;编译环境：使用TASM或MASM编译器，TLINK链接器需使用32位选项"/3"；
;ml /c E:\Source\Assembly\T10-1.asm
;link16 T10-1.obj, T10-1.exe;
;调试运行时，不要安装扩展内存驱动程序，否则会发生冲突
;16位偏移的段间直接转移指令的宏定义
;-------------------------------------------------------------------------

JUMP MACRO selector,offsetv
	DB	0EAH		;操作码
	DW	offsetv		;16位偏移
	DW	selector	;段值或者段选择子
	ENDM
	
;字符显示宏指令的定义
ECHOCH	MACRO ascii
	mov ah,2
	mov dl,ascii
	int 21h
	ENDM
	
;存储段描述符结构类型的定义
DESCRIPTOR STRUC
	LIMITL		dw  0	;段界限（0~15）
	BASEL		dw  0	;段基址（0~15）
	BASEM		db	0	;段基址（16~23）
	ATTRIBUTES	dw	0	;段属性 16位
	BASEH		db	0	;段基址（24~31）
DESCRIPTOR ENDS

;伪描述符结构类型的定义
PDESC	STRUC
	LIMIT	DW	0	;16位界限
	BASE 	DD	0	;基地址
PDESC	ENDS

;常量定义，G位为0，D/B位为0，16位代码段和数据段，界限以字节为单位
ATDW	=   0092H	;存在的可读写数据段属性值
ATCE	=	0098H	;存在的只执行代码段属性值
;

.386P
;----------------------------------------------------
;数据段
DSEG	SEGMENT	USE16	;16位段
GDT	LABEL BYTE			;全局描述符表GDT
DUMMY	DESCRIPTOR<>	;空描述符
CODE_SEG	DESCRIPTOR<0FFFFH,,,ATCE,>	;
CODE_SEL	= CODE_SEG - GDT	;代码段描述符的选择子=描述符表起始位置的位移量
DATAS	DESCRIPTOR<0FFFFH,0H,11H,ATDW,0>	;数据段描述符,基址=00110000H，界限0FFFFH
DATAS_SEL	= DATAS - GDT	;数据段描述符的选择子=描述符表起始位置的位移量
DATAD	DESCRIPTOR<0FFFFH,,,ATDW,>	;目标数据段
DATAD_SEL 	= DATAD - GDT			;目标数据段描述符的选择子
GDTLEN		= $ - GDT				;描述符表的长度
;
VGDTR	PDESC<GDTLEN-1,>		;伪描述符 ,界限GDTLEN-1+基址0
;
BUFFERLEN	= 256				;缓冲区字节长度
BUFFER 		DB BUFFERLEN DUP(0)	;缓冲区

DSEG ENDS

;代码段
CSEG	SEGMENT	USE16

ASSUME	CS:CSEG,DS:DSEG
START:
	MOV AX,DSEG
	MOV DS,AX
	;准备要加载到GDTR的伪描述符
	MOV BX,16			;数据段值左移4位
	MUL BX				;计算并设置GDT基地址
	ADD AX,OFFSET GDT	;+GDT偏移地址 = GDT物理地址。界限已在定义时设置妥当GDTLEN
	ADC DX,0
	MOV WORD PTR VGDTR.BASE,AX
	MOV WORD PTR VGDTR.BASE+2,DX
	;设置代码段描述符
	MOV AX,CS
	MUL BX
	MOV CODE_SEG.BASEL,AX	;代码段开始偏移为0
	MOV CODE_SEG.BASEM,DL	;代码段界限已在定义时设置妥当OFFFFH
	MOV CODE_SEG.BASEH,DH
	;设置目标数据段描述符
	MOV AX,DS
	MUL BX				;计算并设置目标数据段基地址
	ADD AX,OFFSET BUFFER
	ADC DX,0
	MOV DATAD.BASEL,AX
	MOV DATAD.BASEM,DL
	MOV DATAD.BASEH,DH
	;加载GDTR
	LGDT FWORD PTR VGDTR
	;
	CLI					;关中断
	CALL ENABLEA20		;打开地址线A20
	;切换到保护模式
	MOV EAX,CR0
	OR EAX,1			;CR0的PE位置1，切换到保护模式
	MOV CR0,EAX
	;清指令预取队列，并真正进入保护模式
	JUMP CODE_SEL, OFFSET VIRTUAL
	;
VIRTUAL:	;现在开始在保护方式下
	MOV AX,DATAS_SEL
	MOV DS,AX			;加载源数据段描述符
	MOV AX,DATAD_SEL
	MOV ES,AX			;加载目标数据段描述符
	CLD
	XOR SI,SI			;设置指针初始值
	XOR DI,DI
	MOV CX,BUFFERLEN/4	;设置4字节为单位的缓冲区长度
	REP MOVSD			;传送
	;切换回实模式
	MOV EAX,CR0
	AND EAX,0FFFFFFFEH	;PE位置0
	MOV CR0,EAX
	;清指令预取队列，并真正进入实模式
	JUMP SEG REAL_MODE, OFFSET REAL_MODE
	;
REAL_MODE:
	CALL DISABLEA20		;关闭地址线A20
	STI					;开中断
	;
	MOV AX,DSEG			;重置数据段寄存器
	MOV DS,AX
	MOV SI,OFFSET BUFFER
	;
	CLD					;显示缓冲区内容,16个字节一行
	MOV BP,BUFFERLEN/16
NEXTLINE:
	MOV CX,16
NEXTCH:
	LODSB
	PUSH AX
	SHR AL,4
	CALL TOASCII
	ECHOCH AL
	POP AX
	CALL TOASCII
	ECHOCH AL
	ECHOCH ' '
	LOOP NEXTCH
	ECHOCH 0DH
	ECHOCH 0AH
	DEC BP
	JNZ NEXTLINE
	;
	MOV AX,4C00H
	INT 21H

;================================================
TOASCII PROC
	AND AL,0FH
	ADD AL,30H
	CMP AL,'9'
	JNA L1
	ADD AL,07H
L1:
	RET

TOASCII ENDP
;-------------------------------------------
;打开地址线A20
ENABLEA20 PROC
	PUSH AX
	IN AL,92H
	OR AL,2		;第1位置1
	OUT 92H,AL
	POP AX
	RET
ENABLEA20 ENDP
;-------------------------------------------
;关闭地址线A20
DISABLEA20 PROC
	PUSH AX
	IN AL,92H
	AND AL,0FDH	;第1位置0
	OUT 92H,AL
	POP AX
	RET

DISABLEA20 ENDP
;-------------------------------------------

CSEG ENDS

	END START