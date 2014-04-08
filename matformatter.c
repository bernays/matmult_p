//
//  matformatter.c
//  assignment2
//
//  Created by Benjamin Bernays on 4/8/14.
//  Copyright (c) 2014 Benjamin Bernays. All rights reserved.
//

#include <stdio.h>
#include "apue.h"
void readmat(int inputmatrix1[20][20], int* rows, int *columns);

int main(){
    int inputmatrix1[20][20];
    int transpose[20][20];
    int rows; // The number of rows of the 2D array
    int columns;// The number of columns of the 2D array
    int row,column;
    int rows2, columns2; //Size of 2nd 2D array
    readmat(inputmatrix1, &rows, &columns);
    


    for( row = 0 ; row < rows ; row++ )
    {
        for( column = 0 ; column < columns ; column++ )
        {
            transpose[column][row] = inputmatrix1[row][column];
        }

    }
    
    for( row = 0 ; row < columns ; row++ )
    {
        for( column = 0 ; column < rows ; column++ )
        {

            printf("%d ",inputmatrix1[column][row]);
        }  
        printf("\n");
    }
    
    
    
}
void readmat(int inputmatrix1[20][20], int* rows, int *columns){
    char stdbuf[MAXLINE];
    int rowcounter =0, columncounter =0;
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
        columncounter = 0;
        
        strpos =0;
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
                    fprintf(stderr, "Invalid number of columns in first matrix\n");
                    exit(-1);
                    
                }
            }
            else{
                *columns = columncounter;
            }
        }

        
        if(matrix2==0){
            rowcounter++;
            *rows = rowcounter; // The number of rows of the 2D array
            *columns = columncounter; // The number of columns of the 2D array
        }
        
    }
    
}
char* itoa(int val, char* buf, int base){
    
    int i = 28;
    for(; val && i ; --i, val /= base){
        
        buf[i] = "0123456789abcdef"[val % base];
        
    }
    
    return &buf[i+1];
}
