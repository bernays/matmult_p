#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include <sys/shm.h>
#define SHM_MODE 0600 /* user read/write */

/*Function to read matrix from stdin to matrixes and output matrix size */
void readmats(int inputmatrix1[20][20], int inputmatrix2[20][20], int* rows, int *columns, int *rows2, int *columns2 );


int main()
{
    int i,j;
    char buf0[20];
    char buf2[20];
    char buf3[20];
    char buf1[20];
    char buf4[20];
    char buf5[80];
    char buf6[80];
    
    char *bufargs[8] = {buf0,buf1,buf2,buf3,buf4,buf5,buf6,(char*)0};/* arrays which will store the arguments that will be passed to ./multiply*/
                                                                    /*Requires the NULL value at the end!*/
    int *matrix;
    int id_shmem, counter =0, mypid =0,pid, status;
    int inputmatrix1[20][20];
    int inputmatrix2[20][20];
    int rows; // The number of rows of the 2D array
    int columns=0;// The number of columns of the 2D array
    int rows2, columns2; //Size of 2nd 2D array
    readmats(inputmatrix1, inputmatrix2, &rows, &columns, &rows2, &columns2);

    //Allocates correct amount of memory to hold the final matrix
    if((id_shmem = shmget(IPC_PRIVATE, sizeof(int)*rows*columns2, IPC_CREAT|0666)) < 0)
        err_sys("shmget error");

    if((matrix = (int *)shmat(id_shmem, 0, 0)) == NULL)
        err_sys("shmat error");
    int row, column;
    for (row = 0; row < rows; row++){
        for (column = 0; column < columns2; column++){
            if ((mypid = fork()) < 0) {
                err_sys("fork error");
            } else if (mypid == 0) {    /*Child*/
                snprintf(bufargs[2],20,"%d",columns2);  /*maximum number of columns in final matrix*/
                snprintf(bufargs[1],20,"%d",row);       /*Row number where result will be place*/
                snprintf(bufargs[3],20,"%d",column);    /*Column number where result will be placed*/
                snprintf(bufargs[0],20,"%d",id_shmem);  /*ID of shared memory object*/
                int ptr = 0;
                for( int i = 0; i <  columns; i++)
                {
                    ptr += snprintf(bufargs[4] + ptr,sizeof(bufargs[4]),"%d ", inputmatrix1[row][i]); //Creation of string that will hold the row of the first matrix
                    
                    
                }
                ptr = 0;
                for( int i = 0; i <  columns; i++)
                {
                    ptr += snprintf(bufargs[5] + ptr, sizeof(bufargs[5]), "%d ", inputmatrix2[i][column]); //Creation of string that will hold the column of the second matrix
                }
                counter++;
                
                if(execvp("./multiply",bufargs) ) {
                    
                    perror("Cannot 'exec', make sure correct pathname for multiply is specified\n");
                    exit(1);
                }
                else{
                    
                }
                
            }
            
        }
    }
    /*Parent*/
    if(mypid >0) {
        
        while ((pid = wait(&status)) > 0)
        {
            
        }
        //printf("All done! %d\n",counter);
        for (i = 0; i < rows; i++) {
            for (j = 0; j < columns2; j++) {
                if(j == columns2-1){
                    printf("%d", matrix[i*columns2 + j]);
                    fflush(stdout);

                                }
                else{
                    printf("%d ", matrix[i*columns2 + j]);
                    fflush(stdout);

                }
            }
            printf("\n");
            fflush(stdout);
        }
    }
    shmdt(matrix);
    shmctl(id_shmem, IPC_RMID, 0);
    
    return(4);
}

void readmats(int inputmatrix1[20][20], int inputmatrix2[20][20], int* rows, int *columns, int *rows2, int *columns2 ){
    char stdbuf[MAXLINE];
    int rowcounter =0, columncounter =0,rowcounter2 =-1, columncounter2 =0;
    /**Debugging: read from this file and then change where fgets reads from:**/
    // FILE * pFile;
    //pFile = fopen ("/Users/benjaminbernays/Dropbox/CS410/assignment2/assignment2/assignment2/matrix.c", "r");
    //pFile = fopen ("/home/course/cs410/bbernays/CS410/assignment2/matrix.c", "r");
    int matrix2 =0;
    int myint;
    int strpos =0;
    char *token;
    
    /*Beginning of loop to read in matrix from stdin*/
    while ((fgets(stdbuf, MAXLINE, stdin) != NULL)) {
        
        if(strcmp(stdbuf,"\n")==0){
            matrix2 =3;
            *rows = rowcounter; // The number of rows of the 2D array
            *columns = columncounter; // The number of columns of the 2D array
        }
        columncounter =0;
        strpos =0;
        columncounter2 = 0;
        token = strtok(stdbuf, " ");
        
        while((token != NULL)&(matrix2 == 0))
        {
            myint =atoi(token);
            inputmatrix1[rowcounter][columncounter]=myint;
            token = strtok(NULL, " ");
            columncounter++;
        }
        if(matrix2 == 0){
            if(rowcounter>0){
                if(columncounter != *columns){
                    fprintf(stderr, "Invalid number of columns in first matrix %d  %d %d\n", rowcounter,columncounter,*columns);
                    exit(-1);

                }
            }
            else{
                *columns = columncounter;
            }
        }

        if(matrix2 ==3)
            matrix2=1;
        while((token != NULL)&(matrix2 == 1))
        {
            myint =atoi(token);
            inputmatrix2[rowcounter2][columncounter2]=myint;
            token = strtok(NULL, " ");
                columncounter2++;
        }
        if(matrix2 == 1){
            if(rowcounter2>0){
                if(columncounter2 != *columns2){
                    fprintf(stderr, "Invalid number of columns in second matrix\n");
                    exit(-1);

                }
            }
            else{
                *columns2 = columncounter2;
            }
        }
        if(matrix2==0){
            rowcounter++;
        }
        if(matrix2==1){
            rowcounter2++;
        }
        
    }
    //http://sourcecookbook.com/en/recipes/67/how-to-use-multidimensional-arrays-in-shared-memory-with-ipc
    *rows2 = rowcounter2; // The number of rows of the 2D array
    if(*columns == *rows2){
        //printf("Valid matrix dimensions: Columns: %d. Rows %d\n", *columns, *rows2);
    }
    else{
        fprintf(stderr, "Invalid matrix dimensions: cannot multiply: Columns: %d. Rows %d\n", *columns, *rows2);
        exit(-1);

        fflush(stdout);
    }
}


char* itoa(int val, char* buf, int base){
    
    int i = 28;
    for(; val && i ; --i, val /= base){
        
        buf[i] = "0123456789abcdef"[val % base];
        
    }
    
    return &buf[i+1];
}
