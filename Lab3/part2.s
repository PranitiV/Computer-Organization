.global _start
.equ KEY_BASE, 0xff200050
.equ HEX_BASE, 0xff200020
_start:
			LDR R8, =KEY_BASE
			LDR R4, =HEX_BASE
			MOV R6, #0        //counter value 
	
POLL:  	    LDR R2, [R8, #0xC]   //load address of edge cap register into r2
			CMP R2, #0           //if edge cap is at 0 continue poll
			BEQ POLL	
            STR R2, [R8, #0xC]	 //reset edge cap reg when pressed 

RESET:  	MOV R6, #0           //reset r6 to 0 if it is 99
COUNT:	    CMP R6, #100
			BEQ RESET 
		
DO_DELAY: 	LDR R7, =200000000 // for CPUlator use =500000
			BL CHECK           //check if any key was pressed 
SUB_LOOP: 	SUBS R7, R7, #1    //begin counter 
		  	BNE SUB_LOOP
			
DISPLAY:    BL CHECK           //check if any key was pressed 
			MOV R0, R6         //move counter val into r0
			BL DIVIDE          //divide to get decimal number
			MOV R9, R1          // save the tens digit
            BL SEG7_CODE       // gets bit code of ones digit in r0
			MOV R5, R0          // save bit code of ones digit 
            MOV R0, R9          // retrieve the tens digit, get bit code
            BL  SEG7_CODE       
            LSL R0, #8          //shift tens digit by 8 bits to bits 8-15 of r0 (hex1)
            ORR R5, R0          // get the tens digit into bits 8-15 of r5                       
			STR R5, [R4]        //store bit code into hex address to display
			
CONTINUE:   ADD R6, #1         //increment the counter 
			B COUNT            //branch back to count
			
CHECK:     	LDR R3, [R8, #0xC]   // get the address of edge cap reg
			CMP R3, #0           // if key was pressed branch to wait
			BNE WAIT 
			MOV PC, LR           // return if key was not pressed 
		
WAIT:       STR R3, [R8, #0xC]   // reset edge cap reg

SECOND:     LDR R3, [R8, #0xC]    // reload edge cap register 
			CMP R3, #0 
			BEQ SECOND            // wait for second press to continue count
			STR R3, [R8, #0xC]    // reset edge cap reg
			MOV PC, LR            // return and continue count
			

/* Subroutine to perform the integer division R0 / 10.
 * Returns: quotient in R1, and remainder in R0 */
DIVIDE:     MOV    R2, #0
CONT:       CMP    R0, #10
            BLT    DIV_END
            SUB    R0, #10
            ADD    R2, #1
            B      CONT
DIV_END:    MOV    R1, R2     // quotient in R1 (remainder in R0)
            MOV    PC, LR

/* Subroutine to convert the digits from 0 to 9 to be shown on a HEX display.
 *    Parameters: R0 = the decimal value of the digit to be displayed
 *    Returns: R0 = bit patterm to be written to the HEX display
 */

SEG7_CODE:  MOV     R1, #BIT_CODES  
            ADD     R1, R0         // index into the BIT_CODES "array"
            LDRB    R0, [R1]       // load the bit pattern (to be returned)
            MOV     PC, LR              

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment

	   
	