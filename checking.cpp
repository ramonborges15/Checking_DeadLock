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

typedef struct bloco{
  char linha[100];
}Bloco;

typedef struct thread {
  vector<struct bloco> segmento;
  char nome[100];
}Thread;

typedef struct programa {
  vector<int> semaforo;  //0 ou 1
  vector<struct thread> T;
}PROGRAMA;

int ler_arquivo();
void exibir_estrutura();

//Inicializando lista de semáforos
PROGRAMA *p = (PROGRAMA *) malloc(sizeof(PROGRAMA));

int main(int argc, char const *argv[]) {
  ler_arquivo();

  return 0;
}


int ler_arquivo(){
  FILE *fp;
  char myString[100];
  Bloco aux;

  if ((fp = fopen("arquivo.txt","r")) == NULL) {
    printf("ERRO AO ABRIR O ARQUIVO!\n");
    exit(1);
  }

  //Salva a primeira linha, onde tem-se as informações dos semáforos
  fgets(myString, 100, fp);
  char *pedaco = strtok(myString, " ");

  while (pedaco != NULL) {
    p->semaforo.push_back(atoi(pedaco));
    pedaco = strtok(NULL, " ");
  }

  if (p->semaforo.size() > 16) //Caso tenha-se mais do que 16 semaforos
    return -1;

  int flagThread = 0;
  long int numThreads = 0;
  while(fgets(myString, 100, fp) != NULL) {

    if (strlen(myString) == 1) {
      printf("Entrou\n");
      continue;
    }
    if(!strncmp(myString, "thread", 6)){
      if (flagThread)
        flagThread = 0;
      if(!flagThread){
        flagThread = 1;
        numThreads++;
        p->T.resize(numThreads);

        //salvando nome da thread
        pedaco = strtok(myString, " ");
        pedaco = strtok(NULL, " ");
        strcpy(p->T[numThreads-1].nome, pedaco);
      }

    }
    else{
      //salvando linha
      strcpy(aux.linha, myString);
      p->T[numThreads-1].segmento.push_back(aux);
    }
  }

  exibir_estrutura();
}

void exibir_estrutura(){

  printf("Número de Threads: %lu \n", p->T.size());
  for (int i = 0; i < p->T.size(); i++) {
    printf("%s", p->T[i].nome);
    for (int j = 0; j < p->T[i].segmento.size(); j++) {
      printf("%s", p->T[i].segmento[j].linha);
    }
  }
  return;
}
