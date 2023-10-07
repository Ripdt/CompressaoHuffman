namespace Lista {

  using namespace std;

  template <typename T>
  struct No {
    T info = NULL;
    No* elo = nullptr;
  };

  template <typename T>
  struct ListaEncadeada {

    No <T> *comeco = nullptr, *fim = nullptr;

    //
    // Mostra todo o conteúdo da lista na tela
    //
    void mostrar() const {
      No <T>* aux = comeco;

      while (aux != nullptr) {
        cout << aux->info << " ";
        aux = aux->elo;
      }
      if (comeco == nullptr) cout << "<Vazio>";
    }

    //
    // Insere o valor na lista dada respeitando a ordem crescente
    //
    bool inserir_C(T valor) {

      No <T>* novo;

      // Criar o novo no
      novo = new No<T>;
      if (novo == nullptr) return false;
      novo->info = valor;
      novo->elo = nullptr;

      // Lista vazia
      if (comeco == nullptr) {
        comeco = novo;
        fim = novo;
        return true;
      }

      // Inserção no comeco
      if (valor <= comeco->info) {
        novo->elo = comeco;
        comeco = novo;
        return true;
      }

      // Inserção no final
      if (valor >= fim->info) {
        fim->elo = novo;
        fim = novo;
        return true;
      }

      // Inserir no meio
      No <T>* aux = comeco;
      while (aux->info < valor && aux->elo->info <= valor)
        aux = aux->elo;

      novo->elo = aux->elo;
      aux->elo = novo;
      return true;
    }

    //
    // Insere o valor na lista dada respeitando a ordem decrescente
    //
    bool inserir_D(T valor) {

      No <T>* novo;

      // Criar o novo no
      novo = new No<T>;
      if (novo == nullptr) return false;
      novo->info = valor;
      novo->elo = nullptr;

      // Lista vazia
      if (comeco == nullptr) {
        comeco = novo;
        fim = novo;
        return true;
      }

      // Inserção no comeco
      if (valor >= comeco->info) {
        novo->elo = comeco;
        comeco = novo;
        return true;
      }

      // Inserção no final
      if (valor <= fim->info) {
        fim->elo = novo;
        fim = novo;
        return true;
      }

      // Inserir no meio
      No <T>* aux = comeco;
      while (aux->info > valor && aux->elo->info >= valor)
        aux = aux->elo;

      novo->elo = aux->elo;
      aux->elo = novo;
      return true;
    }

    //
    // Insere o valor dado sempre no final da lista
    //
    bool inserir(T valor) {
      No <T>* novo;

      // Criar o novo no
      novo = new No<T>;
      if (novo == nullptr) return false;
      novo->info = valor;
      novo->elo = nullptr;

      // Lista vazia
      if (comeco == nullptr) {
        comeco = novo;
        fim = novo;
        return true;
      }

      fim->elo = novo;
      fim = novo;
      return true;
    }

    //
    // Retirar o valor da lista
    // retornar verdadeiro ou false se não achar
    //
    bool retirar(T valor) {
      No <T>* aux = comeco, * anterior = nullptr;

      while (aux != nullptr && aux->info != valor) {
        anterior = aux;
        aux = aux->elo;
      }
      if (aux == nullptr) return false;

      if (aux == comeco) { // Casos 1 e 2
        comeco = comeco->elo;
        if (aux == fim) fim = nullptr;
      }
      else { // Casos 3 e 4
        anterior->elo = aux->elo;
        if (aux == fim) fim = anterior;
      }
      delete aux;
      return true;
    }

    //
    // Libera toda a memória alocada para a lista fornecida
    //
    void liberar() {
      No <T>* aux = comeco;
      No <T>* aux2;

      while (aux != nullptr) {
        aux2 = aux->elo;
        delete aux;
        aux = aux2;
      }
    }

    //
    // Pesquisar se o valor está na lista
    // retornar o endereço do valor ou nulo se não estiver
    //
    No <T>* pesquisar(T valor) {
      No <T>* aux = comeco;

      while (aux != nullptr) {
        if (aux->info == valor) return aux;
        aux = aux->elo;
      }
      return nullptr;
    }

    // 
    // Retornar tamanho da lista
    //
    size_t tamanho() const {
      No <T>* aux = comeco;
      size_t iterador = 0;
      while (aux != nullptr) {
        iterador++;
        aux = aux->elo;
      }
      return iterador;
    }

    //// 
    //// Retornar o endereço do valor na posicao
    ////
    //T em(size_t posicao) const {
    //  No <T>* aux = comeco;
    //  size_t iterador = 0;
    //  while (aux != nullptr) {
    //    if (iterador+1 == posicao)
    //      return aux->info;
    //    iterador++;
    //    aux = aux->elo;
    //  }
    //  return nullptr;
    //}
  };
}

