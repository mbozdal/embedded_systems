; Lab_1: Arithmetic and Logical Operations in ARM Assembly

; Description:
; This lab focuses on performing arithmetic and logical operations in ARM assembly using various instructions.
; Students are required to load their last 5 digits of student IDs into registers, perform different operations, and observe the results and status flags.

; Export the __main label as the entry point
	EXPORT __main
	AREA flagEffect, CODE, READONLY
; Start of the main program
__main
    ; Your assembly code goes here
    
    ; Task 1: Load last 5 digits of your IDs into R0 and R1
    ; Hint: Use MOV or LDR instructions to load the values into registers.
    
    ; Task 2: R2 = R0 + R1 
    ; Observation 2: Display the values of R0, R1, R2, and xPSR (CPSR).

    ; Task 3: R3 = R1 - R2 (use SUB)
    ; Observation 3: Display the values of R0, R1, R3, and xPSR (CPSR).
    
    ; Task 4: R4 = R1 - R2 (use SUBS)
    ; Observation 4: Display the values of R0, R1, R4, and xPSR (CPSR).
    
	; Task 5: Compare Observation 3 and Observation 4. 

    ; Task 6: R6 equals to a bitwise OR operation between R0 and R1
    ; Observation 6: Display the values of R0, R1, R6, and xPSR (CPSR).
    
    ; Task 7: R8 equals to arithmetic shift left R0 by 2 bits
    ; Observation 7: Display the values of R0, R8, and xPSR (CPSR).
	
	
; End of the main program

; End of program
	END
