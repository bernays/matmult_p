#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include <sys/shm.h>
//#define SHM_MODE 0600 /* user read/write */


int main(int argc, char *argv[]) {

    int row = atoi(argv[1]);
    int columns = atoi(argv[2]);
    int column = atoi(argv[3]);
    char* charmat1 = argv[4];
    char* charmat2 = argv[5];
    int mat1[25];
    int mat2[25];
    int i;
    char *token;
    int myint;
    /*printf("***%s***", argv[4]);
    printf("***%s***", argv[5]);
    printf("\n");
    fflush(stdout);*/

/*    for(i= 0; charmat1[i]!= '\0'; i++){
        mat1[i] =charmat1[i]-'0';
        //printf("Arg5:[%d,%d]: %d",row,column,mat1[i]);
        fflush(stdout);


    }
    //printf("\n");*/
    token = strtok(charmat1, " ");
    i =0;
    //printf("\nMat1: \n");

    while((token != NULL))
    {
        myint =atoi(token);
        mat1[i] =myint;
        //printf("[%d]",mat1[i]);
        //fflush(stdout);

        token = strtok(NULL, " ");
        i++;
        
    }
   // printf("\nMat2: \n");
    
    token = strtok(charmat2, " ");
    i =0;
    int counter = 0;
    while((token != NULL))
    {
        counter++;
        myint =atoi(token);
        mat2[i] =myint;
        //printf("[%d]",mat2[i]);
        //fflush(stdout);

        token = strtok(NULL, " ");
        i++;
        
    }

    
    

    int value = 0;
    for (i = 0; i<counter; i++) {
        //printf("\n[%d]x[%d]\n",mat2[i],mat1[i]);
        value = value + mat2[i]*mat1[i];
        //printf("\nValue[%d,%d]: %d\n",row,column,value);

    }
    //printf("\nValue[%d,%d]: %d\n",row,column,value);
    int id_shmem = atoi(argv[0]);
    int *matrix;
    if((matrix = shmat(id_shmem,0,0)) == (void *) - 1) {
        perror("Cannot access shared memory.\n");
        exit(1);
    }
    matrix[row*columns + column] = value; // Equivalent to matrix[column][row]

    exit(2);
}
