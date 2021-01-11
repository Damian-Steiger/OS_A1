#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER 250
#define READ 0
#define WRITE 1

int totalstrings = 25;

int main(int argc, char **argv) {

    /*read the file name from command line*/
        FILE *fp;
        fp = fopen(argv[1], "r");

    /*check if file was opened properly*/
        if(fp == NULL) {
            perror("Error opening file");
            return(-1);
        }
    
    /*create a dynamically allocated array*/
        char **array = malloc(totalstrings * sizeof(char *));
        for (int i = 0; i < totalstrings; i++) {
            array[i] = malloc(totalstrings * sizeof(char));
        }

    /*read the contents of the file*/ 
        for(int i = 0; i < totalstrings; i++){
            fgets(array[i], totalstrings, fp);
            array[i][strcspn(array[i], "\r\n")] = 0;
        }

    /*close the file*/
        fclose(fp);

    /*split string*/
        char *cmd[10]; 
        char delim[] = " ";
        char *ptr = strtok(array[1], delim);
        int i = 0;
        while(ptr != NULL){
            cmd[i] = ptr;
            i++;
            ptr = strtok(NULL, delim);
        }

    /* create the pipe */
        int fd[2];
        if ( pipe (fd) == -1) {
            fprintf(stderr,"Pipe failed");
            return 1;
        }

    /* fork a child process */
        pid_t pid;
        pid = fork ();

    /*check if fork failed*/
        if (pid < 0) {
            fprintf(stderr, "Fork Failed");
        return 1;
        }

    /*run commands in child*/
        if(pid == 0) {
             char * exec = execvp(cmd[0], cmd);
        }


    char write_msg[BUFFER] = "Greetings";
    char read_msg[BUFFER];


    /* close the unused end of the pipe */
        if (pid > 0) { /* parent process */
            close(fd[READ]);

        /* write to the pipe */
            write(fd[WRITE], write_msg, strlen(write_msg)+1);
            
        /* close the write end of the pipe */
            close(fd[WRITE]);
        
        } else { /* child process */
        
        /* close the unused end of the pipe */
            close(fd[WRITE]);
            
        /* read from the pipe */
            read(fd[READ], read_msg, BUFFER);
            printf("read %s",read_msg);
        }
    
    return 0;
}