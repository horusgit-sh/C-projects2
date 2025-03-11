#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


void usage(char *prog_name, char *filename){
  printf("Usage: %s <route to file> <data to add %s>\n", prog_name, filename);
  exit(0);
}

void fatal(char *);
void *ec_malloc(unsigned int);

int main(int argc, char *argv[]){
  int fd;
  char *buffer, * datafile;
  buffer = (char *)ec_malloc(100);
  datafile = (char *)ec_malloc(20);
  if (argc < 3){

    usage(argv[0], argv[1]);

  }
  strcpy(datafile, argv[1]);
  strcpy(buffer, argv[2]);
  printf("\t[DEBUG] buffer @ %p: \'%s'\n ", buffer, buffer);
  printf("\t[DEBUG] datafile @ %p: \'%s'\n ", datafile, datafile);
  strcat(buffer, "\n");

  fd = open(datafile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

  if (fd == -1){
    fatal("in main() while opening file");
  }
  if (write(fd, buffer, strlen(buffer)) == -1){
    fatal("in main() while writing to file");
  }
  if (close(fd) == -1){
    fatal("in main() while closing file");
  }
  printf("Note has been written to file\n");
  free(buffer);
  free(datafile);
}
void fatal(char *message){
    char error[100];
    strcpy(error, "!! Fatal Error !!");
    strncat(error, message, 83);
    perror(error);
    exit(-1);
}

void *ec_malloc(unsigned int size){
  void *ptr;
  ptr = malloc(size);
  if (ptr == NULL){
    fatal("out of memory");
  }
  return ptr;
}




