#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  char* filename = "testfile";
  int fd = open(filename, O_CREATE | O_WRONLY);

  printf("Create file (%s) to test lseek\n\n", filename);
  // write content into file
  char* s= "testing";
  int filelength = strlen(s);
  write(fd,s,filelength);
  close(fd);

  fd = open(filename, O_RDONLY);
  int num_err = 0;

  printf("\ntest normal cases\n\n");
  printf("use SEEK_SET -> ");
  for(int i=0; i<filelength; i++){
      char buf;
      if(lseek(fd, i, SEEK_SET) < 0) {
        num_err++;
        continue;
      }
      
      read(fd, &buf, 1);
      if(buf != s[i]){
        num_err++;
      }
  }
  if(num_err == 0) {
    printf("success\n");
  } else {
    fprintf(2,"%d errors\n", num_err);
  }
  lseek(fd, 0, SEEK_SET); // default state
  num_err = 0; // reset
  
  printf("use SEEK_CUR -> ");
  for(int i=0; i<filelength; i++){
      char buf;
      if(lseek(fd, 0, SEEK_CUR) < 0) {
        num_err++;
        continue;
      }
      
      read(fd, &buf, 1);
      if(buf != s[i]){
        num_err++;
      }
  }
  if(num_err == 0) {
    printf("success\n");
  } else {
    fprintf(2,"%d errors\n", num_err);
  }
  lseek(fd, 0, SEEK_SET); // default state
  num_err = 0; // reset

  printf("use SEEK_END -> ");
  for(int i=0; i<filelength; i++){
      char buf;
      if(lseek(fd, i-filelength, SEEK_END) < 0) {
        num_err++;
        continue;
      }
      
      read(fd, &buf, 1);
      if(buf != s[i]){
        num_err++;
      }
  }
  if(num_err == 0) {
    printf("success\n");
  } else {
    fprintf(2,"%d errors\n", num_err);
  }
  lseek(fd, 0, SEEK_SET); // default state
  num_err = 0; // reset


  printf("\n\ntest 7 error cases\n\n");

  if(lseek(-1, 0, SEEK_SET)<0)
    printf("check invalid file descriptor (%d) -> ERROR\n", -1);
  lseek(fd, 0, SEEK_SET); // default state

  if(lseek(0, 0, SEEK_SET)<0)
    printf("check not an inode (stdin %d) -> ERROR\n", 0);
  lseek(fd, 0, SEEK_SET); // default state

  if(lseek(fd, 0, 3)<0)
    printf("check invalid whence (%d) -> ERROR\n", 3);
  lseek(fd, 0, SEEK_SET); // default state

  if(lseek(fd, -10, SEEK_SET)<0)
    printf("check offset value to low with SEEK_SET offset: (%d) -> ERROR\n",-10);
  lseek(fd, 0, SEEK_SET); // default state

  if(lseek(fd, filelength+10, SEEK_SET)<0)
    printf("check offset value to high with SEEK_SET offset: (%d), filesize: (%d) -> ERROR\n",filelength+10,filelength);
  lseek(fd, 0, SEEK_SET); // default state

  if(lseek(fd, filelength+10, SEEK_CUR)<0)
    printf("check offset value to high with SEEK_CUR offset: (%d), filesize: (%d) -> ERROR\n",filelength+10,filelength);
  lseek(fd, 0, SEEK_SET); // default state

  if(lseek(fd, 10, SEEK_END)<0)
    printf("check offset value to high with SEEK_END offset: (%d), filesize: (%d) -> ERROR\n",10,filelength);
  lseek(fd, 0, SEEK_SET); // default state

  exit(0);
}
