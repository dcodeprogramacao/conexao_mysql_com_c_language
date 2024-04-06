struct lista {
    void* item;
    struct lista* proximo;
};

typedef struct lista Lista;

int count(Lista* p_lista) {
    int contador = 0;

    for (Lista* aux = p_lista; 
         aux != NULL; 
         aux = aux->proximo) {
        contador++;
    }
    
    return contador;
}

Lista* last(Lista* p_lista) {
    Lista* aux = p_lista;

    if (aux == NULL) {
        aux = (Lista*) malloc(sizeof(Lista));
        aux->proximo = NULL;
    }

    while (aux->proximo != NULL) {
        aux = aux->proximo;
    }

    return aux;    
}

Lista* info(Lista* p_lista, int index) {
    Lista* aux = p_lista;
    int tamanho = count(aux);

    if (index > tamanho-1) {
        printf("Posição é inválida");
        return NULL;
    } else {
        for (int i = 0; i < index; i++){
            aux = aux->proximo;
        }

        return aux;
    }
}

Lista* add(Lista* p_lista, void* p_item) {
    Lista* aux = p_lista;
    Lista* novo;

    if (aux == NULL) {
        aux = (Lista*) malloc(sizeof(Lista));

        aux->item = p_item;
        aux->proximo = NULL;
        return aux;
    } else {
        aux = last(p_lista);
        novo = (Lista*) malloc(sizeof(Lista));

        novo->item = p_item;
        novo->proximo = NULL;
        aux->proximo = novo;
        return p_lista;
    }
}

Lista* insert(Lista* p_lista, void* p_item, int index) {
    int tamanho = count(p_lista);
    Lista* aux = info(p_lista, index);

    if (aux != NULL) {
        Lista* novo = (Lista*) malloc(sizeof(Lista));

        novo->item = aux->item;
        aux->item = p_item;
        aux->proximo = novo;
    }

    return p_lista;
}

Lista* delete(Lista* p_lista, int index) {
    Lista* aux = info(p_lista, index);

    if (aux != NULL) {
        if (index == 0) {
            return aux->proximo;
        }

        Lista* ant = info(p_lista, index-1);
        ant->proximo = aux->proximo;
    }

    return p_lista;
}