#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_FILOSOFOS 3

/* Estados dos filósofos */
enum
{
    PENSANDO,
    FAMINTO,
    COMENDO
};

typedef struct ContextoFilosofos
{
    /* Identificadores de estado do filosofo */
    int             Estado, Refeicoes;
    /* Identificador da thread do filosofo*/
    pthread_t       ThreadFilosofo;
    /* Multiplexador do filosofo*/
    sem_t           FilosofoMutex;
    /* Ponteiro para o  multiplexador geral */

} CONTEXTO_FILOSOFO;

typedef struct TipoFilosofo
{
    int ID;
    sem_t * ProcessoMutex;
    CONTEXTO_FILOSOFO * Contexto;

} FILOSOFO;

void ConferirDisponibilidade(FILOSOFO * Filosofo)
{
    if(((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Estado == FAMINTO && ((FILOSOFO *)Filosofo)->Contexto[(((FILOSOFO *)Filosofo)->ID + MAX_FILOSOFOS - 1) % MAX_FILOSOFOS].Estado != COMENDO && ((FILOSOFO *)Filosofo)->Contexto[(((FILOSOFO *)Filosofo)->ID + 1) % MAX_FILOSOFOS].Estado != COMENDO)
    {
        ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Estado = COMENDO;
        ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Refeicoes ++;
        printf("Filosofo [%d] [COMEU %d x] com talheres %d e %d\n",((FILOSOFO *)Filosofo)->ID + 1, ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Refeicoes, ((((FILOSOFO *)Filosofo)->ID + MAX_FILOSOFOS) % MAX_FILOSOFOS), ((FILOSOFO *)Filosofo)->ID + 1);
        sleep(3);
        sem_post(&(((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].FilosofoMutex));
    }
}

void PegarTalher(FILOSOFO * Filosofo)
{
    sem_wait(((FILOSOFO *)Filosofo)->ProcessoMutex);
    ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Estado = FAMINTO;
    printf("Filosofo [%d] [FAMINTO]\n", ((FILOSOFO *)Filosofo)->ID);
    ConferirDisponibilidade((FILOSOFO *) Filosofo);
    sleep(1);
    sem_post(((FILOSOFO *)Filosofo)->ProcessoMutex);
    sem_wait(&(((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].FilosofoMutex));
}

void EntregarTalher(FILOSOFO * Filosofo)
{
    sem_wait(((FILOSOFO *)Filosofo)->ProcessoMutex);
    ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Estado = PENSANDO;
    printf("Filosofo [%d] [PENSANDO] entrega talheres %d e %d\n",((FILOSOFO *)Filosofo)->ID + 1, ((((FILOSOFO *)Filosofo)->ID + MAX_FILOSOFOS) % MAX_FILOSOFOS), ((FILOSOFO *)Filosofo)->ID + 1);
    ConferirDisponibilidade((FILOSOFO *) Filosofo);
    sleep(1);
    sem_post(((FILOSOFO *)Filosofo)->ProcessoMutex);
}

void * AcoesFilosofo(void * Filosofo)
{
    while(1)
    {
        printf("Filosofo [%d] [PENSANDO]\n", ((FILOSOFO *) Filosofo) -> ID);
        sleep(1);
        PegarTalher((FILOSOFO *) Filosofo);
        sleep(1);
        EntregarTalher((FILOSOFO *) Filosofo);
    }
    return NULL;
}

int main()
{
    unsigned char i;

    sem_t ProcessoMutex;

    FILOSOFO Filosofo[MAX_FILOSOFOS];

    CONTEXTO_FILOSOFO CFilosofo[MAX_FILOSOFOS];

    sem_init(&ProcessoMutex, 0, 1);

    for(i = 0; i < MAX_FILOSOFOS; i++)
        sem_init(&(CFilosofo[i].FilosofoMutex), 0, 0);

    for(i = 0; i < MAX_FILOSOFOS; i++)
    {
        Filosofo[i].ID = i;
        Filosofo[i].ProcessoMutex = &ProcessoMutex;
        Filosofo[i].Contexto = CFilosofo;
        CFilosofo[i].Estado = 0;
        CFilosofo[i].Refeicoes = 0;

        pthread_create((&(CFilosofo[i].ThreadFilosofo)), NULL, AcoesFilosofo, &Filosofo[i]);
    }

    for(i = 0; i < MAX_FILOSOFOS; i++)
        pthread_join(CFilosofo[i].ThreadFilosofo,NULL);

    return 0;
}
