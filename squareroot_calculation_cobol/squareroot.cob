       IDENTIFICATION DIVISION.
       PROGRAM-ID. SQUAREROOT.
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           
           SELECT STANDARD-OUTPUT ASSIGN TO DISPLAY.
       DATA DIVISION.
       FILE SECTION.
       
       FD STANDARD-OUTPUT.
           01 OUT-LINE  PICTURE X(80).
           01 TEST-LINE PICTURE S9(10).
       WORKING-STORAGE SECTION.
       77 DIFF PICTURE V9(5).
       77 Z    PICTURE 9(11)V9(6).
       77 USIN PICTURE S9(11)V9(6).
       77 K    PICTURE S9999.
       77 X    PICTURE 9(11)V9(6).
       77 Y    PICTURE 9(11)V9(6).
       77 TEMP PICTURE S9(11)V9(6).
       
       
       PROCEDURE DIVISION.
          
       
       INPU.       *> Getting user input and checking for correctness
           DISPLAY X'0A'" ENTER VALUE: ".
           ACCEPT USIN.
           COMPUTE DIFF = 0.001.
           IF USIN < ZERO THEN          
               DISPLAY USIN'        INVALID INPUT'
           END-IF.
       INPU-END.

           IF USIN < ZERO THEN
              PERFORM INPU THRU INPU-END 
                  UNTIL USIN > ZERO
           END-IF.

           MOVE USIN TO Z.

       B1. 
   
           
           COMPUTE X = Z / 2.
                              *> The actual algorithm to calculate the square root

           PERFORM S2 THRU E2 VARYING K FROM 1 BY 1
               UNTIL K > 1000.

                   *> incase of too many iterations.
           DISPLAY Z '  ATTEMPT ABORTED,TOO MANY ITERATIONS'.
           PERFORM INPU THRU S2
               UNTIL K < ZERO.

                *> Babylonian squareroot implementation

       S2. 
           COMPUTE Y ROUNDED = 0.5 * (X + Z / X).
           COMPUTE TEMP = X - Y.
           IF TEMP < ZERO THEN
               COMPUTE TEMP = - TEMP
           END-IF.
           IF TEMP / (Y + X) < DIFF THEN
               
               DISPLAY "--------------------------------------------"
               DISPLAY "        NUMBER               SQUARE ROOT"
               DISPLAY "--------------------     -------------------"
               DISPLAY Z "       " Y
               DISPLAY X'0A'
               MOVE Y TO X
               goback
           END-IF.
       E2. 
           MOVE Y TO X.
       FINISH.
       goback.
       