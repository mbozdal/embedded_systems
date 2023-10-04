; startup.s - Bare Minimum Startup Code for ARM Cortex-M

Stack_Size EQU 0x400 ; 1KB stack size
	
               AREA STACK,NOINIT,READWRITE,ALIGN=3
__stack_limit
Stack_Mem       SPACE Stack_Size
__initial_sp


; Vector Table Mapped to Address 0 at Reset
	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors DCD __initial_sp ; Top of Stack
          DCD Reset_Handler ; Reset Handler

	AREA |.text|, CODE, READONLY
Reset_Handler   PROC
	EXPORT Reset_Handler [Weak]
	IMPORT __main
	LDR R0, =__main
	BX  R0
	ENDP
	EXPORT   __stack_limit
	EXPORT   __initial_sp
	END
