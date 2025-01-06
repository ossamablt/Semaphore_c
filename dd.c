#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
pthread_mutex_t mutex;
pthread_mutex_t write_block;
int data = 0, read_count = 0;
void *lecteur(void *arg)
{
    int id = *((int *)arg);

    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1)
    {
        pthread_mutex_lock(&write_block);
    }
    pthread_mutex_unlock(&mutex);

    printf(" Lecteur %d : a lu la donnée = %d\n", id, data);
    sleep(1);

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0)
    {
        pthread_mutex_unlock(&write_block);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}
void *redacteur(void *arg)
{
    int id = *((int *)arg);
    pthread_mutex_lock(&write_block);
    data++;
    printf("✍️ Rédacteur %d : a écrit la donnée = %d\n", id, data);
    sleep(1);
    pthread_mutex_unlock(&write_block);
    return NULL;
}

int main()
{
    pthread_t lecteurs_threads[5], redacteurs_threads[3];
    int lecteurs_ids[5], redacteurs_ids[3];
    // Initialisation des mutex
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_block, NULL);
    for (int i = 0; i < 5; i++)
    {
        lecteurs_ids[i] = i + 1;
        pthread_create(&lecteurs_threads[i], NULL, lecteur, &lecteurs_ids[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        redacteurs_ids[i] = i + 1;
        pthread_create(&redacteurs_threads[i], NULL, redacteur, &redacteurs_ids[i]);
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(lecteurs_threads[i], NULL);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(redacteurs_threads[i], NULL);
    }
    // Détruire les mutex
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_block);
    return 0;
}