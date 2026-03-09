#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <ncurses.h>

#define SIZE 512
#define TARGET 20

int main() {
  char c;
  char envHomeCopy[SIZE]; 
  char actualCommand[SIZE];
  char **commandList = malloc(TARGET*sizeof(SIZE));
  char *bashHistory = "/.bash_history";
  pid_t pid;
  int wstatus;
  int fd[2];
  int i = 0;
  int j = 0;
  long int pos;



      //Get .bash_history path
      const char *envHome = getenv("HOME");
      //printf("%s", envHome);
      if(envHome == NULL){
      exit(EXIT_FAILURE);
      }
      strcpy(envHomeCopy, envHome);
      //strcat(envHomeCopy, bashHistory);
      snprintf(envHomeCopy, sizeof(envHomeCopy), "%s%s", envHome, "/.bash_history");
      //printf("\n");
      //printf("Complete path: %s", envHomeCopy);
      //printf("\n");

      //Open .bash_history and reading commands
      FILE *f = fopen(envHomeCopy, "r");
      if (f == NULL) {
        perror("Error opening file.");
        _exit(EXIT_FAILURE);
      }
      //Point to the end of the file(from las command)
      fseek(f, 0L, SEEK_END);
      pos = ftell(f);
      //printf("%ld", pos);
      pos--;
      while(i < TARGET){
        if(!fseek(f, --pos, SEEK_SET)){
          if(fgetc(f) == '\n' && (fgets(actualCommand, sizeof(actualCommand),f) != NULL)){
            if(actualCommand[0] != '#'){
              printf("Command n %d: %s\n", i + 1, actualCommand);
              //Reuse envHomeCopy as bufer for snprintf
              if((snprintf(commandList[i], SIZE, "%s", actualCommand) >= SIZE)){
                perror("Error copying commandList.");
                _exit(EXIT_FAILURE);
              }
              i++;
            }
          }
        }
      }
      //initscr();              /*Start curses mode */ 
      //printw("NCURSES\n");    /*Print */
      //refresh();              /*Print it on to the real screen */
      //getch();                /*Wait for user input */
      //endwin();               /*End curses mode */

      

      fclose(f);
      fflush(stdout);
      exit(EXIT_SUCCESS);
  
}
