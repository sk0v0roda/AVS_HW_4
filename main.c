#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semhotel;   // количество свободных мест в отеле
FILE *input;   
FILE *output;

void *guest(void *arg) {
    int num = *(int *)arg; // аргумент - это указатель на номер гостя
    sem_wait(&semhotel); // гость ждет пока в отеле освободится место
    fprintf(output, "Гость %d зашел в отель\n", num + 1);
    printf("Гость %d зашел в отель\n", num + 1);
    fprintf(output, "Гость %d спит в отеле...\n", num + 1);
    printf("Гость %d спит в отеле...\n", num + 1);
    sleep(rand() % 7 + 1); // гость остается в отеле на протяжении 1-7 секунд (хотел сделать дней для реалистичности но пожалел проверяющих:))
    sem_post(&semhotel); // гость выходит из отеля
    fprintf(output, "Гость %d покинул отель\n", num + 1);
    printf("Гость %d покинул отель\n", num + 1);
    return NULL;
}

int main(int argc, char *argv[]) {
	srand(time(0));
    unsigned int n = 0; // количество посетителей отеля
    if (argc < 3) { // проверка аргументов командной строки
    	printf("Please enter both filenames\n");
    	return 0;
    }
    input = fopen(argv[1], "r"); 
    output = fopen(argv[2], "w"); 
    if (input == NULL || output == NULL) { // проверка файлов
    	printf("Faied to open one of the files, please enter names of existing files\n");
    	return 0;
    }
    fscanf(input, "%d", &n); // считываем количество гостей
    sem_init(&semhotel, 0, 30); // определяем семафору отель
    pthread_t guests[n]; // массив потоков, по одному на каждого гостя
    int id[n];        // массив с аргументами для потоков - так как нужно передавать указатели, приходится создавать отдельный для каждой функции чтобы они друг другу не мешали
    for (int i = 0; i < n; ++i) {
    	id[i] = i; // аргумент потока - номер гостя
    	pthread_create(&guests[i], NULL, guest, (void *) (id + i)); // создаем поток
    }
    for (int i = 0; i < n; ++i) {
    	pthread_join(guests[i], NULL);
    }
    sem_destroy(&semhotel);
    fclose(input);
    fclose(output);
    return 0;
}
