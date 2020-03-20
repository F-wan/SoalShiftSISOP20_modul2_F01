#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void membuatKiller(char source[]);

int main(int argc, char **argv)
{
  if(argc != 2)
  {
    puts("argument tidak valid");
    exit(EXIT_FAILURE);
  }
  
  pid_t pid, sid;

  pid = fork();
  
  if (pid < 0)
    exit(EXIT_FAILURE);

  if (pid > 0)
    exit(EXIT_SUCCESS);

  umask(0);

  sid = setsid();
  if (sid < 0) 
    exit(EXIT_FAILURE);

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  membuatKiller(argv[1]);

  time_t timer;
  struct tm* tm_info;

  while (1) 
  {
    timer = time(NULL);
    tm_info = localtime(&timer);
	

    pid_t child1_id;
	  int status1;
    
    child1_id = fork();
    
    char nama_folder[100];
    strftime(nama_folder, 100, "%Y-%m-%d_%H:%M:%S", tm_info);

    if (child1_id < 0)
    {
      puts("fail to fork");
      exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child1_id == 0)
    {
      if (fork() == 0)
      {
        char *argv[] = {"mkdir", "-p", nama_folder, NULL};
        execv("/bin/mkdir", argv);
      }
      else 
      {
        while ((wait(&status1)) > 0);
        for (int i = 1; i <= 20; i++)
        {
          if (fork() == 0)
          {
            time_t timer;
			      chdir(nama_folder);
            struct tm* file_tm_info;
        
            timer = time(NULL);
            file_tm_info = localtime(&timer);
        
            int p = (int)time(NULL);
            p = (p % 1000) + 100;
            
            char tujuan[100];
            char name[100];
            
            sprintf(tujuan, "https://picsum.photos/%d", p);

            
            strftime(name, 100, "%Y-%m-%d_%H:%M:%S", file_tm_info);
            char *argv[] = {"wget", tujuan, "-qO", name, NULL};
            execv("/usr/bin/wget", argv);
          }
          
          sleep(5);
        }
        char nama_folder_zip[100];
        sprintf(nama_folder_zip, "%s.zip", nama_folder);
        char *argv[] = {"zip", "-qrm", nama_folder_zip, nama_folder, NULL};
        execv("/usr/bin/zip", argv);
      }
    }
    else
      sleep(30);
  }
}

void membuatKiller(char source[])
{
  FILE *tujuan;
  tujuan = fopen("killer.sh", "w");
  int status;

  if(!strcmp(source, "-a")) fprintf(tujuan, "#!/bin/bash\nkill -9 -%d\nrm killer", getpid());

  if(!strcmp(source, "-b")) fprintf(tujuan, "#!/bin/bash\nkill %d\nrm killer", getpid());
  
  if(fork() == 0)
  {  
    if (fork() == 0)
    {
      char *argv[] = {"chmod", "u+x", "killer.sh", NULL};
      execv("/bin/chmod", argv);
    }
    else
    {
      while ((wait(&status)) > 0);
      char *argv[] = {"mv", "killer.sh", "killer", NULL};
      execv("/bin/mv", argv);
    }
  }
  fclose(tujuan);
}
