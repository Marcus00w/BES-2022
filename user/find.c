#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/*
mostly like ls function
*/
void
find(char *path, char *name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("find: path too long\n");
    return;
  }
  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    if(strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0) {
      // ignore directories . and ..
      continue;
    }

    if(strstr(de.name,name)) {
      // print if current file contains searched string
      printf("%d\t%s\n", st.type, buf);
    }
    if(st.type == T_DIR) {
      // run function recursively with new path
      int pathLength = strlen(path);
      int length = pathLength + strlen(de.name) + 1;
      char* newPath = 0;
      // path + "/" + directory
      newPath = (char *) malloc(length);
      strcpy(&(newPath[0]), path);
      strcpy(&(newPath[pathLength+1]),de.name);
      newPath[pathLength] = '/';

      find(newPath,name);
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    fprintf(2, "Please enter an argument for find\n");
    exit(0);
  }
  for(i=1; i<argc; i++) {
    // searching from current directory
    find(".",argv[i]);
  }
    
  exit(0);
}
