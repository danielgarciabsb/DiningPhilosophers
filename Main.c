/*

    O Incrível Jantar dos Filósofos

    Copyright (C) 2013

    Authors:
    Camila Nogueira <kamilabnogueira@gmail.com>
    Daniel Garcia <contato@danielgarciaweb.com>
    Felipe Aires <>

    Licence:

    This file is part of "O Incrível Jantar dos Filósofos"

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

    Online license:
    http://www.gnu.org/licenses/
    http://www.gnu.org/licenses/gpl.html

*/

/*
    Inclusão de bibliotecas POSIX
*/

/* IO padrão */
#include <stdio.h>
#include <stdlib.h>
/* Operações com semáforos */
#include <semaphore.h>
/* Operações com threads */
#include <pthread.h>
/* Símbolos, constantes e tipos Unix */
#include <unistd.h>
/* Funções com tempo */
#include <time.h>

/* Definição do máximo de filósofos a serem criados */
#define MAX_FILOSOFOS 5

/* Enumeração dos estados dos filósofos */
enum
{
    PENSANDO,
    FAMINTO,
    COMENDO
};

/* Estrutura para armazenar o contexto de cada filosofo */
typedef struct ContextoFilosofos
{
    /* Identificadores de estado do filosofo */
    int             Estado, Refeicoes;
    /* Identificador da thread do filosofo*/
    pthread_t       ThreadFilosofo;
    /* Multiplexador do filosofo*/
    sem_t           FilosofoMutex;

} CONTEXTO_FILOSOFO;

/* Identificadores dos filosofos */
typedef struct TipoFilosofo
{
    /* Identificador do filosofo */
    int ID;
    /* Multiplexador do processo */
    sem_t * ProcessoMutex;
    /* Acesso ao contexto de todos filosofos */
    CONTEXTO_FILOSOFO * Contexto;

} FILOSOFO;

/* Pausa para digestão e arrotar */
void Digestao()
{
    /* Cada digestão é aleatoria */
    srand(time(NULL));
    sleep(rand() % 3 + 1);
}

/* Pausa para filosofar: A vida é bela */
void Quatro_E_Vinte()
{
    /* Voa beija-flor */
    srand(time(NULL));
    sleep(rand() % 5 + 1);
}

