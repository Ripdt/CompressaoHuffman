#include <iostream>
#include "lue.hpp"

#define ASCII_TAM 128 /* https://www.ime.usp.br/~pf/algoritmos/apend/ascii.html */

using namespace std;

struct No {
  char chave = '\0';
  int frequencia = 0;
  No* esq = nullptr;
  No* dir = nullptr;

  static No* novo(const char chave, const int frequencia) {
    auto novo = new No;
    novo->chave = chave;
    novo->frequencia = frequencia;
    return novo;
  }

  bool operator>(const No& b) {
    return frequencia > b.frequencia;
  }

  bool operator<=(const No& b) {
    return frequencia <= b.frequencia;
  }

  bool operator>=(const No& b) {
    return frequencia >= b.frequencia;
  }

  bool operator<(const No& b) {
    return frequencia < b.frequencia;
  }
};

//void ordenarNos(Lista::ListaEncadeada<No>& nos) {
//  size_t menorFrequencia = 1, segundaMenorFrequencia = 2;
//  if (nos.em(1) > nos.em(2)) {
//    const size_t aux = menorFrequencia;
//    menorFrequencia = segundaMenorFrequencia;
//    segundaMenorFrequencia = aux;
//  }
//
//
//  for (size_t i = 2; i < nos.tamanho(); i++) {
//    if (nos.em(menorFrequencia) > nos.em(i)) {
//      segundaMenorFrequencia = menorFrequencia;
//      menorFrequencia = i;
//    }
//    else if (nos.em(segundaMenorFrequencia) > nos.em(i)) {
//      segundaMenorFrequencia = i;
//    }
//  }
//}

void comprimir(No*, string, string[]);

int* calcularFrequencias(const string& linha) {
  static int frequenciasDeCaracteres[ASCII_TAM] = { 0 };
  for (char c : linha)
    frequenciasDeCaracteres[c]++;
  return frequenciasDeCaracteres;
}

Lista::ListaEncadeada<No*>& reunirNosDasFrequencias(const int* frequenciasDeCaracteres) {
  Lista::ListaEncadeada<No*> nos;
  for (int chave = 0; chave < ASCII_TAM; chave++)
    if (frequenciasDeCaracteres[chave] > 0) {
      auto novo = No::novo(chave, frequenciasDeCaracteres[chave]);
      nos.inserir_C(novo);
    }
  return nos;
}

No* montarArvore(Lista::ListaEncadeada<No*>& nos) {
  while (nos.tamanho() != 1) {
    No* esquerda = nos.comeco->info;
    No* direita = nos.comeco->elo->info;
    nos.retirar(esquerda);
    nos.retirar(direita);

    No* novoNo = No::novo('\0', esquerda->frequencia + direita->frequencia);
    novoNo->esq = esquerda;
    novoNo->dir = direita;
    nos.inserir_C(novoNo);
  }

  return nos.comeco->info;
}

string descomprimir(const No* raiz, const string& comprimida) {
  const No* proximoNo = raiz;
  string descomprimida = "";
  for (const char direcao : comprimida) {
    proximoNo = direcao == '0' ? proximoNo->esq : proximoNo->dir;
    if (proximoNo == nullptr) {
      descomprimida += &raiz->chave;
      proximoNo = raiz;
    }
  }
  return descomprimida;
}

void comprimir(const string& linha)
{
  const int* frequencias = calcularFrequencias(linha);
  Lista::ListaEncadeada<No*>& nos = reunirNosDasFrequencias(frequencias);
  No* raiz = montarArvore(nos);

  nos.liberar();

  string caracteresComprimidos[ASCII_TAM];
  comprimir(raiz, "", caracteresComprimidos);

  cout << "Huffman Codes:\n";
  for (int caractere = 0; caractere < ASCII_TAM; caractere++) {
    if (frequencias[caractere] > 0)
      cout << (char)caractere << ": " << caracteresComprimidos[caractere] << "\n";
  }

  string comprimida = "";
  for (char c : linha)
    comprimida += caracteresComprimidos[c];

  cout << descomprimir(raiz, comprimida);




  //for (int caractere = 0; caractere < ASCII_TAM; caractere++)
  //  if (frequencias[caractere] > 0)
  //    cout << descomprimir(raiz, caracteresComprimidos[caractere]);

  //Lista::No<No*>* iterador = nos.comeco;
  //while (iterador != nullptr) {
  //  cout << iterador->info->chave << " -> " << iterador->info->frequencia << endl;
  //  iterador = iterador->elo;
  //}


  //HuffmanNode* root = pq.top();

  //while (nos.tamanho() > 1) {
  //  ordenarNos(nos);
  //}
}

void comprimir(No* raiz, string caractereComprimido, string comprimidos[]) {
  if (raiz == nullptr)
    return;

  if (raiz->chave != '\0')
    comprimidos[raiz->chave] = caractereComprimido;

  comprimir(raiz->esq, caractereComprimido + "0", comprimidos);
  comprimir(raiz->dir, caractereComprimido + "1", comprimidos);
}

int main()
{
  string teste = "teste\n";
  comprimir(teste);
}