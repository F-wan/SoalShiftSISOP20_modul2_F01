# SoalShiftSISOP20_modul2_F01

## directory soal
- [Soal 1](soal1/)
- [Soal 2](soal2/)
- [Soal 3](soal3/)

## Soal no. 1

Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:

A. Program menerima 4 argumen berupa:
  - Detik: 0-59 atau * (any value)
  - Menit: 0-59 atau * (any value)
  - Jam: 0-23 atau * (any value)
  - Path file .sh

B. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai

C. Program hanya menerima 1 config cron

D. Program berjalan di background (daemon)

E. Tidak boleh menggunakan fungsi system()
>Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap
detik pada jam 07:34.

### [Source code : soal1.c](soal1/soal1.c)

### PENJELASAN
- Program menerima 4 argumen. Apabila tidak sesuai, program akan berhenti dan menampilkan pesan error ``"argumen tidak 4"``
```c
if(argc != 5)
{
  puts("argumen tidak 4");
  exit(EXIT_FAILURE);
}
```
- Kemudian setiap argumen akan diperiksa. Argumen pertama sampai ketiga akan diperiksa apakah sama dengan `*`. Apabila sama, detik, menit, atau jam akan dianggap sama dengan `999`
- Apabila bukan `*`, maka argumen akan diubah menjadi `integer` menggunakan fungsi `atoi()`
- Kemudian setiap argumen `argv[i]` dicek apakah masuk dalam range yang diinginkan soal menggunakan fungsi `inRange(int minVal, int MaxVal, char const input[])`
- `argv[4]` disimpan ke dalam `shName[999]`
- Jalankan program secara daemon dengan `sleepTime` yang didapat dari perhitungan pada fungsi
```c
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
    hour = 24;
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
    min = 60;
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
    sec = 60;
  } 
  else
  {
    sec = inputSec - timeNow.tm_sec;
  } 
  
  sleepTime = hour + min + sec;

  if(sleepTime < 1) sleepTime = 1;
  printf("hour = %d\tminute = %d\tsecond = %d\n", hour, min, sec);
  printf("targ = %d\ttarget = %d\ttarget = %d\n", inputHour, inputMin, inputSec);
  return sleepTime;
}
``` 
dengan argumen `inputSec`, `inputMin`, dan `inputHour`
- Setiap waktu pada komputer sama dengan waktu yang didapat dari argumen, perintah di bawah ini akan dijalankan

```c
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
```
- Perintah diatas akan memindah directory program ke `/home/kaori02/repository/kuliah/SMT_4/SISOP_F/Modul_2/shift`
- kemudian akan membuat child yang akan menjalankan command `bash` terhadap file `shName[]`

### SCREENSHOT HASIL
- isi file .sh yang dijalankan

![isi file sh](/screenshot/isiSh.png)
- Program dijalankan

![isi file sh](/screenshot/jalan1.png)

## Soal no. 2
Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia
merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat
sebuah program.

A.  Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat
sebuah program C yang per 30 detik membuat sebuah folder dengan nama
timestamp [YYYY-mm-dd_HH:ii:ss].

B. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari
https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap
gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah
detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-
mm-dd_HH:ii:ss].

C. Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan
folder akan di delete(sehingga hanya menyisakan .zip).

D. Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-
generate sebuah program "killer" yang siap di run(executable) untuk
menterminasi semua operasi program tersebut. Setelah di run, program yang
menterminasi ini lalu akan mendelete dirinya sendiri.

E. Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu
MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan
dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen
-b. Ketika dijalankan dalam MODE_A, program utama akan langsung
menghentikan semua operasinya ketika program killer dijalankan. Untuk
MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi
membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua
folder terisi gambar, terzip lalu di delete).

Kiwa lalu terbangun dan sedih karena menyadari programnya hanya sebuah mimpi. Buatlah program dalam mimpi Kiwa jadi nyata!

### [Source code : soal2.c](soal2/soal2.c)


### PENJELASAN
- Pada fungsi main:

Untuk Mengatur nama folder sesuai instruktur menggunakan date dan formatnya. Setelah itu di loop sebanyak 20x untuk mendownload gambar setelah itu gambarnya di zip. dan program akan sleep selama 30 detik dan mengulangi programnya kembali

- Void membuatkiller :

Ini adalah killer dari program ini. ketika user menjalankan program dengan tipe a dan killer dijalankan maka killer akan membunuh parent dan child. ketika program dengan tipe `"-b"` dijalankan dan killer dijalankan juga maka killer hanya membunuh parent, dengan kata lain program akan berjalan hingga semua folder nya di zip.

### SCREENSHOT HASIL
- #### Uji coba program dengan mode a

![coba-a.jpg](/screenshot/coba-a.jpg)

![coba-a-2.jpg](/screenshot/coba-a-2.jpg)

![coba-a-mati.jpg](/screenshot/coba-a-mati.jpg)



- #### Uji coba program dengan mode b

![coba-b.jpg](/screenshot/coba-b.jpg)

![coba-b-mati.jpg](/screenshot/coba-b-mati.jpg)

![coba-a-Mcek.jpg](/screenshot/coba-b-Mcek.jpg)


## Soal no. 3
Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia
memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan
(multiprocessing).

A.  Program buatan jaya harus bisa membuat dua direktori di
“/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu
lima detik kemudian membuat direktori yang kedua bernama “sedaap”.

B.  Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori
“/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak
hanya itu tugasnya.

C.  Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di
dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan
pengelompokan, semua file harus dipindahkan ke
“/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke
“/home/[USER]/modul2/indomie/”.

D.  Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”
harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu
3 detik kemudian membuat file bernama “coba2.txt”.

