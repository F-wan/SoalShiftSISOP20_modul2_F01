#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int isNumber(char const *s) 
{ 
  if(strcmp(s, "*"))
  {
    for (int i = 0; i < strlen(s); i++) 
      if (!isdigit(s[i])) 
          return 0; 
  }
  return 1; 
} 

int inRange(int minVal, int MaxVal, char const input[])
{
  int i;
  // printf("%d\n", atoi(input));
  if(!isNumber(input))
  {
    // puts("not number");
    return 0;
  }

  for (i = minVal; i<=MaxVal; i++)
  {
    if(atoi(input) == i)
    {
      // puts("di dalam range");
      return 1;
    }
  }
  return 0;
}

int CekWaktu(int inputSec, int inputMin, int inputHour)
{
  time_t t = time(NULL);
  struct tm timeNow = *localtime(&t);
  int hour, min, sec, sleepTime;
  
  /*cek hour*/
  if(inputHour == 999)
  {
    //puts("any hour");
    hour = 0;
  }
  else if(inputHour < timeNow.tm_hour)
  {
    hour = 24 - (timeNow.tm_hour - inputHour);
    hour *= 3600;
  }
  else if(inputHour == timeNow.tm_hour)
  {
    hour = 24 - timeNow.tm_hour;
    hour *= 3600;
  }
  else
  {
    hour = inputHour - timeNow.tm_hour;
    hour *= 3600;
  }

  /*cek min*/
  if(inputMin == 999)
  {
    //puts("any min");
    min = 0;
  }
  else if(inputMin < timeNow.tm_min)
  {
    min = 60 - (timeNow.tm_min - inputMin);
    min *= 60;
  }
  else if(inputMin == timeNow.tm_min)
  {
    min = 60 - timeNow.tm_min;
    min *= 60;
  }
  else
  {
    min = inputMin - timeNow.tm_min;
    min *= 60;
  }

  /*cek sec*/
  if(inputSec == 999)
  {
    //puts("any sec");
    sec = 0;
  }
  else if(inputSec < timeNow.tm_sec)
  {
    sec = 60 - (timeNow.tm_sec - inputSec);
  }
  else if(inputSec == timeNow.tm_sec)
  {
    sec = 60 - timeNow.tm_sec;
  } 
  else sec = inputSec - timeNow.tm_sec;
  
  sleepTime = hour + min + sec;

  if(sleepTime < 1) sleepTime = 1;

  return sleepTime;
}

int main(int argc, char const *argv[]) 
{
  int i = 0, sec, min, hour, sleepTime, inputSec, inputMin, inputHour;

  /***************  cek parameter   ***********************/
  if(!strcmp(argv[1], "*")) //kalo sama dg "*"
  {
    inputSec = 999;
  }
  else inputSec = atoi(argv[1]);

  if(!inRange(0, 59, argv[1]))
  {
    puts("invalid seconds input");
    exit(EXIT_FAILURE);
  }
  
  if(!strcmp(argv[2], "*")) //kalo sama dg "*"
  {
    inputMin = 999;
  }
  else inputMin = atoi(argv[2]);
  
  if(!inRange(0, 59, argv[2]))
  {
    puts("invalid minute input");
    exit(EXIT_FAILURE);
  }
  
  if(!strcmp(argv[3], "*")) //kalo sama dg "*"
  {
    inputHour = 999;
  }
  else inputHour = atoi(argv[3]);

  if(!inRange(0, 23, argv[3]))
  {
    puts("invalid hour input");
    exit(EXIT_FAILURE);
  }

  char shName[999];
  strcpy(shName, argv[4]);
  /*****************  end cek parameter   ******************/

  sleepTime = CekWaktu(inputSec, inputMin, inputHour);


  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    puts("fail to fork");
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    puts("killing parent");
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while (1)
  {
    if(i > 0) sleepTime = CekWaktu(inputSec, inputMin, inputHour);
    sleep(sleepTime);
    time_t t = time(NULL);
    struct tm timeNow = *localtime(&t);
    printf("%02d:%02d:%02d\n", timeNow.tm_hour, timeNow.tm_min, timeNow.tm_sec);
    
    if ((chdir("/home/kaori02/repository/kuliah/SMT_4/SISOP_F/Modul_2/shift")) < 0)
    {
      puts("fail to chdir");
      exit(EXIT_FAILURE);
    }

    pid_t child_id;
    child_id = fork();
    if (child_id < 0)
    {
      puts("fail to fork");
      exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }
    if(child_id == 0)
    {
      // kalo pake exec programnya lgsg berhenti
      char *cmd[] = {"bash", shName, NULL};
      execv("/bin/bash", cmd);
    }

    i++;
  }
}