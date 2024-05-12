/*
** select.c -- a select() demo
*/
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define STDIN 0 // standard input 的 file descriptor
int main(void)
{
  struct timeval tv;
  fd_set readfds;

  tv.tv_sec = 2;
  tv.tv_usec = 500000;

  FD_ZERO(&readfds);
  FD_SET(STDIN, &readfds);

  // 不用管 writefds 与 exceptfds：
  select(STDIN+1, &readfds, NULL, NULL, &tv);
  char input[1024]; 
  if (FD_ISSET(STDIN, &readfds)){
    printf("A key was pressed!\n");
    if (fgets(input, sizeof(input), stdin)) { // Read input from stdin
            printf("You entered: %s", input);
    }
  }
  else
    printf("Timed out.\n");
  return 0;
}