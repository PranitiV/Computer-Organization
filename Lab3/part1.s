.global _start
.equ KEY_BASE, 0xff200050
.equ HEX_BASE, 0xff200020
_start:
			LDR R2, =KEY_BASE
			LDR R3, =HEX_BASE 
			MOV R6, #0      // hex display value
			MOV R7, #0      // set to 1 if key3 has been pressed 
			
POLL:       LDR R4, [R2]	// R4 gets value at keybase
			MOV R5, R4      // copy key value if it's been pressed 
			CMP R7, #1      // checks if key3 was pressed previously
			BNE CONT        // continue if wasn't pressed 
			MOV R5, #1      // change key value to key0 if key 3 was pressed previously 
CONT:	    CMP R4, #0      // check if key has been pressed 
			BEQ POLL        // loop if key has not been pressed 

RELEASE:    LDR R4, [R2]    // reload new key value
            CMP R4, #0      // check if key has been released 
			BNE RELEASE     
			
			CMP R5, #1      // check if key 0 was pressed 
			BEQ ZERO        // branch to display 0 
			
			CMP R5, #2      // check if key 1 was pressed 
			BEQ INCREMENT   // branch to increment display 
			
			CMP R5, #4      // check if key 2 was pressed
			BEQ DECREMENT   // branch to decrement display 
			
			CMP R5, #8      // check if key 3 was pressed 
			BEQ BLANK       // branch to blank the display 
			
ZERO:       MOV R6, #0 
			MOV R0, R6      // display 0 to hex0
			BL SEG7_CODE    // get bit pattern 
			STR R0, [R3]    // load the pattern into hex0
			B POLL          // poll again 

INCREMENT:  CMP R6, #9    
			BEQ POLL        // poll again if display value is 9
			ADD R6, #1      // increment R6
			MOV R0, R6      // display the incremented value
			BL SEG7_CODE    // get and load bit pattern into hex0
			STR R0, [R3]
			B POLL          // poll again

DECREMENT:  CMP R6, #0      
			BEQ POLL        // poll again if display value is 0
			SUB R6, #1      // decrement R6
			MOV R0, R6      // display the decremented value 
			BL SEG7_CODE    //get and load bit pattern into hex0
			STR R0, [R3] 
			B POLL          // poll again 
			
BLANK:      MOV R6, #0      // blank the display
			STR R6, [R3]
			MOV R7, #1     // display 0 if any key is pressed after this 
			B POLL
     

SEG7_CODE:  MOV     R1, #BIT_CODES  
            ADD     R1, R0         // index into the BIT_CODES "array"
            LDRB    R0, [R1]       // load the bit pattern (to be returned)
            MOV     PC, LR              

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
            .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
            .skip   2      // pad with 2 bytes to maintain word alignment
