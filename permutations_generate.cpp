#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace std;

typedef struct indices{
	int thr;
	int linha;
}Indices;

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

int main(int argc, char const *argv[]) {
	vector<Indices> base;
	int numThreads, flag, cont=0;
	vector<int> qtdelinhas;

	//Inicialização
	qtdelinhas.push_back(5);
	qtdelinhas.push_back(2);
	qtdelinhas.push_back(3);
	numThreads = 3;

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
