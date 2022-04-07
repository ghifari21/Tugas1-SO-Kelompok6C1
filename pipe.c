#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <time.h>        /* time() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>        /* errno */

int main(int argc, char **argv) {
        int pid;
        int fd[2]; //pipe
        int producer, consumer;
        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }
        printf("mulai \n");
        
        printf("Masukan jumlah yang diproduksi Producer: ");
        scanf("%d", &producer);
        printf("Masukan jumlah yang diambil Consumer: ");
        scanf("%d", &consumer);
        
        switch (pid = fork()) {
        case 0:         /* fork returns 0 ke proses anak */
                printf("----------\n");
                /* tutup bagian input dari pipe */
                close(fd[0]);
                // tulis ke pipe
                srand(time(NULL));
                printf("Producer:\n");
                int arrIntChild[sizeof(producer)];
                for (int i = 0; i < producer; i++) {
                    // generate random number dari 0 sampai 99
                    arrIntChild[i] = rand() % 100;
                    printf("Producer menghasilkan: %d\n", arrIntChild[i]);
                }
                    write(fd[1], arrIntChild, sizeof(arrIntChild));
                break;
        default:        /* fork returns pid ke proses ortu */
                printf("----------\n");
                int pidWait,status,total=0;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        // mengambil random number dari buffer dan kemudian dijumlahkan
                        close(fd[1]);
                        // baca yang ditulis child dari pipe
                        printf("Consumer:\n");
                        int arrIntParent[sizeof(consumer)];
                        read(fd[0], arrIntParent, sizeof(arrIntParent));
                        for (int i = 0; i < consumer; i++) {
                            printf("Consumer mengambil: %d\n", arrIntParent[i]);
                            total += arrIntParent[i];
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
                /* tutup bagian output dari pipe */

                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}