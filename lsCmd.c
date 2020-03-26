#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void listfile(char *name)
{
  struct stat statbuff;	/* file attributes with stat */
  char *modtime;

  /* Array of file types, indexed by the top four bits of st_mode in the stat structure */
  char *filetype[] = { "?", "p", "c", "?", "d", "?", "b", "?", "-", "?", "l", "?", "s" };

  if (stat(name, &statbuff) < 0) {
    perror(name);
    exit(2);
  }

  /* Printing the file type */
  printf("%s", filetype[(statbuff.st_mode >> 12) & 017]);

  /* Printing the Permissions */
  printf("%c%c%c%c%c%c%c%c%c",
         (statbuff.st_mode & S_IRUSR) ? 'r' : '-',
         (statbuff.st_mode & S_IWUSR) ? 'w' : '-',
         (statbuff.st_mode & S_IXUSR) ? 'x' : '-',
         (statbuff.st_mode & S_IRGRP) ? 'r' : '-',
         (statbuff.st_mode & S_IWGRP) ? 'w' : '-',
         (statbuff.st_mode & S_IXGRP) ? 'x' : '-',
         (statbuff.st_mode & S_IROTH) ? 'r' : '-',
         (statbuff.st_mode & S_IWOTH) ? 'w' : '-',
         (statbuff.st_mode & S_IXOTH) ? 'x' : '-' );

  printf("%8ld", statbuff.st_size);

  modtime = ctime(&statbuff.st_mtime);

  /* ctime() string has \n at end, we need to remove it */
  modtime[strlen(modtime) - 1] =  '\0';
  printf("  %s  ", modtime);
  printf("%s\n", name);
}

int main(int argc, char * argv[])
{
  DIR *dir;
  struct dirent *info;
  int opt, withflag = 0;

  opterr = 0;  /* Disabling getopt's own error messages */

  while ( (opt = getopt(argc, argv, "a")) != EOF) {
    switch (opt) {
      case 'a':
        withflag = 1;
        break;
      case '?':
        fprintf(stderr, "unknown option: -%c\n", optopt);
    }
  }

  argv += optind;  /* Adjust to move past the options */
  argc -= optind;

  /* argc is the number of non-option args and argv[0] is the first one */

  if (argc != 1) {
    fprintf(stderr, "Usage: lst [-a] dirname\n");
    exit(1);
  }
  chdir(argv[0]);
  dir = opendir(".");

  while ((info = readdir(dir)) != NULL) {
    if (info->d_name[0] != '.' || withflag)
      listfile(info->d_name);
  }
}
