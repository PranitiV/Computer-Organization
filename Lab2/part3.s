/* Program that counts consecutive 1's, 0's and alternating 0's and 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R3, #TEST_NUM   // R3 points to start of list 
          MOV     R0, #0          // R0 will hold the result
		  MOV     R5, #0          //R5 gets initialized to 0
		  MOV     R6, #0          //R6 gets initialized to 0
		  MOV     R7, #0          //R7 gets initialized to 0
		  MOV     R4, #SEQUENCE   //R4 points to the alternating sequence
		  LDR     R4, [R4]        //R4 gets the alt sequence
		  MOV     sp, #0x20000    //initialize stack pointer
		  
 
LOOP1:    LDR R1, [R3]            //load word into R1
          CMP R1, #0              //check if words are finished 
		  BEQ END             
		  BL ONES  		          //moves to ones subroutine

ONESDONE: CMP R5, R0              //check if number of ones in new word is greater than previous
		  BGT CONT1               //move the largest number of ones into r5 
		  MOV R5, R0
CONT1:    LDR R1, [R3]            //reloads the word into R1
		  BL ZEROS                 //moves to zero subroutine  

ZEROSDONE: CMP R6, R0             //check if number of zeros in new word is greater than previous
		   BGT CONT2              //move the largest number of ones into r6 
		   MOV R6, R0
CONT2:     LDR R1, [R3]            //reloads the word into R1
		   BL ALTERNATE            //moves to alternate subroutine
		  
ALTDONE:  CMP R7, R0              //check if alternating numbers in new word is greater than previous
		  BGT CONT3               //move the largest number of alternating numbers into r7
		  MOV R7, R0 
CONT3:    ADD R3, #4              //increment R3 to point to next word
		  B LOOP1

ALTERNATE:  MOV R0, #0
			MOV R8, #0
			EOR R1, R1, R4        //XOR of the word and the alternating sequence into R1
			PUSH {LR}               //push current value in LR to stack
			BL ONES               //count longest number of ones in R1
			MOV R8, R0            //store the longest value in R8
			LDR R1, [R3]          //reload the word into R1
			MVN R1, R1            //invert R1 
			EOR R1, R1, R4        //XOR of the inverted word and alternating sequence into R1
			BL ONES               //count the longest number of ones in R1
			POP {LR}
			CMP R0, R8            //check if R0 is greater than R8
			BGT CONTINUE
			MOV R0, R8            //move R8 into R0 if R0<R8
CONTINUE:   MOV PC, LR             //return from subroutine
		  
		  
ZEROS:    MOV R0, #0              //reset R0 to 0
		  MVN R1, R1              //inverts R1
ZEROSLOOP:CMP R1, #0              //loop until the data 
		  BEQ     RETURN             
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      
          ADD     R0, #1          // count the string length so far
          B       ZEROSLOOP                  

		
		
ONES:     MOV     R0, #0          //reset R0 to 0   
ONESLOOP: CMP     R1, #0          // loop until the data contains no more 1's
          BEQ     RETURN             
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      
          ADD     R0, #1          // count the string length so far
          B       ONESLOOP            
RETURN:   MOV PC, LR             // exit subroutine 


END:      B       END             

TEST_NUM: .word   0x103fe00f  
		  .word   0x104fe00f
		  .word   0x105fe00f
		  .word   0x106fe00f
		  .word   0x107fe00f
		  .word   0x203fe00f
		  .word   0x213ef004
		  .word   0x10230420
		  .word   0x12341022
		  .word   0
		  
SEQUENCE: .word 0x55555555
          .end                            
