#include <stdio.h>

//Ajusta valores de um matriz 4d para uma matriz identidade
void criaIdentidade4d(float **novaMatriz);

//Imprime uma matriz 4d no terminal
void imprimeMatriz4dDBG(float **matriz);

//Multiplica uma matriz 4d por um ponto
float *multMatriz4dPonto(float **matriz, float *ponto);

//Multiplica duas matrizes 4d salvando o resultado na segunda matriz
void multMatriz4d(float **matrizA, float **matrizB);

//produto escalar entre dois vetores, retorna o valor do produto escalar
float produtoEscalar(float *vet1, float *vet2);

//produto vetorial entre dois vetores, retorna o vetor resultante
void produtoVetorial(float *vet1, float *vet2, float *resultado);

//normalizar vetor (transformar ele em um vetor unitário)
void normalizarVetor(float *vet);
