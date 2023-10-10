#include "ListaEncadeada.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

#define ASCII_TAM                                                              \
  128 /* https://www.ime.usp.br/~pf/algoritmos/apend/ascii.html */

using namespace std;

struct No {
  char chave = '\0';
  int frequencia = 0;
  No *esq = nullptr;
  No *dir = nullptr;

  static No *novo(const char chave, const int frequencia) {
    auto novo = new No;
    novo->chave = chave;
    novo->frequencia = frequencia;
    return novo;
  }

  bool operator>(const No &b) { return frequencia > b.frequencia; }

  bool operator<=(const No &b) { return frequencia <= b.frequencia; }

  bool operator>=(const No &b) { return frequencia >= b.frequencia; }

  bool operator<(const No &b) { return frequencia < b.frequencia; }
};

// void ordenarNos(Lista::ListaEncadeada<No>& nos) {
//   size_t menorFrequencia = 1, segundaMenorFrequencia = 2;
//   if (nos.em(1) > nos.em(2)) {
//     const size_t aux = menorFrequencia;
//     menorFrequencia = segundaMenorFrequencia;
//     segundaMenorFrequencia = aux;
//   }
//
//
//   for (size_t i = 2; i < nos.tamanho(); i++) {
//     if (nos.em(menorFrequencia) > nos.em(i)) {
//       segundaMenorFrequencia = menorFrequencia;
//       menorFrequencia = i;
//     }
//     else if (nos.em(segundaMenorFrequencia) > nos.em(i)) {
//       segundaMenorFrequencia = i;
//     }
//   }
// }

void comprimir(No *, string, string[]);

int *calcularFrequencias(const string &linha) {
  static int frequenciasDeCaracteres[ASCII_TAM] = {0};
  for (char c : linha)
    frequenciasDeCaracteres[c]++;
  return frequenciasDeCaracteres;
}

Lista::ListaEncadeada<No *> &
reunirNosDasFrequencias(const int *frequenciasDeCaracteres) {
  Lista::ListaEncadeada<No *> nos;
  for (int chave = 0; chave < ASCII_TAM; chave++)
    if (frequenciasDeCaracteres[chave] > 0) {
      auto novo = No::novo(chave, frequenciasDeCaracteres[chave]);
      nos.inserir_C(novo);
    }
  return nos;
}

No *montarArvore(Lista::ListaEncadeada<No *> &nos) {
  while (nos.tamanho() != 1) {
    No *esquerda = nos.comeco->info;
    No *direita = nos.comeco->elo->info;
    nos.retirar(esquerda);
    nos.retirar(direita);

    No *novoNo = No::novo('\0', esquerda->frequencia + direita->frequencia);
    novoNo->esq = esquerda;
    novoNo->dir = direita;
    nos.inserir_C(novoNo);
  }

  return nos.comeco->info;
}

string descomprimir(const No *raiz, const string &comprimida) {
  const No *proximoNo = raiz;
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

void comprimir(const string &linha) {
  const int *frequencias = calcularFrequencias(linha);
  Lista::ListaEncadeada<No *> &nos = reunirNosDasFrequencias(frequencias);
  No *raiz = montarArvore(nos);

  nos.liberar();

  string caracteresComprimidos[ASCII_TAM];
  comprimir(raiz, "", caracteresComprimidos);

  cout << "Huffman Codes:\n";
  for (int caractere = 0; caractere < ASCII_TAM; caractere++) {
    if (frequencias[caractere] > 0)
      cout << (char)caractere << ": " << caracteresComprimidos[caractere]
           << "\n";
  }

  string comprimida = "";
  for (char c : linha)
    comprimida += caracteresComprimidos[c];

  cout << descomprimir(raiz, comprimida);
}

void comprimir(No *raiz, string caractereComprimido, string comprimidos[]) {
  if (raiz == nullptr)
    return;

  if (raiz->chave != '\0')
    comprimidos[raiz->chave] = caractereComprimido;

  comprimir(raiz->esq, caractereComprimido + "0", comprimidos);
  comprimir(raiz->dir, caractereComprimido + "1", comprimidos);
}

void lerDadosDoTeclado() {
  string linha;

  while (getline(cin, linha)) {
    comprimir(linha);
    cout << "\n";
  }
}

void lerInformacoesDoArquivo(const string &nomeArquivo) {
  ifstream arquivo(nomeArquivo);
  vector<string> linhas;

  if (!arquivo.is_open()) {
    cout << "Não foi possível abrir o arquivo " << nomeArquivo << endl;
    return; // Retorna um vetor vazio se não conseguir abrir o arquivo
  }

  string linha;
  while (getline(arquivo, linha)) {
    stringstream ss(linha);
    string palavra;

    while (ss >> palavra) {
      comprimir(palavra);
    }
  }

  arquivo.close();
}

void exibirMenu() {
  int escolha;
  string nomeArquivo;

  do {
    cout << "MENU:" << endl;
    cout << "1 - Teclado" << endl;
    cout << "2 - Arquivo" << endl;
    cout << "3 - Sair" << endl;
    cout << "Escolha uma opção: ";
    cin >> escolha;

    switch (escolha) {
    case 1:
      cout << "Você escolheu a opção Teclado." << endl;
      lerDadosDoTeclado();

      break;
    case 2:
      cout << "Você escolheu a opção Arquivo." << endl;
      cout << "Digite o nome do arquivo a ser lido: ";
      cin >> nomeArquivo;
      lerInformacoesDoArquivo(nomeArquivo);
      break;
    case 3:
      cout << "Saindo do programa." << endl;
      break;
    default:
      cout << "Opção inválida. Por favor, escolha uma opção válida." << endl;
    }
  } while (escolha != 3);
}

int main() {
  exibirMenu();

  string teste = "teste";
  comprimir(teste);
}
