#include <iostream>

using namespace std;

#define MAXTAM 50

template <typename T>
struct ListaEstatica {

  T vetor[MAXTAM];
  int ultimo = -1;

  bool inserir(T valor) {
    int pos;
    if (ultimo == MAXTAM - 1) return false;
    ultimo++;
    vetor[ultimo] = valor;
    return true;
  }

  int pesquisar(const T& valor) const {
    for (int i = 0; i <= ultimo; i++)
      if (vetor[i] == valor) return i;
    return -1;
  }

  bool retirar(const T& valor) {
    int pos = pesquisar(valor);

    if (pos == -1) return false;
    retirar(pos);
  }

  bool retirar(int pos) {
    if (pos < 0 || pos > ultimo) return false;

    for (int i = pos; i < ultimo; i++)
      vetor[i] = vetor[i + 1];
    ultimo--;
    return true;
  }

  int getTamanho() const {
    return ultimo + 1;
  }

  T getValor(int pos) const {
    return vetor[pos];
  }

  void liberar() {
    while (getTamanho() - 1);
  }

  void imprimir(ostream& out) {
    for (int i = 0; i < getTamanho(); i++)
      out << getValor(i);
  }
};
