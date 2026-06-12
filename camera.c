#include <stdio.h>
#include <SDL2/SDL.h>

#include "camera.h"
#include "algebra.h"

//Aloca a estrutura de memória para representar a câmera
tCamera3d *criaCamera(){

    tCamera3d *camera = malloc(sizeof(tCamera3d));

    //alocando memória para o vetor posição
    camera->posicao = (float*) malloc(3 * sizeof(float));
    camera->posicao[0] = 0;
    camera->posicao[1] = 0;
    camera->posicao[2] = -5;


    //alocando memória para o vetor foco
    camera->foco = (float*) malloc(3 * sizeof(float));
    camera->foco[0] = 0;
    camera->foco[1] = 0;
    camera->foco[2] = 0;


    //alocando memória para o vetor cima
    camera->cima = (float*) malloc(3 * sizeof(float));
    camera->cima[0] = 0;
    camera->cima[1] = 1;
    camera->cima[2] = 0;


    //alocando espaço para a matriz viewMatrix
    int identidade = 4;
    camera->viewMatrix = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        camera->viewMatrix[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(camera->viewMatrix);

    return camera;

}




//Recebe os parâmetros específicos da câmera e calcula viewMatrix
void defineCamera(tCamera3d *camera, float posX, float posY, float posZ, float focX, float focY, float focZ, float cimX, float cimY, float cimZ){

    //definindo a posição
    camera->posicao[0] = posX;
    camera->posicao[1] = posY;
    camera->posicao[2] = posZ;

    //definindo o foco
    camera->foco[0] = focX;
    camera->foco[1] = focY;
    camera->foco[2] = focZ;

    //definindo cima
    camera->cima[0] = cimX;
    camera->cima[1] = cimY;
    camera->cima[2] = cimZ;


   float z[3];
   z[0] = camera->posicao[0] - camera->foco[0];
   z[1] = camera->posicao[1] - camera->foco[1];
   z[2] = camera->posicao[2] - camera->foco[2];
   normalizarVetor(z);


   float produtoEsc;
   produtoEsc = produtoEscalar(z, camera->cima);


   float y[3];
   y[0] = camera->cima[0] - produtoEsc * z[0];
   y[1] = camera->cima[1] - produtoEsc * z[1];
   y[2] = camera->cima[2] - produtoEsc * z[2];
   normalizarVetor(y);


   float x[3];
   produtoVetorial(y, z, x);
   normalizarVetor(x);


   //criando a matriz de rotação
   int identidade = 4;

    float **matRotacao = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        matRotacao[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(matRotacao);
    matRotacao[0][0] = x[0];
    matRotacao[1][0] = y[0];
    matRotacao[2][0] = z[0];

    matRotacao[0][1] = x[1];
    matRotacao[1][1] = y[1];
    matRotacao[2][1] = z[1];

    matRotacao[0][2] = x[2];
    matRotacao[1][2] = y[2];
    matRotacao[2][2] = z[2];



    //criando a matriz de translacao
    float **matTranslacao = (float**) malloc(identidade * sizeof(float*));

    for(int i=0;i<identidade;i++){
        matTranslacao[i] = (float*) malloc(identidade * sizeof(float));
    }

    criaIdentidade4d(matTranslacao);
    matTranslacao[0][3] = - camera->posicao[0];
    matTranslacao[1][3] = - camera->posicao[1];
    matTranslacao[2][3] = - camera->posicao[2];


    criaIdentidade4d(camera->viewMatrix);
    multMatriz4d(matTranslacao, camera->viewMatrix);
    multMatriz4d(matRotacao, camera->viewMatrix);


    //desalocando matriz rotação
    for(int i=0;i<identidade;i++){
        free(matRotacao[i]);
    }
    free(matRotacao);

    //desalocando matriz translação
    for(int i=0;i<identidade;i++){
        free(matTranslacao[i]);
    }
    free(matTranslacao);

}




//Desaloca a câmera
void desalocaCamera(tCamera3d *objeto){

    //desaloca memória para o vetor posição
    free(objeto->posicao);


    //desaloca memória para o vetor foco
    free(objeto->foco);


    //desaloca memória para o vetor cima
    free(objeto->cima);


    //desaloca espaço para a matriz viewMatrix
    for(int i=0;i<4;i++){
        free(objeto->viewMatrix[i]);
    }
    free(objeto->viewMatrix);


    //desaloca a camera
    free(objeto);

}
