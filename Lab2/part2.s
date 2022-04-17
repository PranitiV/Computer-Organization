/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R3, #TEST_NUM   // R3 points to start of list 

          MOV     R0, #0          // R0 will hold the result
		  MOV     R5, #0          //R5 gets initialized to 0
		          
LOOP1:    LDR R1, [R3], #4        //load word into R1 and increment address to next word
          CMP R1, #0              //check if words are finished 
		  BEQ END             
		  BL ONES                 //count the ones in the word 
		  CMP R5, R0              //check if number of ones in new word is greater than previous
		  BGT CONTINUE            //move the largest number of ones into r5 and continue loop
          MOV R5, R0
CONTINUE: B LOOP1                 //continue loop 
		  
		
ONES:     MOV     R0, #0          //reset r0 to 0   
LOOP2:    CMP     R1, #0          // loop until the data contains no more 1's
          BEQ     RETURN             
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      
          ADD     R0, #1          // count the string length so far
          B       LOOP2            
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
          .end                            
