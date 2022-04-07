#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include<time.h> 

#define ARR_SIZE 50
#define MAX_VALUE 50
#define MIN_VALUE 1

int main(int argc, char const *argv[])
{
    char inbuf[ARR_SIZE]; // buffer
    int pid;
    int fd[2]; // pipe
    int producer, consumer;


    // create pipe
    if(pipe(fd) < 0){
        exit(1);
    }
    
                printf("masukan jumlah proses producer: ");
            scanf("%d", &producer);
                    printf("masukan jumlah proses consumer: ");
        scanf("%d", &consumer);
            if (consumer > producer) consumer = producer;


    printf("mulai\n");

    switch (pid = fork())
    {
    case 0: // fork return 0 ke proses anak
        // printf("proses anak\n");
        // tutup bagian input dari pipe
        close(fd[0]);


        // tulis ke pipe
        srand(time(NULL));
        int randomNumberBuffer[sizeof(producer) + 1];
        for (int i = 0; i < producer; i++)
        {
            randomNumberBuffer[i] = (rand() % (MAX_VALUE + 1) - MIN_VALUE + MIN_VALUE);
            printf("Producer memberikan nilai : %d\n", randomNumberBuffer[i]);
        }
        write(fd[1], randomNumberBuffer, sizeof(randomNumberBuffer));

        

        break;
    default: // fork return pid ke proses ortu

        // printf("proses ortu\n");
        // tutup bagian output dari pipe
        close(fd[1]);


        // baca yang ditulis child dari pipe
        int addChildNumber[sizeof(consumer) + 1];

        read(fd[0], addChildNumber, sizeof(addChildNumber));
        int jumlahNumber = 0;
        for (int i = 0; i < consumer; i++)
        {
            printf("Consumer mengambil nilai : %d\n", addChildNumber[i]);
            jumlahNumber += addChildNumber[i];
        }
            printf("jumlah number %d\n", jumlahNumber);

        break;
    
    }
    return 0;
}