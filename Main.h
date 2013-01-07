/*

    O Incrível Jantar dos Filósofos

    Copyright (C) 2013

    Authors:
    Daniel Garcia <contato@danielgarciaweb.com>
    Felipe Aires <felipcool@gmail.com>
    Kamila Nogueira <kamilabnogueira@gmail.com>

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

/* Assinaturas */
void Digestao();
void Quatro_E_Vinte();
void ConferirDisponibilidade(FILOSOFO * Filosofo);
void PegarTalher(FILOSOFO * Filosofo);
void EntregarTalher(FILOSOFO * Filosofo);
void * AcoesFilosofo(void * Filosofo);


/*

    Apenas desperdício de linhas...

*/
