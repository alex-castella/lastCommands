#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

#define SIZE 512

int main() {
  char c;
  char envHomeCopy[SIZE]; 
  char commands[SIZE];
  char *bashHistory = "/.bash_history";
  pid_t pid;
  int wstatus;
  int fd[2];
  int i = 0;

  pid = fork();
  switch (pid) {
    case -1:
      perror("Error creating process.");
      exit(EXIT_FAILURE);
    case 0:
      puts("Child existing");
      //Get .bash_history path
      const char *envHome = getenv("HOME");
      printf("%s", envHome);
      if(envHome == NULL){
      exit(EXIT_FAILURE);
      }
      strcpy(envHomeCopy, envHome);
      //strcat(envHomeCopy, bashHistory);
      snprintf(envHomeCopy, sizeof(envHomeCopy), "%s%s", envHome, "/.bash_history");
      printf("\n");
      printf("Complete path: %s", envHomeCopy);
      printf("\n");
      //Open .bash_history and reading commands
      FILE *f = fopen(envHomeCopy, "r");
      if (f == NULL) {
        perror("Error opening file.");
        _exit(EXIT_FAILURE);
      }
      
      while (fgets(commands, sizeof(commands), f) != NULL && i < 10) {
        if (commands[0] != '#') {
          printf("Command n%d: %s \n", i, commands);
          i++;
        }
      }

      fclose(f);
      fflush(stdout);
      _exit(EXIT_SUCCESS);
    default:
      wait(&wstatus);
      exit(EXIT_SUCCESS);
  }
}
