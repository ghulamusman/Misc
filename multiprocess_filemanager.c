
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#define _XOPEN_SOURCE 700

int main(void) {

    int  comm_sz;               /* Number of processes    */
    int  my_rank;               /* My process rank        */
    char dirName[50];
    char *cmdLine = NULL;
    size_t len = 1000;
    struct stat st = {0};
    int numDirs=0;
    char *token[256];
    const char delim[6] = " \n";
    int j=0;
    int putNum = 0;



    /* Start up MPI */
    MPI_Init(NULL, NULL); 

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    if (my_rank == 0){  // THE MAIN PROCESS
        
        for(int i = 1; i < comm_sz; i++){
            sprintf(dirName, "p%d", i);
            if (stat(dirName, &st) == -1) {
                numDirs = numDirs+1;
                //printf("p%d ", i);
                mkdir(dirName, 0700);
            }
        }
/*
        int sizeIncrement = 1;
        char** fileStructure= malloc(sizeof(char*)*comm_sz);
        for(int i =0;i<comm_sz-3;i++){
            fileStructure[i] = malloc(1);
        }
*/        
        while(1){ //SHELL PROCESS
            input:
            //get command line input
            getline(&cmdLine, &len, stdin);
            //tokenize input
            j=0;
            token[j] = NULL;
            token[j] = strtok(cmdLine,delim);
            while( token[j] != NULL ) {
                j++;
                token[j] = strtok(NULL, delim);
            }
            
            
            if(strcmp(token[0],"exit")==0){
                for (int k = 1 ; k<comm_sz;k++){
                    MPI_Send("exit\0", 5, MPI_CHAR, k, 0, MPI_COMM_WORLD); 
                }
                break;
            }else 
            if(strcmp(token[0],"put")==0){
                
                if (putNum == comm_sz-1) putNum = 0;

                FILE *fileptr;
                char *buffer;
                long filelen;
                int savedNum;
                //printf("%s", token[1]);
                fileptr = fopen(token[1], "rb");  // Open the file in binary mode
                if(fileptr == NULL){
                    printf("fileptr failed\n");
                    goto input;
                }
                fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
                filelen = ftell(fileptr);             // Get the current byte offset in the file
                rewind(fileptr);

                buffer =  malloc(sizeof(char)*filelen);
                fread(buffer, filelen, 1, fileptr);
                fclose(fileptr); 
                
                if (token[2]==NULL){
                    putNum++;
                }else if (strcmp(token[2],"")!=0){
                    putNum = atoi(token[2]);
                } else {
                    putNum++;
                }              
                
                MPI_Send("put \0", 5, MPI_CHAR, putNum, 0, MPI_COMM_WORLD); 
                
                int value = filelen;
                int fileNameLen = strlen(token[1]);
                //sends name
                MPI_Send(&fileNameLen, 1, MPI_INT, putNum, 0, MPI_COMM_WORLD); 
                MPI_Send(token[1], fileNameLen+1, MPI_BYTE, putNum, 0, MPI_COMM_WORLD); 
                //sends content
                MPI_Send(&value, 1, MPI_INT, putNum, 0, MPI_COMM_WORLD); 
                MPI_Send(buffer, value+1, MPI_BYTE, putNum, 0, MPI_COMM_WORLD); 
    
            }else
            if(strcmp(token[0],"get")==0){
                int recieveCheck;
                for (int k = 1 ; k<comm_sz;k++){
                    MPI_Send("get \0", 5, MPI_CHAR, k, 0, MPI_COMM_WORLD);
                    int fileNameLen = strlen(token[1]);
                    MPI_Send(&fileNameLen, 1, MPI_INT, k, 0, MPI_COMM_WORLD); 
                    MPI_Send(token[1], fileNameLen+1, MPI_BYTE, k, 0, MPI_COMM_WORLD);  
                } 
                for (int k = 1 ; k<comm_sz;k++){
                    MPI_Recv(&recieveCheck, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);       
                    if(recieveCheck != -1){
                        char* filecontent;
                        
                        char* newFilePath;
                        int contentSize;
                        FILE* fileptr;
                        printf("Process %d returned %s\n",recieveCheck,token[1]);
                        MPI_Recv(&contentSize, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
                        filecontent = malloc(sizeof(char)*contentSize+1);
                        MPI_Recv(filecontent, contentSize+1, MPI_BYTE, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

                        fileptr = fopen(token[1], "wb+");  // Open the file in binary mode
                        fwrite(filecontent , 1 , strlen(filecontent) , fileptr ); 
                        
                        fclose(fileptr);
                        
                        free(filecontent);
                    }    
                } 
                

            }else
            if(strcmp(token[0],"lls")==0){
                DIR *dr;
                
                struct dirent *de;     
                dr = opendir ("./");

                while ((de = readdir(dr)) != NULL) {
                    if(strcmp(de->d_name,"..")==0 ||strcmp(de->d_name,".")==0){
                        continue;
                    }
                    printf("%s\n", de->d_name); 
                }
                closedir(dr);

            }else
            if(strcmp(token[0],"lrm")==0){
                remove(token[1]);
            }else
            if(strcmp(token[0],"ls")==0){
                for (int k = 1 ; k<comm_sz;k++){
                    MPI_Send("ls  \0", 5, MPI_CHAR, k, 0, MPI_COMM_WORLD); 
                }
                for (int k = 1 ; k<comm_sz;k++){
                    char* filecontent;
                    int contentSize;
                    
                    MPI_Recv(&contentSize, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
                    filecontent = malloc(sizeof(char)*contentSize+1);
                    MPI_Recv(filecontent, contentSize+1, MPI_BYTE, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

                    printf("%s\n", filecontent);
                    
                    free(filecontent);
                        
                } 
            }
            else{
                printf("Not a valid Command\n");
            }



        }
    



    }else{     // THE OTHER PROCESSes
        int dirSize = 100;
        char *myDirectory= malloc(sizeof(char)*dirSize);        
        int contentSize;

        
        while(1){
            char cmd[5];
            MPI_Recv(cmd, 5, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(strcmp(cmd,"exit")==0){
                break;
            }else
            if(strcmp(cmd,"put ")==0){

                
                char* filecontent;
                char* filename;
                char* newFilePath;
                FILE* fileptr;
                
                //recv name     
                MPI_Recv(&contentSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
                filename = malloc(sizeof(char)*contentSize+1);
                MPI_Recv(filename, contentSize+1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
                //printf("%d %s\n", contentSize,filename);
               
                //recv content
                MPI_Recv(&contentSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
                filecontent = malloc(sizeof(char)*contentSize+1);
                MPI_Recv(filecontent, contentSize+1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
                //printf("%d %s", contentSize,filecontent);
                
                //create file and populate
                
                newFilePath  = malloc(sizeof(char)*100);
                sprintf(newFilePath,"p%d/%s",my_rank,filename);
                
                    
                printf("%s put.\n",newFilePath);
                fileptr = fopen(newFilePath, "wb+");  // Open the file in binary mode
                fwrite(filecontent , 1 , strlen(filecontent) , fileptr ); 
                
                
                fclose(fileptr);
                free(filename);
                free(filecontent);
                free(newFilePath);
            }else
            if(strcmp(cmd,"get ")==0){
                char* filecontent;
                char* filename;
                FILE *fileptr;
                char *buffer;
                long filelen;
                int savedNum;
                int found = -1;
                //recv name     
                MPI_Recv(&contentSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
                filename = malloc(sizeof(char)*contentSize+1);
                MPI_Recv(filename, contentSize+1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
                DIR *dr;
                

                myDirectory=realloc(myDirectory,dirSize);
                sprintf(myDirectory,"p%d/",my_rank);
                
                struct dirent *de;     
                dr = opendir (myDirectory);
                 
                while ((de = readdir(dr)) != NULL) {
                    if(strcmp(de->d_name,"..")==0 ||strcmp(de->d_name,".")==0){
                        continue;
                    }
                    if(strcmp(filename,de->d_name)==0){
                        found = my_rank;
                        
                        //printf("%s", token[1]);
                        strcat(myDirectory,filename);
                        fileptr = fopen(myDirectory, "rb");  // Open the file in binary mode
                        if(fileptr == NULL){
                            printf("fileptr failed\n");
                            goto input;
                        }
                        fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
                        filelen = ftell(fileptr);             // Get the current byte offset in the file
                        rewind(fileptr);

                        buffer =  malloc(sizeof(char)*filelen);
                        fread(buffer, filelen, 1, fileptr);
                        fclose(fileptr);
                        break;
                    }
                }

                if(found == my_rank){
                    MPI_Send(&found, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                    int value = filelen; 
                    MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
                    MPI_Send(buffer, value+1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
                }else{
                    MPI_Send(&found, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 

                }
                
            
                closedir(dr);
            }else
            if(strcmp(cmd,"ls  ")==0){
                //printf("%d my files %s\n", my_rank, myFiles);
                DIR *dr;
                dirSize = dirSize + 100;

                myDirectory=realloc(myDirectory,dirSize);
                sprintf(myDirectory,"p%d/",my_rank);
                
                struct dirent *de;     
                dr = opendir (myDirectory);
                sprintf(myDirectory,"p%d/:\n", my_rank);
                 
                while ((de = readdir(dr)) != NULL) {
                    if(strcmp(de->d_name,"..")==0 ||strcmp(de->d_name,".")==0){
                        continue;
                    }
                    strcat(myDirectory, de->d_name); 
                    strcat(myDirectory, ", \n");
                }
            
                closedir(dr);
                int value = strlen(myDirectory); 
                MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
                MPI_Send(myDirectory, value+1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
                
            }
            
        }

    }
    /* Shut down MPI */
    MPI_Finalize(); 

    return 0;
    /* main */

}