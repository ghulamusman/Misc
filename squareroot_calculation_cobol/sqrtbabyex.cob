       IDENTIFICATION DIVISION.
       PROGRAM-ID. MAIN.
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
       77 CHOICE PICTURE XX.
       77 X    PICTURE 9(11)V9(6).
       77 Y    PICTURE 9(11)V9(6).
       77 TEMP PICTURE 9(11)V9(6).
       
       PROCEDURE DIVISION.

            Move " " to CHOICE.
            DISPLAY X'0A''SQUARE ROOT APPROXIMATIONS'.
           
       S1.    

           CALL 'SQUAREROOT'.
           DISPLAY "---------------------------------------------------------"
            *>    user choice to run program again or exit
           DISPLAY "To Exit Program Enter Y Else Any Other Key.".
           ACCEPT CHOICE.
           
       S2.
           PERFORM S1 THRU S2    
               UNTIL CHOICE = "Y".
       
       STOP RUN.
       