//
// Created by bonella on 25/04/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "Lista.h"

// Implementacao de uma lista simplesmente encadeada sem sentinela
// Cada celula ira guardar um pgid
// O pgid sera util para matar um grupo inteiro de processos de uma so vez

struct celula{
    int pgid;
    Celula* proxCel;
};

struct lista{
    Celula* primCel;
    Celula* ultCel;
};

Lista* inicializaLista(){
    Lista* novaLista = (Lista*) malloc(sizeof(Lista));
    if(novaLista == NULL){
        perror("Erro na alocação da lista\n");
        exit(1);
    }

    novaLista->primCel = NULL;
    novaLista->ultCel = NULL;

    return novaLista;
}

int listaVazia(Lista* lista){
    if(lista->primCel == NULL && lista->ultCel == NULL) return 1;
    return 0;
}

void insereLista(Lista* lista, int pgid){
    Celula* novaCelula = (Celula*) malloc(sizeof(Celula));
    if(novaCelula == NULL){
        perror("Erro na alocação da celula\n");
        exit(1);
    }

    novaCelula->pgid = pgid;
    novaCelula->proxCel = NULL;

    if(listaVazia(lista)){
        lista->primCel = lista->ultCel = novaCelula;
    }else{
        lista->ultCel = lista->ultCel->proxCel = novaCelula;
    }
}

void retiraLista(Lista* lista, int pgid){
    Celula* celAux;
    Celula* celAuxAnt = NULL;

    for(celAux = lista->primCel; celAux != NULL; celAux = celAux->proxCel){
        if(celAux->pgid == pgid) break;
        celAuxAnt = celAux;
    }

    if(celAux == NULL) return;

    if(celAux == lista->primCel && celAux == lista->ultCel){ // Lista so tem o elemento
        lista->primCel = NULL;
        lista->ultCel = NULL;
    }else if(lista->primCel == celAux){ // Elemento retirado esta na primeira posicao
        lista->primCel = celAux->proxCel;
    }else if(lista->ultCel == celAux){ // Elemento retirado esta na ultima posicao
        celAuxAnt->proxCel = NULL;
        lista->ultCel = celAuxAnt;
    }else{ // Elemento esta no meio da lista
        celAuxAnt->proxCel = celAux->proxCel;
    }

    free(celAux);
}

// Percorre cada celula da lista e, para cada celula, envia um comando kill para o grupo armazenado nela
void percorreListaMatandoGrupos(Lista* lista){
    Celula* celAux;

    for(celAux = lista->primCel; celAux != NULL; celAux = celAux->proxCel){
        killpg(celAux->pgid,SIGKILL);
    }
}

void imprimeLista(Lista* lista){
    for(Celula* celAux = lista->primCel; celAux != NULL; celAux = celAux->proxCel){
        printf("%d ", celAux->pgid);
    }
    printf("\n");
}

void liberaLista(Lista* lista){
    Celula* celAux1 = lista->primCel;
    Celula* celAux2;

    while(celAux1 != NULL){
        celAux2 = celAux1->proxCel;
        free(celAux1);
        celAux1 = celAux2;
    }

    free(lista);
}
