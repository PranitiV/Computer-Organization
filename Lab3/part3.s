.global _start
.equ KEY_BASE, 0xff200050
.equ HEX_BASE, 0xff200020
.equ TIMER, 0xFFFEC600
_start:
			LDR R8, =KEY_BASE
			LDR R4, =HEX_BASE
			LDR R10, =TIMER
			MOV R6, #0        //counter value 
	
POLL:  	    LDR R2, [R8, #0xC]   //load edge cap reg address into r2
			CMP R2, #0 
			BEQ POLL	         //keep polling until a key is pressed
            STR R2, [R8, #0xC]	 //reset edge cap reg after key is pressed

RESET:  	MOV R6, #0          //reset counter val to 0 if it exceeds 99
COUNT:	    CMP R6, #100
			BEQ RESET 
		
DO_DELAY: 	BL CHECK            //check if key has been pressed 
			LDR R7, =50000000  //200MHz*0.25  count down value into r7
			STR R7, [R10]      //store count down value into load reg in timer
			BL CHECK           //check if key has been pressed 
			MOV R7, #3         //set a bit and e bit in control reg to 1
			STR R7, [R10, #0x8]     
	
LOOP: 	    LDR R7, [R10, #0xc]   //get address of interrupt status reg
			CMP R7, #0
			BEQ LOOP              //continue looping until f bit is 1
			STR R7, [R10, #0xc]   //reset f bit 
			
			
DISPLAY:    BL CHECK            //check if key was pressed 
			MOV R0, R6          //move counter val into r0 for division
			BL DIVIDE 
			MOV R9, R1          // save the tens digit
            BL SEG7_CODE       // gets bit code of ones digit in r0
			MOV R5, R0          // save bit code of ones digit 
            MOV R0, R9          // retrieve the tens digit, get bit code
            BL  SEG7_CODE       
            LSL R0, #8          //shift tens digit by 8 bits 
            ORR R5, R0          // get the tens digit into bits 8-15 of r5                      
			STR R5, [R4]        //display bit code to hex  
			
CONTINUE:   ADD R6, #1          // increment counter val
			B COUNT             // branch to count 
			
CHECK:     	LDR R3, [R8, #0xC]   //load edge cap reg address
			CMP R3, #0           //check if key was pressed 
			BNE WAIT 
			MOV PC, LR           //if key was not pressed return
		
WAIT:       STR R3, [R8, #0xC]   //reset the edge cap reg
			MOV R2, #0b010       //stop the timer, store 0 into enable bit
			STR R2, [R10, #0x8]  

SECOND:     LDR R3, [R8, #0xC]  //reload edge cap register address
			CMP R3, #0          //wait for key to be pressed again 
			BEQ SECOND 
			STR R3, [R8, #0xC]   //reset edge cap reg
			MOV R2, #0b011       //continue the time, store 1 into enable bit
			STR R2, [R10, #0x8]   
			MOV PC, LR           //return to continue counting 
			

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

	   
	