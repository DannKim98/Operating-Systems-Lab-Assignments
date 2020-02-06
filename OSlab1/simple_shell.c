#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#define CMDLEN 80

int main(){
   int pid; int status;
   char command[CMDLEN];
   printf("Program begins\n");
   for(;;){
           // user input
           printf("Please enter a command: ");
           //TODO1:​ ​ user input command preparation using fgets()
           fgets(command, CMDLEN, stdin);
           command[strcspn(command, "\n")] = 0;
           if (strcmp(command, "quit") == 0) break;
           pid = fork();
           if (pid < 0){ // fork error
               fprintf(stderr, "Fork Failed");
               return 1;
           }else if (pid > 0){ // parent
               //TODO2: print pid of Child and the status (0 means okay)
           	   int status;
           	   pid_t return_pid = waitpid(pid, &status, 0); //on success, returns the process ID of the child whose state has changed
           	   // siginfo_t info;
               // pid_t status = waitid( P_PID, pid, &info, WEXITED);
           	   printf("child pid=%i, parent pid=%i (parent ppid=%i), status=%i\n", pid, getpid(),getppid(), status);
           }else{ // child
               //TODO3: print received command and execute the command.
               printf("(recieved %s)\n", command);
               execlp(command, command, NULL);
           }
           printf("---------------------------------------------\n");
   }
   return 0;
}