>(contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat
program tersebut.
Catatan :
- Tidak boleh memakai system().
- Tidak boleh memakai function C mkdir() ataupun rename().
- Gunakan exec dan fork
- Direktori “.” dan “..” tidak termasuk

### [Source code : soal3.c](soal3/soal3.c)


### PENJELASAN
- Pertama kita buat child process untuk membuat folder `modul2`
```c
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
```
- Setelah selesai membuat folder `modul2`, pindah directory ke folder `modul2` lalu membuat child process lain untuk membuat folder `indomie` di dalam folder `modul2`
```c
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
```
- Setelah selesai membuat folder `indomie`, tunggu 5 detik kemudian membuat child baru untuk membuat folder `sedaap` di dalam folder `modul2`
```c
while ((wait(&status_a1)) > 0);
puts("wait for 5 sec");
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
```
- Setelah selesai membuat folder `sedaap`, buat child process lain untuk mengekstrak file `jpg.zip`
```c
while ((wait(&status_a2)) > 0);
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
  char *unzip[] = {"unzip", "/home/kaori02/Downloads/jpg.zip", NULL};
  execv("/usr/bin/unzip", unzip);
}
```

- Setelah selesai mengekstrak `jpg.zip`, cek semua isi dari folder `/modul2/jpg`. Apabila isi dari folder adalah file, maka file tersebut akan dipindahkan ke folder `/modul2/sedaap`. Sementara apabila isi dari folder adalah folder, maka folder tersebut akan dipindahkan ke folder `/modul2/indomie`.
```c
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
```
- Setelah selesai memindahkan semua isi dari folder `jpg` ke `indomie` dan `sedaap`, pindah directory program ke folder `indomie`
- Kemudian membuat file `coba1.txt` dan `coba2.txt` di setiap direktori yang ada pada folder `indomie`
- file `coba1.txt` akan dibuat terlebih dahulu, kemudian 3 detik kemudian membuat file `coba2.txt`
```c
while ((wait(&status_unzip)) > 0);
DIR *d_indomie;
struct dirent *dir_indomie;
d_indomie = opendir("/home/kaori02/modul2/indomie");

while((dir_indomie = readdir(d_indomie)) != NULL) //smpe gada file lagi di dir_indomie
{
  if(strcmp(dir_indomie->d_name,".") == 0 || strcmp(dir_indomie->d_name, "..") == 0) continue;
  
  if((chdir("/home/kaori02/modul2/indomie")) < 0)
  {
    puts("fail to chdir modul2/indomie");
    exit(EXIT_FAILURE);
  }

  pid_t child_d_id;
  int status_d;
  child_d_id = fork();

  if (child_d_id < 0)
  {
    puts("fail to create child_d_id");
    exit(EXIT_FAILURE);
  }

  char indomie[999];
  strcpy(indomie, "/home/kaori02/modul2/indomie/");
  char dirname[999];
  strcpy(dirname, dir_indomie->d_name);
  char lokasiCoba[999];
  strcpy(lokasiCoba, strcat(indomie, dirname));

  if (child_d_id == 0)
  {
    // buat file coba1.txt
    char fileCoba1[999];
    strcpy(fileCoba1, strcat(lokasiCoba, "/coba1.txt"));

    printf("coba1 =  %s\n", fileCoba1);
    char* coba1[] = {"touch", fileCoba1, NULL};
    execv("/usr/bin/touch", coba1);
  }
  else
  {
    // 3 detik kemudian buat coba2.txt
    while ((wait(&status_d)) > 0);
    puts("wait for 3 sec");
    sleep(3);

    pid_t child_d1_id;
    int status_d1;
    child_d1_id = fork();

    if (child_d1_id < 0)
    {
      puts("fail to create child_d1_id");
      exit(EXIT_FAILURE);
    }
    if(child_d1_id == 0)
    {
      // puts(indomie);              
      char fileCoba2[999];
      strcpy(fileCoba2, strcat(lokasiCoba, "/coba2.txt"));

      printf("coba2 =  %s\n\n", fileCoba2);
      char* coba2[] = {"touch", fileCoba2, NULL};
      execv("/usr/bin/touch", coba2);
    }
  }
}
closedir(d_indomie);
```

### SCREENSHOT HASIL
- Program dijalankan

![smpeUnzip](/screenshot/smpeUnzip.png)

![move](/screenshot/move.png)

![coba12](/screenshot/coba12.png)


- Isi directory modul2

![ssIsiModul2](/screenshot/ssIsiModul2.png)


- Isi directory jpg

![ssIsiJpg](/screenshot/ssIsiJpg.png)


- Isi directory sedaap

![ssIsiSedaap](/screenshot/ssIsiSedaap.png)


- Isi directory indomie

![ssIsiIndomie](/screenshot/ssIsiIndomie.png)


- Isi directory 744

![744](/screenshot/744.png)


- Isi directory 3577

![3577](/screenshot/3577.png)


- Isi directory 3985

![3985](/screenshot/3985.png)


- Isi directory 5071

![5071](/screenshot/5071.png)


- Isi directory 5353

![5353](/screenshot/5353.png)


- Isi directory 6372

![6372](/screenshot/6372.png)


- Isi directory 6885

![6885](/screenshot/6885.png)


- Isi directory 6944

![6944](/screenshot/6944.png)


- Isi directory 7191

![7191](/screenshot/7191.png)


- Isi directory 7333

![7333](/screenshot/7333.png)


- Isi directory 7592

![7592](/screenshot/7592.png)


- Isi directory 7912

![7912](/screenshot/7912.png)


- Isi directory 8352

![8352](/screenshot/8352.png)

