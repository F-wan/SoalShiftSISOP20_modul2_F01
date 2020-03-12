#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <wait.h>
#include <dirent.h>
#include <string.h>

int is_regular_file(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
} 

int main()
{
  pid_t child_modul2_id;
  int status_modul2;
  child_modul2_id = fork();
  
  if(child_modul2_id < 0)
  {
    puts("fail to create child_modul2_id");
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if((chdir("/home/kaori02/")) < 0)
  {
    puts("fail to chdir home");
    exit(EXIT_FAILURE);
  }

  if (child_modul2_id == 0)
  {
    // child mkdir 
    puts("creating modul2");
    char *modul2[] = {"mkdir", "-p", "modul2", NULL};
    execv("/bin/mkdir", modul2);
  } 
  else
  {
    // parent
    while ((wait(&status_modul2)) > 0);

    pid_t child_a1_id;
    int status_a1;
    child_a1_id = fork();

    if (child_a1_id < 0)
    {
      puts("fail to create child_a1_id");
      exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if((chdir("modul2/")) < 0)
    {
      puts("fail to chdir modul2");
      exit(EXIT_FAILURE);
    }

    if (child_a1_id == 0)
    {
      //child_a1_id
      puts("creating indomie");
      char *indomie[] = {"mkdir", "-p", "indomie", NULL};
      execv("/bin/mkdir", indomie);
    }
    else
    {
      //parent
      while ((wait(&status_a1)) > 0);
      sleep(5);

      pid_t child_a2_id;
      int status_a2;
      child_a2_id = fork();

      if (child_a2_id < 0)
      {
        puts("fail to create child_a2_id");
        exit(EXIT_FAILURE);
      }

      if (child_a2_id == 0)
      {
        //child_a2_id
        puts("creating sedaap");
        char *sedap[] = {"mkdir", "-p", "sedaap", NULL};
        execv("/bin/mkdir", sedap);
      }
      else
      {
        //parent
        while ((wait(&status_a2)) > 0);

        pid_t child_copy_id;
        int status_copy;
        child_copy_id = fork();

        if (child_copy_id < 0)
        {
          puts("fail to create child_copy_id");
          exit(EXIT_FAILURE);
        }

        if (child_copy_id == 0)
        {
          puts("copying jpg.zip to modul2");
          char *copy[] = {"cp", "/home/kaori02/Downloads/jpg.zip", "/home/kaori02/modul2", NULL};
          execv("/bin/cp", copy);
        }
        
        else
        {
          //parent
          while ((wait(&status_copy)) > 0);
          pid_t child_unzip_id;
          int status_unzip;
          child_unzip_id = fork();

          if (child_unzip_id < 0)
          {
            puts("fail to create child_unzip_id");
            exit(EXIT_FAILURE);
          }

          if (child_unzip_id == 0)
          {
            puts("unzipping jpg.zip");
            char *unzip[] = {"unzip", "jpg.zip", NULL};
            execv("/usr/bin/unzip", unzip);
          }
          else
          {
            //parent
            while ((wait(&status_unzip)) > 0);
            DIR *d;
            struct dirent *dir;
            d = opendir("/home/kaori02/modul2/jpg");

            if((chdir("jpg/")) < 0)
            {
              puts("fail to chdir jpg/");
              exit(EXIT_FAILURE);
            }

            while((dir = readdir(d)) != NULL) //smpe gada file lagi di dir
            {
              if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
              pid_t child_c_id;
              int status_c;
              child_c_id = fork();

              if (child_c_id < 0)
              {
                puts("fail to create child_c_id");
                exit(EXIT_FAILURE);
              }

              if (child_c_id == 0)
              {
                if(is_regular_file(dir->d_name))
                {
                  printf("file %s to sedap\n", dir->d_name);
                  char* sedap[] = {"mv", dir->d_name, "/home/kaori02/modul2/sedaap", NULL};
                  execv("/bin/mv", sedap);
                }
                else
                {
                  printf("dir %s to indomie\n", dir->d_name);
                  char* indomie[] = {"mv", dir->d_name, "/home/kaori02/modul2/indomie", NULL};
                  execv("/bin/mv", indomie);
                }
              }
            }
            closedir(d);

            if((chdir("/home/kaori02/modul2/")) < 0)
            {
              puts("fail to back to modul2");
              exit(EXIT_FAILURE);
            }
            
          }
        }
      }
    }
  }
}