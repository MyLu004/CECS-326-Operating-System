#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


//Error value return list:
//1 : missing argument, missing I/O file
//-1 : folk error, pipe error
//5: child process error
// 6: parent process error


int main(int argc, char* argv[]){
        int fd[2]; //pipe descriptor : 1.write | 2.read
        FILE *myInputFile, *myOutputFile;
        // Buffer value to hold the file content
        char myBuffer[256];

        //store the result of functions that return the number of bytes read/write
        ssize_t bytesRead;

        //ssize_t : system call is to read/write from specified file with argument of file descriptor fd

        //check if there correct amount of argument are provided
        if (argc != 3){
                printf("Error1: Missing argument, %s<input file> <output file>\n", argv[0]);
                return 1;
        }

        //open the input file, argv[1] = input file - for read mode
        myInputFile = fopen(argv[1], "r");
        if (myInputFile == NULL){
                printf("Error1, unable to open source file %s\n", argv[1]);

                //if error occur, close inputFile before exiting
                fclose(myInputFile);
                return 1;
        };

         //open the outfile, argv[2] = output file - for write mode
        myOutputFile = fopen(argv[2], "w");
        if (myOutputFile == NULL){
                printf("Error1, unable to open source file %s\n", argv[2]);
                fclose(myOutputFile);
                return 1;
        }


        //create the pipe
        //
        if (pipe(fd) == -1){
                printf("error -1 : occurred with pipe");

        }

        //FORK PROCESS
        //
        //Make/declare fork
        int myForkId = fork();
        if (myForkId == -1){
                printf("Error -1 :occurred with fork\n");
                //close both file before exiting
                fclose(myInputFile);
                fclose(myOutputFile);
                return -1;
        };


         //check if the folkId is valid
        //0 : process -> children
        //else: process -> parent
        if (myForkId == 0){
                //CHILD PROCESS
                close(fd[1]);

                //read from the pipe
                //write to the output file
                while ((bytesRead = read(fd[0],myBuffer,sizeof(myBuffer)))>0){

                        if(bytesRead == -1){ //check if read() fail :(
                                printf("Error5, childprocess: reading from pipe");
                                close(fd[1]);
                                close(fd[0]);
                                fclose(myOutputFile);
                                fclose(myInputFile);
                                return 5;
                        };

                        //write to the output file
                        if(fwrite(myBuffer, 1, bytesRead, myOutputFile)!= bytesRead){
                                printf("Error5, childprocess: writing output to file");
                                close(fd[1]);
                                close(fd[0]);
                                fclose(myOutputFile);
                                fclose(myInputFile);
                                return 5;
                        }
                        //fwrite(myBuffer, 1, bytesRead, myOutputFile);

                }

                printf("finish writing, child process, r pipe -> w output");
                close(fd[0]); // close read end after reading
                fclose(myOutputFile); //close output file after writing
        }