/* Confere se há disponibilidade de talheres para comer */
void ConferirDisponibilidade(FILOSOFO * Filosofo)
{
    /* Confere estado do filosofo atual, verifica se está faminto e se os talheres não estão sendo utilizados */
    if(((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Estado == FAMINTO && ((FILOSOFO *)Filosofo)->Contexto[(((FILOSOFO *)Filosofo)->ID + MAX_FILOSOFOS - 1) % MAX_FILOSOFOS].Estado != COMENDO && ((FILOSOFO *)Filosofo)->Contexto[(((FILOSOFO *)Filosofo)->ID + 1) % MAX_FILOSOFOS].Estado != COMENDO)
    {
        /* Altera o estado para COMENDO */
        ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Estado = COMENDO;
        /* Incrementa o numero de refeições realizadas */
        ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Refeicoes ++;
        /* Imprime o evento */
        printf("Filosofo [%d] [COMEU %d x] com talheres %d e %d\n",((FILOSOFO *)Filosofo)->ID, ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Refeicoes, ((((FILOSOFO *)Filosofo)->ID + MAX_FILOSOFOS) % MAX_FILOSOFOS), ((FILOSOFO *)Filosofo)->ID + 1);
        /* Dá um tempo pra descer */
        Digestao();
        /* Dá UP no mutex do filosofo */
        sem_post(&(((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].FilosofoMutex));
    }
}

/* Pega talher quando disponível */
void PegarTalher(FILOSOFO * Filosofo)
{
    /* Dá DOWN no mutex do processo */
    sem_wait(((FILOSOFO *)Filosofo)->ProcessoMutex);
    /* Altera o estado para FAMINTO */
    ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Estado = FAMINTO;
    /* Imprime o evento */
    printf("Filosofo [%d] [FAMINTO]\n", ((FILOSOFO *)Filosofo)->ID);
    /* Confere a disponibilidade de dar uma comidinha */
    ConferirDisponibilidade((FILOSOFO *) Filosofo);
    /* Dá UP no mutex do processo */
    sem_post(((FILOSOFO *)Filosofo)->ProcessoMutex);
    /* Dá DOWN no mutex do filosofo */
    sem_wait(&(((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].FilosofoMutex));
}

/* Entrega talher quando acaba de comer */
void EntregarTalher(FILOSOFO * Filosofo)
{
    /* Dá DOWN no mutex do processo */
    sem_wait(((FILOSOFO *)Filosofo)->ProcessoMutex);
    /* Altera o estado para PENSANDO */
    ((FILOSOFO *)Filosofo)->Contexto[((FILOSOFO *)Filosofo)->ID].Estado = PENSANDO;
    /* Imprime o evento */
    printf("Filosofo [%d] [PENSANDO] entrega talheres %d e %d\n",((FILOSOFO *)Filosofo)->ID, ((((FILOSOFO *)Filosofo)->ID + MAX_FILOSOFOS) % MAX_FILOSOFOS), ((FILOSOFO *)Filosofo)->ID + 1);
    /* Confere a oportunidade de fazer uma boquinha */
    ConferirDisponibilidade((FILOSOFO *) Filosofo);
    /* Dá UP no mutex do processo */
    sem_post(((FILOSOFO *)Filosofo)->ProcessoMutex);
}

/* Determina os procedimentos dos filosofos */
void * AcoesFilosofo(void * Filosofo)
{
    /* Non stop nyan cat */
    while(1)
    {
        /* O começo de cada idéia, o filósofo começa a pensar... */
        printf("Filosofo [%d] [PENSANDO]\n", ((FILOSOFO *) Filosofo) -> ID);
        /* Pensamentos profundos */
        Quatro_E_Vinte();
        /* Vamos comer? */
        PegarTalher((FILOSOFO *) Filosofo);
        sleep(0);
        /* Chega de comida. */
        EntregarTalher((FILOSOFO *) Filosofo);
    }
    /* Retorna NULL */
    return NULL;
}

/* É aqui que começa a diversão */
int main(int argc, char * argv[])
{
    /* Variável auxiliar para operações com vetores */
    unsigned char i;

    /* Instancia o semáforo do processo */
    sem_t ProcessoMutex;

    /* Instancia um vetor da estrutura de filosofos */
    FILOSOFO Filosofo[MAX_FILOSOFOS];

    /* Instancia um vetor da estrutura de contexto de filosofos */
    CONTEXTO_FILOSOFO CFilosofo[MAX_FILOSOFOS];

    /* Inicia o multiplexador do processo em UP */
    sem_init(&ProcessoMutex, 0, 1);

    /* Inicia o multiplexador referente a cada thread (filosofo) em DOWN */
    for(i = 0; i < MAX_FILOSOFOS; i++)
        sem_init(&(CFilosofo[i].FilosofoMutex), 0, 0);

    /*
        Cria threads para cada filosofo de acordo com o numero de filosofos MAX_FILOSOFOS
        Define o estado inicial de cada filósofo
    */
    for(i = 0; i < MAX_FILOSOFOS; i++)
    {
        /* Definição dos estados iniciais */
        Filosofo[i].ID = i;
        Filosofo[i].ProcessoMutex = &ProcessoMutex;
        Filosofo[i].Contexto = CFilosofo;
        CFilosofo[i].Estado = 0;
        CFilosofo[i].Refeicoes = 0;
        /* Cria a thread de cada processo */
        pthread_create((&(CFilosofo[i].ThreadFilosofo)), NULL, AcoesFilosofo, &Filosofo[i]);
    }

    /* Faz o processo acompanhar as threads */
    for(i = 0; i < MAX_FILOSOFOS; i++)
        pthread_join(CFilosofo[i].ThreadFilosofo,NULL);

    /* Retorna 0 */
    return 0;
}

/*

    Apenas desperdício de linhas...

*/
