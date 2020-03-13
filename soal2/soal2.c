//buat file c yg jalan tiap 30 detik
// daemon

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <wait.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int main()
{
  pid_t child_fstFolder_id;
  int status_fstFolder;
  child_fstFolder_id = fork();
  
  if(child_fstFolder_id < 0)
  {
    puts("fail to create child_fstFolder_id");
    exit(EXIT_FAILURE);
  }

  if (child_fstFolder_id == 0)
  {
    puts("creating folder_khusus");
    char *folder_khusus[] = {"mkdir", "-p", "folder_khusus", NULL};
    execv("/bin/mkdir", folder_khusus);
  } 
  else
  {
    // parent
    while ((wait(&status_fstFolder)) > 0);

    if((chdir("folder_khusus/")) < 0)
    {
      puts("fail to chdir folder_khusus");
      exit(EXIT_FAILURE);
    }

    pid_t createC_id;
    int status_CreateC;
    createC_id = fork();

    if(createC_id < 0)
    {
      puts("fail to create createC_id");
      exit(EXIT_FAILURE);
    }

    if (createC_id == 0)
    {
      //buat file c disini
      char* buatC[] = {"touch", "test.c", NULL};
      execv("/usr/bin/touch", buatC);
    }
    else
    {
      while ((wait(&status_CreateC)) > 0);
      FILE *fp;
      fp = fopen("test.c", "w");
      
      // tulis program buat bikin folder dengan nama [YYYY-mm-dd_HH:ii:ss]
      // disini

      fclose(fp);

      // compile disini
      pid_t compileC_id;
      int status_compileC;
      compileC_id = fork();

      if(compileC_id < 0)
      {
        puts("fail to create compileC_id");
        exit(EXIT_FAILURE);
      }

      if (compileC_id == 0)
      {
        char* compileC[] = {"gcc", "test.c", "-o", "test.o",NULL};
        execv("/usr/bin/gcc", compileC);
      }
      else
      {
        while (1)
        {
          // jalanin program buat bikin folder
          pid_t runC_id;
          int status_runC;
          runC_id = fork();

          if (runC_id < 0)
          {
            puts("fail to create runC_id");
            exit(EXIT_FAILURE);
          }
          
          if (runC_id == 0)
          {
            char* runC[] = {"./test.o", NULL};
            execv("./test.o", runC);
          }
          // tiap folder diisi dg 20 gambar download dari https://picsum.photos/
          // tiap gambar didownload tiap 5 detik
          sleep(30);
        }  
      }
    }
  }
}