/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R3, #TEST_NUM   
          MOV     R0,  #0
		  MOV     R5,  #0
		  MOV     R6,  #0
		  MOV     R7,  #0
		  MOV     R4, #SEQUENCE
		  LDR	  R4, [R4]
		  MOV 	  sp, #0x20000
		  
LOOP1:   LDR     R1, [R3]
		  CMP 	  R1, #0
		  BEQ     END 
		  BL 	  ONES 
		  
ONESDONE:CMP	 R5, R0
		  BGT     CONT1
		  MOV     R5, R0
CONT1:   LDR     R1, [R3]
		  BL 	  ZEROES 
		  
ZEROESDONE:CMP	 R6, R0
		  BGT     CONT2
		  MOV     R6, R0
CONT2:   LDR     R1, [R3]
		  BL 	  ALTERNATE

ALTDONE: CMP	 R7, R0
		  BGT     CONT3
		  MOV     R7, R0
CONT3:   ADD	 R3, #4
		  B 	  LOOP1 
		  
ALTERNATE: MOV R0, #0
			MOV R8, #0 
			EOR R1, R1, R4
			PUSH {LR}
			BL ONES 
			MOV R8, R0
			LDR R1, [R3]
			MVN R1, R1
			EOR R1, R1, R4
			BL ONES
			POP {LR}
			CMP R0, R8
			BGT CONTINUE
			MOV R0, R8
CONTINUE:  MOV PC, LR

ZEROES: MOV R0, #0
		 MVN R1, R1
ZEROESLOOP: CMP R1,#0
			 BEQ RETURN 
			 LSR R2, R1, #1
			 AND R1, R1, R2
			 ADD R0, #1
			 B ZEROESLOOP
			 
ONES: MOV R0, #0
ONESLOOP:   CMP R1,#0
			 BEQ RETURN 
			 LSR R2, R1, #1
			 AND R1, R1, R2
			 ADD R0, #1
			 B ONESLOOP	
RETURN: 	MOV PC, LR

END:      B       END             

TEST_NUM: .word   0x103fe00f, 0x104fe00f, 0x105fe00f, 0x106fe00f,0x107fe00f,
		    .word  0x108fe00f, 0

SEQUENCE: .word 0x55555555
          .end                            
