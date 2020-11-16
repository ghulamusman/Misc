       IDENTIFICATION DIVISION.
       PROGRAM-ID. SQRT.
       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.

           SELECT STANDARD-OUTPUT ASSIGN TO DISPLAY.
       DATA DIVISION.
       FILE SECTION.
       WORKING-STORAGE SECTION.
       77 DIFF PICTURE V9(5).
       77 Z    PICTURE 9(11)V9(6).
       77 USIN PICTURE S9(11)V9(6).
       77 K    PICTURE S9999.
       77 E    PICTURE S9999.
       77 X    PICTURE 9(11)V9(6).
       77 Y    PICTURE 9(11)V9(6).
       77 TEMP PICTURE S9(11)V9(6).
       77 CHOICE PICTURE XX.
       
       
        PROCEDURE DIVISION.
           Move " " to CHOICE.
           DISPLAY X'0A''SQUARE ROOT APPROXIMATIONS'.
           DISPLAY "---------------------------------------------------------".
      
       
       INPU.       *> Getting user input and checking for correctness
               COMPUTE E = 0.
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
                   UNTIL K > 1000 OR E = 5000.
    
                   *> User choice to input a new value or exit program
            IF E = 5000 THEN
               DISPLAY "To Exit Enter ""Y"" else any other key."
               ACCEPT CHOICE
            ELSE       *> incase too many iterations occur
               DISPLAY Z '  ATTEMPT ABORTED,TOO MANY ITERATIONS'
            END-IF.

            IF CHOICE = "Y"
                STOP RUN
            ELSE
               PERFORM INPU THRU FINISH
                   UNTIL K < ZERO
            END-IF.

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
                COMPUTE E = 5000

                CONTINUE 
            END-IF.
       E2. 
       
           
           

           MOVE Y TO X.
       FINISH.

           
       STOP RUN.
       