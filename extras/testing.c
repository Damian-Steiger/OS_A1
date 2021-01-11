#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int BUFFER = 25;

void writeOutput(char* command, char* output)
{
	printf("The output of: %s : is\n", command);
	printf(">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);	
}

int main(int argc, char **argv) {
    
    /*create a dynamically allocated array*/
        char ***array = malloc(BUFFER * sizeof(char **));
        for (int i = 0; i < BUFFER; i++) {
            array[i] = malloc(BUFFER * sizeof(char*));
            for (int j = 0; j < BUFFER; j++) {
                array[i][j] = malloc(BUFFER * sizeof(char));
            }
        }

    /*read the contents of the file*/ 
        array[0][0] = "pwd";
        array[0][1] = NULL;
        array[1][0] = "ls";
        array[1][1] = "-l";
        array[1][2] = "-a";
        array[1][3] = "-F";
        array[1][4] = NULL;
        array[2][0] = "ps";
        array[2][1] = NULL;
        array[3][0] = "pwd";
        array[3][1] = "-L";
        array[3][2] = "-P";
        array[3][4] = NULL;
        array[4][0] = "ls";
        array[4][1] = NULL;
        array[5][0] = "cat";
        array[5][1] = "makefile";
        array[5][2] = NULL;

    /* create the pipe */
        int fd[2];
        if ( pipe (fd) == -1) {
            fprintf(stderr,"Pipe failed");
            return 1;
        }

    /* fork a child process */
        pid_t pid;
        char *buf = malloc(BUFFER * sizeof(char));
        pid = fork ();

    /*check if fork failed*/
        if (pid < 0) {
            fprintf(stderr, "Fork Failed");
        return 1;

        } else if(pid == 0) { /*child*/
            for(int k = 0; k <= 5; k++){
                close(fd[0]);
                dup2(fd[1], 1);
                dup2(fd[1], 2);
                close(fd[1]);
                
                execvp(array[1][0], array[1]);
            }     

        } else { /*parent*/
            close(fd[1]);
            wait(NULL);
            read(fd[0], buf, BUFFER); 
            writeOutput(array[0][0], buf);
            close(fd[0]);   
        }

    return 0;

}