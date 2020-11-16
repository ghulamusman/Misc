//GHULAM USMAN 0993650 A3 OS3110

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]){

    int logicalAddress = 0;
    int physicalAddress = 0;
    int pgNum = 0;
    int offset = 0;
    char backingStoreBuff[256];
    int physicalMemory[256*256];
    int pgTable[256];
    int tlbTable[16][2]; // page num and frame num
    bool tlbHit = false;
    int tlbCount = -1;
    int theFrame = 0;
    int tlbPointer = 0;
    int storedValue = 0;
  

    if (argc != 2) {
        fprintf(stderr, "usage: %s missingsecondargument\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int addressCount=0;
    

    FILE* virtualAddress = fopen(argv[1], "r");
    FILE* backingStore = fopen("BACKING_STORE.bin", "rb");
   
    if(virtualAddress == NULL){     // IF ADDRESS FAILS TO OPEN
        
        fclose(backingStore);

        fprintf(stderr, "usage: %s %sfilemissingorIncorrect\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }


    if (backingStore == NULL){      // IF BACKING FAILS
        fprintf(stderr, "usage: %s BackingStorefilemissingorIncorrect\n", argv[0]);
        
        fclose(virtualAddress);
        exit(EXIT_FAILURE);        
    }
    FILE* addressOutput = fopen("outputAddress.txt", "w");

    if (addressOutput == NULL){ // IF OUTPUT FAILS
        fprintf(stderr, "usage: %s outputfilefailed\n", argv[0]);
        
        fclose(virtualAddress);
        fclose(backingStore);
        exit(EXIT_FAILURE);        
    }

    for(int i =0; i<256;i++){
        pgTable[i] = -1;
    }
    for(int i =0; i<16;i++){
        tlbTable[i][0] = -1;
    }

    while(fscanf(virtualAddress, "%d", &logicalAddress) == 1){
    //             fseek(backingStore, 0, SEEK_SET);
        tlbHit = false;
        addressCount++;

        pgNum = (logicalAddress & 0xFFFF)>>8;

        offset = logicalAddress & 0xFF;

        
        for(int k = 0;k<16;k++){
            if(tlbTable[k][0] == pgNum){
                physicalAddress = (tlbTable[k][1]*256)+offset;
               
                tlbCount++;
               //printf("%d %d %d ",tlbTable[k][0], tlbTable[k][1],physicalAddress );
               tlbHit = true;
            }
        }

    
        if(tlbHit == false){
            
            if(pgTable[pgNum] == -1){                   //page fault

                fseek(backingStore, pgNum*256, SEEK_SET);
                fread(backingStoreBuff, sizeof(char), 256, backingStore); //reading from backing table
                pgTable[pgNum] = theFrame;                 //populating pgtable

                for(int i = 0; i < 256; i++){
                    
                    physicalMemory[theFrame*256 + i] = backingStoreBuff[i];   //populating physical memory.
                // printf("%d ", physicalMemory[theFrame][i]);                     
                }
                theFrame++;         //incrementing the Frame

                if(tlbPointer == 16){
                    tlbPointer = 0;
                }                           //Beautifully done FIFO(personal opinion)
                tlbTable[tlbPointer][0] = pgNum;
                tlbTable[tlbPointer][1] = pgTable[pgNum];
                
                tlbPointer++;

            }                       

            physicalAddress =( pgTable[pgNum]*256 )+ offset;
            //printf(" %d\n",physicalAddress);
        }
        storedValue =  physicalMemory[physicalAddress];

    //    // printf("\nLogAdd: %d, pgNUM: %d, offset: %d, physicalAddress: %d, storedValue: %d\n", logicalAddress, pgNum, offset, physicalAddress, storedValue);
        fprintf(addressOutput, "Virtual address: %d Physical address: %d Value: %d \n", logicalAddress, physicalAddress, storedValue);

        // printf("\n");
    }
    fprintf(addressOutput, "Number of Addresses = %d\nNumber of Page Faults = %d\nPage Fault Rate = %0.3f\nTLB Hits = %d\nTLB Hit Rate = %0.3f", addressCount, theFrame, (float)theFrame/(float)addressCount, tlbCount, (float)tlbCount/(float)addressCount);



    fclose(addressOutput);
    fclose(virtualAddress);
    fclose(backingStore);
}