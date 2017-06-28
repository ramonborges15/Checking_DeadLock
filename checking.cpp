/*
  Trabalho - Detecção de DeadLock
  Alunos: Ramon Borges e Andrey Contadini
  -----------------------------------------
  Informações do Trabalho
  * Operações atômicas:
    - P : decrementa o semáforo, bloqueia thread
    - v : incrementa o semáforo, desbloqueira thread

  Para identação será utilizado o caracter de tabulação '\t'

*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

using namespace std;

typedef struct linha {
  char l[100];
}Linha;

typedef struct thread {
  vector<struct linha> bloco;
}Thread;

typedef struct programa {
  vector<int> semaforo;  //0 ou 1
  struct thread T;
}PROGRAMA;

int ler_arquivo();

//Inicializando lista de semáforos
PROGRAMA *p = (PROGRAMA *) malloc(sizeof(PROGRAMA));

int main(int argc, char const *argv[]) {
  ler_arquivo();

  return 0;
}

int ler_arquivo(){
  FILE *fp;
  char myString[32];

  if ((fp = fopen("arquivo.txt","r")) == NULL) {
    printf("ERRO AO ABRIR O ARQUIVO!\n");
    exit(1);
  }

  //Salva a primeira linha, onde tem-se as informações dos semáforos
  fgets(myString, 32, fp);
  char *pedaco = strtok(myString, " ");

  while (pedaco != NULL) {
    p->semaforo.push_back(atoi(pedaco));
    pedaco = strtok(NULL, " ");
  }

  if (p->semaforo.size() > 16) //Caso tenha-se mais do que 16 semaforos
    return -1;
  else
    return 0;

}
