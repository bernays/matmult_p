$(shell export LD_LIBRARY_PATH=$PWD:LD_LIBRARY_PATH)

all: matmult_p multiply myshell

matmult_p: main.c
	gcc -o matmult_p main.c error.c 
multiply: multiply.c
	gcc multiply.c -o multiply
myshell: myshell.c
	gcc -g -o myshell myshell.c -lreadline

clean:
	-rm -f *.a *.o *~ *.so matmult_p multiply myshell