#include <iostream>
#include <string>
#include <fstream>

//-------------------------------------------------------------------------------------------------

#include "ListaEncadeada.hpp"
#include "ListaEstatica.hpp"

//-------------------------------------------------------------------------------------------------

#define ASCII_TAM 128 /* https://www.ime.usp.br/~pf/algoritmos/apend/ascii.html */

using namespace std;

//-------------------------------------------------------------------------------------------------

struct No {
  char conteudo = '\0';
  int frequencia = 0;
  No* esq = nullptr;
  No* dir = nullptr;

  static No* novo(const char conteudo, const int frequencia) {
    auto novo = new No;
    novo->conteudo = conteudo;
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

  static void liberar(No* raiz) {
    if (raiz == nullptr) return;
    liberar(raiz->esq);
    liberar(raiz->dir);
    delete raiz;
  }
};

//-------------------------------------------------------------------------------------------------

void executar();
  void processarEscolha(int);

void lerInformacoesDoArquivo(const string&);
  ListaEstatica<string> buscarConteudoDoArquivo(ifstream&);

void lerDadosDoTeclado();

No* montarArvore(const ListaEstatica<string>&);
  void calcularFrequenciasDosCaracteres(const string&, int[]);
  Lista::Encadeada<No*> criarNosDasFrequencias(const int*);
  No* montarArvore(Lista::Encadeada<No*>&);

ListaEstatica<string> comprimirConteudo(const ListaEstatica<string>&, No*);
  void comprimirCaracteres(No*, const string&, string[]);
  ListaEstatica<string> comprimirConteudo(const ListaEstatica<string>&, const string[]);

ListaEstatica<char> descomprimir(const ListaEstatica<string>&, const No*);

//-------------------------------------------------------------------------------------------------

int main()
{
  executar();
  return 0;
}

//-------------------------------------------------------------------------------------------------

void executar() {
  int escolha;
  string nomeArquivo;

  do {
    cout << "MENU:" << endl;
    cout << "1 - Teclado" << endl;
    cout << "2 - Arquivo" << endl;
    cout << "3 - Sair" << endl;
    cout << "Escolha uma opcao: ";
    cin >> escolha;
    system("cls");
    processarEscolha(escolha);
  } while (escolha != 3);
}

void processarEscolha(int escolha) {
  string nomeArquivo;
  switch (escolha) {
  case 1:
    cout << "Você escolheu a opcao Teclado." << endl;
    lerDadosDoTeclado();
    break;

  case 2:
    cout << "Você escolheu a opcao Arquivo." << endl;
    cout << "Digite o nome do arquivo a ser lido: ";
    cin >> nomeArquivo;
    lerInformacoesDoArquivo(nomeArquivo);
    break;

  case 3:
    cout << "Saindo do programa." << endl;
    break;
  default:
    cout << "Opcao invalida. Por favor, escolha uma opcao valida." << endl;
  }
}

//-------------------------------------------------------------------------------------------------

void lerDadosDoTeclado() {
  ofstream arquivo("temp.txt");
  string linha;
  getline(cin, linha); // lixo da stream
  cout << "Digite o que quer comprimir. Escreva \"sair\" para finalizar.\n";
  while (getline(cin, linha) && linha != "sair")
    arquivo << linha + "\n";
  arquivo.close();
  lerInformacoesDoArquivo("temp.txt");
  remove("temp.txt");
}

//-------------------------------------------------------------------------------------------------

void lerInformacoesDoArquivo(const string& nomeArquivo) {
  ifstream arquivo(nomeArquivo);

  if (!arquivo.is_open()) {
    cout << "\n\nNao foi possível abrir o arquivo " << nomeArquivo << "\n\n";
    return;
  }
  ListaEstatica<string> conteudo = buscarConteudoDoArquivo(arquivo);
  cout << "\n\nConteudo:{\n";
  conteudo.imprimir(cout);
  cout << "}";

  No* raiz = montarArvore(conteudo);
  ListaEstatica<string> conteudoComprimido = comprimirConteudo(conteudo, raiz);
  cout << "\n\nConteudo comprimido:{\n";
  conteudoComprimido.imprimir(cout);
  cout << "\n}";

  ListaEstatica<char> conteudoDescomprimido = descomprimir(conteudoComprimido, raiz);
  cout << "\n\nConteudo descomprimido:{\n";
  conteudoDescomprimido.imprimir(cout);
  cout << "}\n\n";

  No::liberar(raiz);

  arquivo.close();
}

ListaEstatica<string> buscarConteudoDoArquivo(ifstream& arquivo) {
  ListaEstatica<string> conteudo;
  string linha;
  while (getline(arquivo, linha)) {
    linha += "\n";
    conteudo.inserir(linha);
  }
  return conteudo;
}

//-------------------------------------------------------------------------------------------------

No* montarArvore(const ListaEstatica<string>& conteudo) {
  int frequencias[ASCII_TAM] = { 0 };
  for (int i = 0; i < conteudo.getTamanho(); i++) {
    string linha = conteudo.getValor(i);
    calcularFrequenciasDosCaracteres(linha, frequencias);
  }
  Lista::Encadeada<No*> nos = criarNosDasFrequencias(frequencias);
  No* raiz = montarArvore(nos);
  nos.liberar();

  return raiz;
}

void calcularFrequenciasDosCaracteres(const string& linha, int frequencias[]) {
  for (char c : linha)
    frequencias[c]++;
}

Lista::Encadeada<No*> criarNosDasFrequencias(const int* frequenciasDeCaracteres) {
  Lista::Encadeada<No*> nos;
  for (int chave = 0; chave < ASCII_TAM; chave++)
    if (frequenciasDeCaracteres[chave] > 0) {
      auto novo = No::novo(chave, frequenciasDeCaracteres[chave]);
      nos.inserir_C(novo);
    }
  return nos;
}

No* montarArvore(Lista::Encadeada<No*>& nos) {
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

//-------------------------------------------------------------------------------------------------

ListaEstatica<string> comprimirConteudo(const ListaEstatica<string>& conteudo, No* arvore) {
  string caracteresComprimidos[ASCII_TAM];
  comprimirCaracteres(arvore, ""/*caractereComprimido*/, caracteresComprimidos);

  return comprimirConteudo(conteudo, caracteresComprimidos);
}

void comprimirCaracteres(No* raiz, const string& caractereComprimido, string comprimidos[]) {
  if (raiz == nullptr)
    return;

  if (raiz->conteudo != '\0')
    comprimidos[raiz->conteudo] = caractereComprimido;

  comprimirCaracteres(raiz->esq, caractereComprimido + "0", comprimidos);
  comprimirCaracteres(raiz->dir, caractereComprimido + "1", comprimidos);
}

ListaEstatica<string> comprimirConteudo(const ListaEstatica<string>& conteudo, const string caracteresComprimidos[ASCII_TAM])
{
  ListaEstatica<string> comprimida;
  for (int i = 0; i < conteudo.getTamanho(); i++) {
    const string linha = conteudo.getValor(i);
    for (const int ascii : linha)
      comprimida.inserir(caracteresComprimidos[ascii]);
  }
  return comprimida;
}

//-------------------------------------------------------------------------------------------------

ListaEstatica<char> descomprimir(const ListaEstatica<string>& comprimida, const No* raiz) {
  ListaEstatica<char> descomprimido;
  for (int i = 0; i < comprimida.getTamanho(); i++) {
    const No* no = raiz;
    string caractereComprimido = comprimida.getValor(i);
    string linhaDescomprimida = "";
    for (const char bit : caractereComprimido) {
      if (no != nullptr)
        no = bit == '0' ? no->esq : no->dir;
    }
    descomprimido.inserir(no->conteudo);
  }
  return descomprimido;
}

//-------------------------------------------------------------------------------------------------