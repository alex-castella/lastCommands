#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define SIZE 512
#define TARGET 10

int main() {
  struct termios newt, oldt;
  char commandList[TARGET][SIZE];
  char c;
  char envHomeCopy[SIZE];
  char commands[SIZE];
  char *bashHistory = "/.bash_history";
  int i = 0;
  int j = TARGET;
  int selectedIndex = 0;
  long int pos;

  // Get .bash_history path const
  char *envHome = getenv("HOME");
  // printf("%s", envHome);
  if (envHome == NULL) {
    exit(EXIT_FAILURE);
  }
  strcpy(envHomeCopy, envHome);
  // strcat(envHomeCopy, bashHistory);
  snprintf(envHomeCopy, sizeof(envHomeCopy), "%s%s", envHome, "/.bash_history");
  // printf("\n");
  // printf("Complete path: %s", envHomeCopy);
  // printf("\n");

  // Open .bash_history and reading commands
  FILE *f = fopen(envHomeCopy, "r");
  if (f == NULL) {
    perror("Error opening file.");
    _exit(EXIT_FAILURE);
  }
  // Point to the end of the file(from las command)
  fseek(f, 0L, SEEK_END);
  pos = ftell(f);
  // printf("%ld", pos);
  // Substract one position because the last char i EOF
  pos--;
  while (i < TARGET) {
    if (!fseek(f, --pos, SEEK_SET)) {
      if (fgetc(f) == '\n' && (fgets(commands, sizeof(commands), f) != NULL)) {
        if (commands[0] != '#') {
          strcpy(commandList[i], commands);

          // printf("Command n %d: %s\n", i + 1, commands);
          i++;
        }
      }
    }
  }
  fclose(f);
  fflush(stdout);

  for (int j = i - 1; j >= 0; j--) {
    if (selectedIndex == j) {
      printf("\x1b[7m Command n %d: %s\x1b[0m\n", (i - j), commandList[j]);
    } else {
      printf("Command n %d: %s\n", (i - j), commandList[j]);
    }
  }
  fflush(stdout);
  printf("HERE\n");

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  while (c != '\n' && c != '\r') {

    c = getchar();
  }
  printf("EXIT PROGRAM\n");
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
