#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>        /* errno */
#include<time.h>        /* time() */

#define ARR_SIZE 20

int main(int argc, char **argv) {
        int pid, c, p; // deklarasi variabel
        
        // memasukkan nilai producer dan consumer
        printf("Masukkan jumlah proses: \n");
        printf("producer : ");scanf("%d", &p);
        printf("consumer: ");scanf("%d",&c);


        int size = ARR_SIZE * sizeof(int);
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;


        switch (pid = fork()) {
        case 0: /*  proses anak */
                //isi data di memory map
                srand(time(NULL));
                
                // mengisi random number kedalam buffer
                printf("Producer:\n");
                for (int i = 0; i < p; i++)
                {
                    // random number dari 0 sampai 99
                    shared[i] = rand() % 100;
                    printf("Producer menghasilkan: %d\n", shared[i]);
                }
                break;
        default:        /*  ortu */
                printf("----------\n");
                int pidWait,status,total=0;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        
                        // mengambil random number dari buffer dan kemudian dijumlahkan
                        printf("Consumer:\n");
                        for (int i = 0; i < c; i++) {
                            printf("Consumer mengambil: %d\n", shared[i]);
                            total += shared[i];
                        }
                        break; //keluar dari loop wait
                    if ((pidWait == -1) && (errno != EINTR)) {
                        /* ada error*/
                        perror("waitpid");
                        exit(1);
                    }
                }
                printf("Total yang ada didalam buffer: %d\n", total);
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}
