.global _start
.equ KEY_BASE, 0xff200050
.equ HEX_BASE, 0xff200020
.equ TIMER, 0xFFFEC600
_start:
			LDR R8, =KEY_BASE
			LDR R4, =HEX_BASE
			LDR R10, =TIMER
			MOV R6, #0        //counter value for DD
			MOV R11, #0       //counter value for SS
	
POLL:  	    LDR R2, [R8, #0xC]   //load edge cap reg value 
			CMP R2, #0 
			BEQ POLL	          //keep polling until key is pressed 
            STR R2, [R8, #0xC]	  //reset edge cap reg 

RESET:  	MOV R6, #0             //when timer reaches 59:99 reset to 0
			MOV R11, #0
COUNT:	    CMP R11, #60      
			BEQ RESET 
			CMP R6, #100          //when DD value reaches 99 reset to 0 and increment SS
			BEQ CHANGE
		
DO_DELAY: 	LDR R7, =2000000 // 200MHz*0.01 value to count down 
			STR R7, [R10]      //load value into load reg of timer 
			BL CHECK           //check if key was pressed 
			MOV R7, #3          //set a and e bit of control reg to 1 
			STR R7, [R10, #0x8]  
			
LOOP: 	    LDR R7, [R10, #0xc]  //load the value of f bit in interrupt status reg 
			CMP R7, #0
			BEQ LOOP             //wait until f bit is 1 
			STR R7, [R10, #0xc]   //reset f bit 
			
DISPLAY:    BL CHECK            //check if key was pressed 
			MOV R0, R6          //move DD counter value into r0
			BL DIVIDE            
			MOV R9, R1          // save the tens digit
            BL SEG7_CODE       // gets bit code of ones digit in r0
			MOV R5, R0          // save bit code of ones digit into bits 0-7 of r5 
            MOV R0, R9          // retrieve the tens digit, get bit code
            BL  SEG7_CODE       
            LSL R0, #8          //shift tens digit by 8 bits to bits 8-15 of r0 
            ORR R5, R0          // get the tens digit into bits 8-15 of r5  
			
			MOV R0, R11         //move SS counter value into r0
			BL DIVIDE           
			MOV R9, R1          // save the tens digit
            BL SEG7_CODE       // gets bit code of ones digit in r0
			LSL R0, #16        // get ones digit into bits 16-23 of r5
			ORR R5, R0         
            MOV R0, R9          // retrieve the tens digit, get bit code
            BL  SEG7_CODE      
			LSL R0, #24         //get tens digit into bits 24-31 of r5
			ORR R5, R0
			STR R5, [R4]        //display the digits to hex  
			
CONTINUE:   ADD R6, #1          //increment the DD value 
			B COUNT             //continue to count 
			
CHECK:     	LDR R3, [R8, #0xC]   //load val of edge cap register 
			CMP R3, #0        
			BNE WAIT             //wait for second press if key was pressed 
			MOV PC, LR           //return if key was not pressed 
		
WAIT:       STR R3, [R8, #0xC]   //reset edge cap register 
			MOV R2, #0b010       //stop the timer by storing 0 into e bit of control reg 
			STR R2, [R10, #0x8]

SECOND:     LDR R3, [R8, #0xC]   //reload edge cap reg val 
			CMP R3, #0   
			BEQ SECOND           //wait until key is pressed again 
			STR R3, [R8, #0xC]   //reset edge cap reg 
			MOV R2, #0b011       //continue timer by storing 1 into e bit of control reg 
			STR R2, [R10, #0x8]  
			MOV PC, LR           //return and continue counting 
			
CHANGE:     MOV R6, #0           //when the DD value reaches 99 reset to 0 and increment SS val
			ADD R11, #1
			B  DO_DELAY          //continue counting 

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

	   
	
	