#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

using namespace std;

void ler_arquivo();

typedef struct smaforos {
  vector<int> valor_inicial;  //0 ou 1
}SMAFOROS;

//Inicializando lista de semáforos
SMAFOROS *lista_semaforos = (SMAFOROS *) malloc(sizeof(SMAFOROS));

int main(int argc, char const *argv[]) {
  ler_arquivo();
  return 0;
}

void ler_arquivo(){
  FILE *fp;
  char myString[32];

  if ((fp = fopen("arquivo.txt","r")) == NULL) {
    printf("ERRO AO ABRIR O ARQUIVO!\n");
    exit(1);
  }

  //Salva a primeira linha, onde tem-se as informações dos semáforos
  fgets(myString, 32, fp);
  char *pedaco = strtok(myString, " ");

  while (pedaco != '\0') {
    lista_semaforos->valor_inicial.push_back(atoi(pedaco));
    pedaco = strtok(myString, " ");
  }

}
