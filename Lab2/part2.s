.text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R1, #TEST_NUM   // load the data word ...
		  LDR     R1, [R1]        // into R1

          MOV     R0, #0          // R0 will hold the result
LOAD:	  MOV 	  R3, #TEST_NUM
		  MOV 	  R5, #0
		  
ONES:     CMP     R1, #0          // loop until the data contains no more 1's
          BEQ     COMPARE             
          LSR     R2, R1, #1      // perform SHIFT, followed by AND
          AND     R1, R1, R2      
          ADD     R0, #1          // count the string length so far
          B       ONES            

COMPARE: CMP R0, R5
		  BLT CONTINUE
		  MOV R5, R0
		  
CONTINUE: ADD R3, #4
		   LDR R1, [R3]
		   MOV R0, #0
		   CMP R1, #0
		   BEQ END 
		   B ONES

END:      B       END             

TEST_NUM: .word   0x103fe00f , 0x103ff00f, 0

          .end
