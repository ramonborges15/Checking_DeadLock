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
#include <string>
#include <iostream>

using namespace std;

typedef struct bloco{
  string linha;
}Bloco;

typedef struct thread {
  vector<struct bloco> segmento;
  string nome;
}Thread;

typedef struct programa {
  vector<int> semaforo;  //0 ou 1
  vector<struct thread> T;
}PROGRAMA;

//Responsável pelo controle da geração das sequências
typedef struct indices{
	int thr;
	int linha;
}Indices;

//Responsável pelo controle da tabela codificada
typedef struct tabela{
  int pos_thr;
  int pos_linha;
  int valor;
}Tabela;

int ler_arquivo();
void exibir_estrutura();
void gerar_vetor_base(vector<Indices> *base,int numThreads, vector<int> qtdeLinhas);
vector<Indices> gerar_permutacoes(vector<Indices> base, int *flag);
void codificador(vector<Tabela> *tabel_cod);

//Inicializando lista de semáforos
PROGRAMA *p = (PROGRAMA *) malloc(sizeof(PROGRAMA));

int main(int argc, char const *argv[]) {
  vector<Indices> base;
	int numThreads, flag, cont=0;
	vector<int> qtdelinhas;

	//Inicialização
	qtdelinhas.push_back(2);
	qtdelinhas.push_back(2);
	numThreads = 2;

  ler_arquivo();
	gerar_vetor_base(&base, numThreads, qtdelinhas);

	printf("%lu\n", base.size());
	for (int i = 0; i < base.size(); ++i) {
		cout << base[i].thr << base[i].linha << " ";
	}
	cout<< " " <<endl;
	cout<<"--------------------------------------"<<endl;

	while(1){
		base = gerar_permutacoes(base, &flag);
		cout << cont+1 << " - ";
		for (int j = 0; j < base.size(); j++){
			cout << base.at(j).thr << base.at(j).linha << " ";
		}
		cout << " " << endl;
		cont++;
		if(!flag)
			break;
	}

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
        //strcpy(p->T[numThreads-1].nome, pedaco);
        p->T[numThreads-1].nome = pedaco;
      }

    }
    else{
      //salvando linha
      //strcpy(aux.linha, myString);
      aux.linha = myString;
      p->T[numThreads-1].segmento.push_back(aux);
    }
  }

  exibir_estrutura();
}

void exibir_estrutura(){

  printf("Número de Threads: %lu \n", p->T.size());
  for (int i = 0; i < p->T.size(); i++) {
    cout << p->T[i].nome;
    //printf("%s", p->T[i].nome);
    for (int j = 0; j < p->T[i].segmento.size(); j++) {
      cout << p->T[i].segmento[j].linha;
      //printf("%s", p->T[i].segmento[j].linha);
    }
  }
  cout << endl;
  return;
}

void gerar_vetor_base(vector<Indices> *base,int numThreads, vector<int> qtdeLinhas){
	Indices aux;

	for (int i = 0; i < numThreads; i++) {
		for (int j = 0; j < qtdeLinhas[i]; j++) {
			aux.linha = j;
			aux.thr = i;
			base->push_back(aux);
		}
	}

	return;
}

vector<Indices> gerar_permutacoes(vector<Indices> base, int *flag) {
	//Função resposável por gerar apenas uma permutação

	int f = 0;
	vector<Indices> aux;

	for (int i = 0; i < base.size()-1; i++){
		if(base.at(i).thr < base.at(i+1).thr){
			//Realizo a troca
			aux.push_back(base.at(i));
			base[i] = base.at(i+1);
			base[i+1] = aux.at(0);
			f = 1;
			//Deixo a variável sem valor
			aux.pop_back();
			break;
		}
	}
	if(!f) //Significa que não há mais permutas
		*flag = 0;
	else
		*flag = 1;
	return base;
}

void codificador(vector<Tabela> *tabel_cod) {
  Tabela aux;
  size_t found;
  unsigned long cod = 0, valor;
  char *c;

  for (int i = 0; i < p->T.size(); i++) {
    for (int j = 0; j < p->T[i].segmento.size(); j++) {

      found = p->T[i].segmento[j].linha.find("if");
      if (found != std::string::npos) {
        *c = p->T[i].segmento[j].linha[found+2];
        valor = atoi(c);
        aux.valor = 17;
        aux.pos_thr = i;
        aux.pos_linha = j;
        tabel_cod->push_back(aux);
      }

      found = p->T[i].segmento[j].linha.find("else");
      if (found != std::string::npos) {
        *c = p->T[i].segmento[j].linha[found+2];
        valor = atoi(c);
        aux.valor = 18;
        aux.pos_thr = i;
        aux.pos_linha = j;
        tabel_cod->push_back(aux);
      }

      found = p->T[i].segmento[j].linha.find("p");
      if (found != std::string::npos) {
        *c = p->T[i].segmento[j].linha[found+2];
        valor = atoi(c);
        aux.valor = -1*(valor+1);
        aux.pos_thr = i;
        aux.pos_linha = j;
        tabel_cod->push_back(aux);
      }

      found = p->T[i].segmento[j].linha.find("v");
      if (found != std::string::npos) {
        *c = p->T[i].segmento[j].linha[found+2];
        valor = atoi(c);
        aux.valor = (valor+1);
        aux.pos_thr = i;
        aux.pos_linha = j;
        tabel_cod->push_back(aux);
      }
    }
  }
  return;
}
