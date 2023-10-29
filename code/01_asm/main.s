; Define GPIO register addresses for controlling the LED, reading buttons, and delay

	INCLUDE constants.s


delayCount   EQU 0xFFFFF ; Delay counter for the loop
BUTTON4_MASK EQU 0x0010          ; Mask for Button 1
BUTTON5_MASK EQU 0x0020          ; Mask for Button 2


	EXPORT __main
	EXPORT SystemInit
		
	
	AREA led_button, CODE, READONLY
		
SystemInit	PROC
    ; Initialize system peripherals and GPIO
    LDR R6, = RCC_APB2ENR  ; Load peripheral clock enable register
	LDR R5, [R6]           ; Read its content
	ORR R5, #RCC_APB2ENR_GPIODEN_mask ; Set Bit 5 to enable GPIOD clock
	STR R5, [R6]           ; Store the result back in RCC_APB2ENR

	; Configure PORTD Pin0 and Pin1 as output
	LDR R6, = GPIOD_CRL    ; Load GPIOD control register low (Pin0 and Pin1 are in CRL register)
	LDR R5, = 0x00000022   ; Set CNF bits to 00 and MODE bits to 10 (CNF[1:0]_MODE[1:0])
	; Direct value assignment is not recommended
	STR R5, [R6]           ; Store the result in GPIOD_CRL
	
	
	; Configure pin4 and pin5 as input
	LDR R6, =GPIOD_BASE
	LDR R1, [R6, #GPIOx_CRL]  ; Load GPIOD->CRL
	BIC R1, R1, #(0xf<<4*4) ; Clear bits for CNF0[1:0] MODE0[1:0]
	ORR R1, R1, #(0x01<<4*4) ; CNFy[1:0]: 10  MODE0[1:0]: 00
	STR R1, [R6, #GPIOx_CRL]
	
	LDR R1, [R6, #GPIOx_CRL] ; Load GPIOD->CRL
	BIC R1, R1, #(0xf<<4*5) ; Clear bits for CNF0[1:0] MODE0[1:0]
	ORR R1, R1, #(0x01<<4*5) ; CNFy[1:0]: 10  MODE0[1:0]: 00
	STR R1, [R6, #GPIOx_CRL]
	BX LR
	ENDP
		
; Delay subroutine		
delay PROC
	LDR r0, =delayCount  ; Load delayCount value
delay_loop
	sub r0,#1
	cmp r0,#0
	bne delay_loop	; Wait until 0
	BX LR
	ENDP	
		

; LED toggle subroutine
toggle PROC
	LDR R6, = GPIOD_ODR ; Load GPIOD output data address
	LDR R1, [R6]		;Load current GPIOD output data
	MOV R2, #3 			;Bit mask to toggle pin0 and pin1
	EOR	R1,R1,R2		; Toggle the bits
	STR R1, [R6]        ; Store the result in GPIOD_ODR
	BL  delay			; Delay for button bouncing
	B loop
	ENDP




; LED blink subroutine
blink PROC
firstRound 
	LDR R6, = GPIOD_ODR ; Load GPIOD output data address
	LDR R1, [R6]		;Load current GPIOD output data
	MOV R2, #3 			;Bit mask to toggle pin0 and pin1
	EOR	R1,R1,R2		; Toggle the bits
	STR R1, [R6]        ; Store the result in GPIOD_ODR
	BL  delay
secondRound	
	LDR R6, = GPIOD_ODR ; Load GPIOD output data address
	LDR R1, [R6]		;Load current GPIOD output data
	MOV R2, #3 			;Bit mask to toggle pin0 and pin1
	EOR	R1,R1,R2		; Toggle the bits
	STR R1, [R6]        ; Store the result in GPIOD_ODR
	BL  delay
	ENDP



; Start of the main program
__main PROC

loop
	; Read the input value from GPIO port D's input register (GPIOD_IDR) into R1
    LDR R5, =GPIOD_IDR
	LDR R1, [R5]
	
	; Check if Button 4
button4	
    TST R1, #BUTTON4_MASK
    BEQ button5
	BL blink
	B loop
	
	; Check if Button 5
button5	
	TST R1, #BUTTON5_MASK
    BEQ loop
    BL toggle
	B loop
	ENDP
; End of the main program
	END
