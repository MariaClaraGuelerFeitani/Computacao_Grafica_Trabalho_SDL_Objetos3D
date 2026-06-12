#include <stdio.h>
#include <math.h>

#include "algebra.h"

//Ajusta valores de um matriz 4d para uma matriz identidade
void criaIdentidade4d(float **novaMatriz){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(i==j){
                novaMatriz[i][j] = 1;
            }
            else{
                novaMatriz[i][j] = 0;
            }
        }
    }
}




//Imprime uma matriz 4d no terminal
void imprimeMatriz4dDBG(float **matriz){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            printf("%f ", matriz[i][j]);
        }
        printf("\n");
    }
}




//Multiplica uma matriz 4d por um ponto
float *multMatriz4dPonto(float **matriz, float *ponto){

    int identidade = 4;
    float pontoAux[4];

    for(int i=0;i<identidade-1;i++){
        pontoAux[i] = ponto[i];
    }
    pontoAux[identidade-1] = 1;

    float *resultado = (float*) malloc(identidade * sizeof (float));

    for(int i=0;i<identidade;i++){
        resultado[i] = 0;
    }

    for(int i=0;i<identidade;i++){
        for(int j=0;j<identidade;j++){
            resultado[i] += matriz[i][j] * pontoAux[j];
        }
    }

    return resultado;
}




//Multiplica duas matrizes 4d salvando o resultado na segunda matriz
void multMatriz4d(float **matrizA, float **matrizB){
    int identidade = 4;

    float matAux[identidade][identidade];

    for(int i=0;i<identidade;i++){
        for(int j=0;j<identidade;j++){
            matAux[i][j] = 0;
        }
    }

    for(int i=0;i<identidade;i++){
        for(int j=0;j<identidade;j++){
            int cont = 0;
            while(cont<4){
                matAux[i][j] += matrizA[i][cont] * matrizB[cont][j];
                cont++;
            }
        }
    }

    for(int i=0;i<identidade;i++){
        for(int j=0;j<identidade;j++){
            matrizB[i][j] = matAux[i][j];
        }
    }
}




//produto escalar entre dois vetores, retorna o valor do produto escalar
float produtoEscalar(float *vet1, float *vet2){
    float resultado = 0;

    for(int i=0;i<3;i++){
        resultado += (vet1[i] * vet2[i]);
    }

    return resultado;
}




//produto vetorial entre dois vetores, retorna o vetor resultante
void produtoVetorial(float *vet1, float *vet2, float *resultado){
    resultado[0] = vet1[1] * vet2[2] - vet1[2] * vet2[1];
    resultado[1] = vet1[2] * vet2[0] - vet1[0] * vet2[2];
    resultado[2] = vet1[0] * vet2[1] - vet1[1] * vet2[0];
}




//normalizar vetor (transformar ele em um vetor unitário)
void normalizarVetor(float *vet){
    float modulo = 0;

    modulo = sqrt(vet[0] * vet[0] + vet[1] * vet[1] + vet[2] * vet[2]);

    if(modulo == 0){

    }
    else{
        vet[0] = vet[0] / modulo;
        vet[1] = vet[1] / modulo;
        vet[2] = vet[2] / modulo;
    }
}
