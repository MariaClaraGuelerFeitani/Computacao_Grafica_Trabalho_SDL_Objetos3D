#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "objeto.h"

//Lê as informações de um arquivo e as carrega num novo objeto alocado
tObjeto3d *carregaObjeto(char *nomeArquivo){
    FILE * arq = fopen(nomeArquivo, "r");

    if(arq == NULL){
        printf("Erro ao abrir o arquivo!");
        return NULL;
    }

    tObjeto3d *obj = malloc(sizeof(tObjeto3d));

    fscanf(arq, "%d", &obj->nPontos);


    //alocando o espaco para as linhas da matriz de vertices (pontos)
    obj->pontos = (float**) malloc(obj->nPontos*sizeof(float*));

    //alocando espaço para as colunas da matriz de vertices (pontos)
    int colunas = 3;

    for(int i=0;i<obj->nPontos;i++){
        obj->pontos[i] = (float *) malloc(colunas * sizeof(float)); //alocando espaço para três colunas por linha
    }


    //preenchendo a matriz
    for(int i=0;i<obj->nPontos;i++){
        for(int j=0;j<colunas;j++){
            fscanf(arq, "%f", &obj->pontos[i][j]);
        }
    }


    fscanf(arq, "%d", &obj->nArestas);


    //alocando o espaco para as linhas da matriz de arestas
    obj->arestas = (int**) malloc(obj->nArestas*sizeof(int*));

    //alocando espaço para as colunas da matriz de vertices (pontos)
    int colunasArestas = 2;

    for(int i=0;i<obj->nArestas;i++){
        obj->arestas[i] = (int *) malloc(colunasArestas * sizeof(int)); //alocando espaço para duas colunas por linha
    }


    //preenchendo a matriz
    for(int i=0;i<obj->nArestas;i++){
        for(int j=0;j<colunasArestas;j++){
            fscanf(arq, "%d", &obj->arestas[i][j]);
        }
    }


    //alocando memória para o vetor de escala
    obj->escala = (float*) malloc(3 * sizeof(float));
    for(int i=0;i<3;i++){
        obj->escala[i] = 1;
    }


    //alocando memória para o vetor de translação
    obj->transl = (float*) malloc(3 * sizeof(float));
    for(int i=0;i<3;i++){
        obj->transl[i] = 0;
    }


    //inicializando as rotações
    obj->Rx = 0;
    obj->Ry = 0;
    obj->Rz = 0;


    //inicializando a ModelMatrix
    int identidade = 4;
    obj->modelMatrix = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        obj->modelMatrix[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(obj->modelMatrix);

    fclose(arq);
    return obj;
}




//Altera a modelMatrix de um objeto para redimenciona-lo segundo os parâmetros escalaX, escalaY e escalaZ
void escalaObjeto(tObjeto3d *objeto, float escalaX, float escalaY, float escalaZ){

    objeto->escala[0] = escalaX;
    objeto->escala[1] = escalaY;
    objeto->escala[2] = escalaZ;

    int identidade = 4;

    float **matEscala = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        matEscala[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(matEscala);
    matEscala[0][0] = objeto->escala[0];
    matEscala[1][1] = objeto->escala[1];
    matEscala[2][2] = objeto->escala[2];

    multMatriz4d(matEscala, objeto->modelMatrix);


    //desalocando memória da matriz auxiliar
    for(int i=0;i<identidade;i++){
        free(matEscala[i]);
    }
    free(matEscala);
}




//Altera a modelMatrix de um objeto para translada-lo segundo os parâmetros transX, transY e transZ
void transladaObjeto(tObjeto3d *objeto, float transX, float transY, float transZ){

    objeto->transl[0] = transX;
    objeto->transl[1] = transY;
    objeto->transl[2] = transZ;

    int identidade = 4;

    float **matTranladada = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        matTranladada[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(matTranladada);
    matTranladada[0][3] = objeto->transl[0];
    matTranladada[1][3] = objeto->transl[1];
    matTranladada[2][3] = objeto->transl[2];

    multMatriz4d(matTranladada, objeto->modelMatrix);


    //desalocando memória da matriz auxiliar
    for(int i=0;i<identidade;i++){
        free(matTranladada[i]);
    }
    free(matTranladada);

}




//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo X segundo o angulo informado
void rotacionaObjetoEixoX(tObjeto3d *objeto, float angulo){

    objeto->Rx = angulo;

    int identidade = 4;

    float **matRotacao = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        matRotacao[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(matRotacao);
    matRotacao[1][1] = cos(objeto->Rx);
    matRotacao[1][2] = -sin(objeto->Rx);
    matRotacao[2][1] = sin(objeto->Rx);
    matRotacao[2][2] = cos(objeto->Rx);

    multMatriz4d(matRotacao, objeto->modelMatrix);


    //desalocando memória da matriz auxiliar
    for(int i=0;i<identidade;i++){
        free(matRotacao[i]);
    }
    free(matRotacao);

}




//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo Y segundo o angulo informado
void rotacionaObjetoEixoY(tObjeto3d *objeto, float angulo){

    objeto->Ry = angulo;

    int identidade = 4;

    float **matRotacao = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        matRotacao[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(matRotacao);
    matRotacao[0][0] = cos(objeto->Ry);
    matRotacao[0][2] = sin(objeto->Ry);
    matRotacao[2][0] = -sin(objeto->Ry);
    matRotacao[2][2] = cos(objeto->Ry);

    multMatriz4d(matRotacao, objeto->modelMatrix);


    //desalocando memória da matriz auxiliar
    for(int i=0;i<identidade;i++){
        free(matRotacao[i]);
    }
    free(matRotacao);

}




//Altera a modelMatrix de um objeto para rotaciona-lo ao redor do eixo Z segundo o angulo informado
void rotacionaObjetoEixoZ(tObjeto3d *objeto, float angulo){

    objeto->Rz = angulo;

    int identidade = 4;

    float **matRotacao = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        matRotacao[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(matRotacao);
    matRotacao[0][0] = cos(objeto->Rz);
    matRotacao[0][1] = -sin(objeto->Rz);
    matRotacao[1][0] = sin(objeto->Rz);
    matRotacao[1][1] = cos(objeto->Rz);

    multMatriz4d(matRotacao, objeto->modelMatrix);


    //desalocando memória da matriz auxiliar
    for(int i=0;i<identidade;i++){
        free(matRotacao[i]);
    }
    free(matRotacao);

}




//Imprime um objeto no terminal
void imprimeObjetoDBG(tObjeto3d *objeto){
    printf("Informacoes Objeto\n");
    printf("Numero de vertices (pontos): %d\n", objeto->nPontos);
    printf("\n");
    printf("Pontos: \n");
    for(int i=0;i<objeto->nPontos;i++){
        printf("X: %f, Y: %f, Z: %f \n", objeto->pontos[i][0], objeto->pontos[i][1], objeto->pontos[i][2]);
    }
    printf("\n");
    printf("Numero de arestas: %d\n", objeto->nArestas);
    printf("\n");
    printf("Arestas: \n");
    for(int i=0;i<objeto->nArestas;i++){
        printf("Primeira: %d, Segunda: %d\n", objeto->arestas[i][0], objeto->arestas[i][1]);
    }
    printf("\n");
    printf("Vetor de translacao: \n");
    for(int i=0;i<3;i++){
        printf("Vet[%d]: %f  ", i, objeto->transl[i]);
    }
    printf("\n");
    printf("\n");
    printf("Vetor de escala: \n");
    for(int i=0;i<3;i++){
        printf("Vet[%d]: %f  ", i, objeto->escala[i]);
    }
    printf("\n");
    printf("\n");
    printf("Rotacao x: %f \n", objeto->Rx);
    printf("Rotacao y: %f \n", objeto->Ry);
    printf("Rotacao z: %f \n", objeto->Rz);
    printf("\n");
    printf("Model Matrix:\n");
    imprimeMatriz4dDBG(objeto->modelMatrix);
}




//Desaloca o objeto
void desalocaObjeto(tObjeto3d *objeto){

    //desalocando os pontos
    for(int i=0;i<objeto->nPontos;i++){
        free(objeto->pontos[i]);
    }
    free(objeto->pontos);


    //desalocando as arestas
    for(int i=0;i<objeto->nArestas;i++){
        free(objeto->arestas[i]);
    }
    free(objeto->arestas);


    //desaloca o vetor de translação
    free(objeto->transl);


    //desaloca o vetor de escala
    free(objeto->escala);


    //desalocando a model matrix
    for(int i=0;i<4;i++){
        free(objeto->modelMatrix[i]);
    }
    free(objeto->modelMatrix);


    //desaloca o objeto
    free(objeto);
}

