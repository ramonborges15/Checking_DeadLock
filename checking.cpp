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


int ler_arquivo();
void exibir_estrutura();
void gerar_vetor_base(vector<Indices> *base,int numThreads, vector<int> qtdeLinhas);
vector<Indices> gerar_permutacao(vector<Indices> base, int *flag);
int identificando_deadlock(vector<Indices> *base, int numThreads, vector<int> qtdelinhas, int *ponto_de_parada);
int varrendo_vetor(vector<int> v);

//Inicializando lista de semáforos
PROGRAMA *p = (PROGRAMA *) malloc(sizeof(PROGRAMA));

int main(int argc, char const *argv[]) {
  //Declaração de Variáveis
  vector<Indices> base;
	int numThreads, flag, cont=0, resultado, pparada, x,y,q;
	vector<int> qtdelinhas;

  //Inicialização
  ler_arquivo();

  //Inicializando valores necessários para funcão gerar vetor base
  for (int i = 0; i < p->T.size(); ++i) {
    qtdelinhas.push_back(p->T[i].segmento.size());
  }
  numThreads = p->T.size();

  //Execução da função que identifica deadlock
  resultado = identificando_deadlock(&base, numThreads, qtdelinhas, &pparada);

  //Exibição da Informação do Vetor Base
  cout << "Vetor Base: " <<endl;
  printf("%lu\n", base.size());
	for (int i = 0; i < base.size(); ++i) {
		cout << base[i].thr << base[i].linha << " ";
	}
	cout<< " " <<endl;
	cout<<"--------------------------------------"<<endl;

  //Exibindo informação do resultado sobre impasses
  if(resultado) {
    //Caso ocorra DeadLock
    cout << "DeadLock :(" <<endl;
    cout<< "Sequência de impasse: "<<endl;
    for (int i = 0; i <= pparada; i++) {
      x = base.at(i).thr;
      y = base.at(i).linha;
      cout<< p->T.at(x).nome << ":" << p->T[x].segmento.at(y).linha;
    }
    cout <<endl;
  }
  else {
    //Caso não ocorra
    cout << "Programa livre de impasses." <<endl;
  }

  return 0;
}


int ler_arquivo(){
  FILE *fp;
  char myString[100];
  Bloco aux;
  int q;
  string s;

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

        //Manobra com objetivo de apagar \n ao final do nome da thread
        s = pedaco;
        s.erase(s.length()-1);
        //Salvando nome
        p->T[numThreads-1].nome = s;
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
    cout << p->T[i].nome << ":" <<endl;
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

vector<Indices> gerar_permutacao(vector<Indices> base, int *flag) {
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

int identificando_deadlock(vector<Indices> *base, int numThreads, vector<int> qtdelinhas, int *ponto_de_parada) {
  /*  O que devo fazer nesta função?
      1 - Gerar este vetor base(Notando que preciso passar apenas a referência deste).
      2 - Gerar as permutações
      3 - Para cada permutação gerada é necessário testar se ela resulta em deadlock ou não.
        3.1 - Criar uma função que verifica se todos os elementos do vetor são 0.
            3.1.1 - Se sim 1, senão 0.
  */

  //Declaração das variáveis
  int continuar = 1, t, lin, todos_semaforos_bloqueados;
  size_t found;
  unsigned long cod = 0, id_semaforo;
  char *c;
  vector<int> semaforo_copy;

  //Área responsável para gerar vetor
  gerar_vetor_base(base, numThreads, qtdelinhas);


  while(continuar) {
    cout << "**Resultado dos Semáforos: "<<endl;
    //Fazendo Cópia do estado inicial dos semáforos
    for (int k = 0; k < p->semaforo.size(); k++)
      semaforo_copy.push_back(p->semaforo.at(k));

    //Responsável por testar todas as linhas presentes no vetor base
    for (int j = 0; j < base->size(); j++) {
      *ponto_de_parada = j;
      t = base->at(j).thr;
      lin = base->at(j).linha;

      found = p->T[t].segmento[lin].linha.find("if");
      if (found != std::string::npos) {
        //Faz nada
      }

      found = p->T[t].segmento[lin].linha.find("else");
      if (found != std::string::npos) {
        //Faz nada
      }

      found = p->T[t].segmento[lin].linha.find("p");
      if (found != std::string::npos) {
        *c = p->T[t].segmento[lin].linha[found+2];
        id_semaforo = atoi(c);

        if(semaforo_copy.at(id_semaforo) == 0){ //O caso do recurso estar bloqueado
          semaforo_copy[id_semaforo]--;
          todos_semaforos_bloqueados = varrendo_vetor(semaforo_copy);
          if(todos_semaforos_bloqueados)
            return 1; //Ou seja, houve deadlock.
        }
        else //senão eu apenas decremento a posição do semáforo.
          semaforo_copy[id_semaforo]--;
      }

      found = p->T[t].segmento[lin].linha.find("v");
      if (found != std::string::npos) {
        *c = p->T[t].segmento[lin].linha[found+2];
        id_semaforo = atoi(c);

        if(!semaforo_copy[id_semaforo]) //se for 0
          semaforo_copy[id_semaforo]++;

        //O caso de tentar dar v em um semaforo com 1. Analisar isso!

      }
      for (int i = 0; i < semaforo_copy.size(); i++)
        cout << semaforo_copy.at(i);
      cout<< endl;
    }

    //Área responsável por gerar permutação
    *base = gerar_permutacao(*base, &continuar);

    semaforo_copy.clear();
  }
  return 0;
}

int varrendo_vetor(vector<int> v) {
  for (int i = 0; i < v.size(); i++) {
    if(v.at(i) >= 0)
      return 0; //o vetor possui valores maiores que -1.
  }
  return 1;
}
