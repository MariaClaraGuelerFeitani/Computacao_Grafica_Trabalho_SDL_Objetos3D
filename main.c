#include <stdio.h>
#include <SDL2/SDL.h>
#include "objeto.h"
#include "tela.h"
#include "algebra.h"
#include "camera.h"

//desenha um objeto na tela
void desenhaObjetoTela(SDL_Renderer *renderer, float **matriz, tObjeto3d *objeto){
    //alocando o espaço para uma matriz com os vértices transfomados
    float **matrizAlterada = (float**) malloc(objeto->nPontos * sizeof(float*));

    for(int i=0;i<objeto->nPontos;i++){
        //dentro de multMatriz4dPonto a alocação do espaço para cada coluna está sendo feita
        matrizAlterada[i] = multMatriz4dPonto(matriz, objeto->pontos[i]);
    }


    //desenhando as arestas
    for(int i=0; i<objeto->nArestas; i++){
        //desenhando as arestas
        desenhaArestaTela(renderer, matrizAlterada[objeto->arestas[i][0]], matrizAlterada[objeto->arestas[i][1]]);

    }


    //liberando o espaço da matriz
    for(int i=0;i<objeto->nPontos;i++){
        free(matrizAlterada[i]);
    }
    free(matrizAlterada);

}




int mouseSobreObjeto(tObjeto3d *obj, float **mat, int xmouse, int ymouse){
    //alocando o espaço para uma matriz com os vértices transfomados
    float **matrizAlterada = (float**) malloc(obj->nPontos * sizeof(float*));

    for(int i=0;i<obj->nPontos;i++){
        //dentro de multMatriz4dPonto a alocação do espaço para cada coluna está sendo feita
        matrizAlterada[i] = multMatriz4dPonto(mat, obj->pontos[i]);
    }

    float xmax = matrizAlterada[0][0];
    float xmin = matrizAlterada[0][0];
    float ymax = matrizAlterada[0][1];
    float ymin = matrizAlterada[0][1];

    //xmax
    for(int i=0;i<obj->nPontos;i++){
        if(xmax <= matrizAlterada[i][0]){
            xmax = matrizAlterada[i][0];
        }
    }

    //xmin
    for(int i=0;i<obj->nPontos;i++){
        if(xmin >= matrizAlterada[i][0]){
            xmin = matrizAlterada[i][0];
        }
    }

    //ymax
    for(int i=0;i<obj->nPontos;i++){
        if(ymax <= matrizAlterada[i][1]){
            ymax = matrizAlterada[i][1];
        }
    }

    //ymin
    for(int i=0;i<obj->nPontos;i++){
        if(ymin >= matrizAlterada[i][1]){
            ymin = matrizAlterada[i][1];
        }
    }

    //conversão para ficar compatível com a tela
    int xmaxTela = (xmax + 1) * WIDTH / 2;
    int xminTela = (xmin + 1) * WIDTH / 2;
    int ymaxTela = (1 - ymin) * HEIGHT / 2;
    int yminTela = (1 - ymax) * HEIGHT / 2;


    //liberando o espaço da matriz
    for(int i=0;i<obj->nPontos;i++){
        free(matrizAlterada[i]);
    }
    free(matrizAlterada);


    if(xmouse >= xminTela && xmouse <= xmaxTela && ymouse >= yminTela && ymouse <= ymaxTela){
        return 1;
    }
    else{
        return 0;
    }

}




int main( int argc, char * argv[] ){

    if (SDL_Init( SDL_INIT_EVERYTHING) < 0){
        printf("SDL não inicializou! SDL Erro: %s\n", SDL_GetError());
    }

    SDL_Window *window = criaTela("Hello SDL World!");

    if(window == NULL){
        printf("SDL não criou a janela! SDL Erro: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Event windowEvent;




    //Carregando objetos
    tObjeto3d *obj1;
    obj1 = carregaObjeto("cubo.dcg");
    escalaObjeto(obj1, 0.05, 0.05, 0.05);
    rotacionaObjetoEixoX(obj1, 0.2);
    rotacionaObjetoEixoY(obj1, 0.2);
    rotacionaObjetoEixoZ(obj1, 0.2);
    transladaObjeto(obj1, -0.6, 0.5, 0.1);
    //imprimeObjetoDBG(obj1);


    tObjeto3d *obj2;
    obj2 = carregaObjeto("piramide.dcg");
    escalaObjeto(obj2, 0.05, 0.05, 0.05);
    rotacionaObjetoEixoX(obj2, -0.25);
    rotacionaObjetoEixoY(obj2, 1.2);
    rotacionaObjetoEixoZ(obj2, 1.3);
    transladaObjeto(obj2, 0.6, -0.5, 0.1);




    //Carregando a câmera
    tCamera3d *camera;

    camera = criaCamera();
    float posx;
    float posy;
    float posz;
    float focox;
    float focoy;
    float focoz;
    float cimax;
    float cimay;
    float cimaz;

    posx = 0.0;
    posy = 0.0;
    posz = -5.0;
    focox = 0.0;
    focoy = 0.0;
    focoz = 0.0;
    cimax = 0.0;
    cimay = 1.0;
    cimaz = 0.0;
    defineCamera(camera, posx, posy, posz, focox, focoy, focoz, cimax, cimay, cimaz);



    //Matriz Final Objeto 1
        int identidade = 4;
        float **matFinal1 = (float**) malloc(identidade * sizeof(float*));

        for(int i=0;i<identidade;i++){
            matFinal1[i] = (float*) malloc(identidade * sizeof(float));
        }
        criaIdentidade4d(matFinal1);



    //Matriz Final Objeto 2
        float **matFinal2 = (float**) malloc(identidade * sizeof(float*));

        for(int i=0;i<identidade;i++){
            matFinal2[i] = (float*) malloc(identidade * sizeof(float));
        }
        criaIdentidade4d(matFinal2);



    printf("Se a seta estiver em cima de algum objeto, ele se movera. Se a seta nao estiver em cima de nenhum objeto, os dois se moverao\n");
    printf("\n");
    printf("MENU TECLADO\n");
    printf("- Clique na seta da esquerda para mover o objeto para a esquerda\n");
    printf("- Clique na seta da direita para mover o objeto para a direita\n");
    printf("- Clique na seta para cima para mover o objeto para cima\n");
    printf("- Clique na seta para baixo para mover o objeto para baixo\n");
    printf("- Clique no sinal de + para aumentar o tamanho do objeto\n");
    printf("- Clique no sinal de - para diminuir o tamanho do objeto\n");
    printf("- Clique na tecla a para rotacionar o eixo x do objeto\n");
    printf("- Clique na tecla b para rotacionar o eixo y do objeto\n");
    printf("- Clique na tecla c para rotacionar o eixo z do objeto\n");
    printf("\n");
    printf("Clique na tecla d para mover a posicao da camera no eixo x\n");
    printf("Clique na tecla e para mover a posicao da camera no eixo y\n");
    printf("Clique na tecla f para mover a posicao da camera no eixo z\n");
    printf("Clique na tecla g para mover o foco da camera no eixo x\n");
    printf("Clique na tecla h para mover o foco da camera no eixo y\n");
    printf("Clique na tecla i para mover o foco da camera no eixo z\n");
    printf("Clique na tecla j para mover o cima da camera\n");
    printf("\n");
    printf("MENU MOUSE\n");
    printf("- Clique no botao esquerdo do mouse para rotacionar o eixo x do objeto\n");
    printf("- Clique no botao direito do mouse para rotacionar o eixo y do objeto\n");
    printf("- Clique no botao do meio do mouse para rotacionar o eixo z do objeto\n");
    printf("- Rode a bolinha do mouse para cima para aumentar o tamanho do objeto\n");
    printf("- Rode a bolinha do mouse para baixo para diminuir o tamanho do objeto\n");


    criaIdentidade4d(matFinal1);
    multMatriz4d(obj1->modelMatrix, matFinal1);
    multMatriz4d(camera->viewMatrix, matFinal1);


    criaIdentidade4d(matFinal2);
    multMatriz4d(obj2->modelMatrix, matFinal2);
    multMatriz4d(camera->viewMatrix, matFinal2);


    while(1){
        if( SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);





        //ESCREVA AQUI O SEU PROGRAMA


        int xmouse, ymouse;
        //captura a localização do mouse na tela
        if(windowEvent.type == SDL_MOUSEMOTION){
            SDL_GetMouseState(&xmouse, &ymouse);
            //printf("Mouse :: %3d %3d\n", xmouse, ymouse);
        }


        //OBJETO 1
        if(mouseSobreObjeto(obj1, matFinal1, xmouse, ymouse)){
        //SDL_KEYDOWN captura o clicar em uma tecla
        if(windowEvent.type == SDL_KEYDOWN){
            if(windowEvent.key.keysym.sym == SDLK_LEFT){
                transladaObjeto(obj1, -0.1, 0, -0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_RIGHT){
                transladaObjeto(obj1, 0.1, 0, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_UP){
                transladaObjeto(obj1, 0, 0.1, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_DOWN){
                transladaObjeto(obj1, 0, -0.1, -0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_EQUALS){
                escalaObjeto(obj1, 1.1, 1.1, 1.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_MINUS){
                escalaObjeto(obj1, 0.9, 0.9, 0.9);
            }
            else if(windowEvent.key.keysym.sym == SDLK_a){
                rotacionaObjetoEixoX(obj1, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_b){
                rotacionaObjetoEixoY(obj1, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_c){
                rotacionaObjetoEixoZ(obj1, 0.1);
            }
        }

        //SDL_KEYUP captura o soltar uma tecla
        if(windowEvent.type == SDL_KEYUP){
            if(windowEvent.key.keysym.sym == SDLK_LEFT){
                printf("Seta da esquerda liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_RIGHT){
                printf("Seta da direita liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_UP){
                printf("Seta para cima liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_DOWN){
                printf("Seta para baixo liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_EQUALS){
                printf("Seta + liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_MINUS){
                printf("Seta - liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_a){
                printf("Seta a liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_b){
                printf("Seta b liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_c){
                printf("Seta c liberada\n");
            }
        }

        //SDL_MOUSEBUTTONDOWN captura o clicar um botão do mouse
        if(windowEvent.type == SDL_MOUSEBUTTONDOWN){
            if(windowEvent.button.button == SDL_BUTTON_LEFT){
                rotacionaObjetoEixoX(obj1, 0.1);
            }
            else if(windowEvent.button.button == SDL_BUTTON_RIGHT){
                rotacionaObjetoEixoY(obj1, 0.1);
            }
            else if(windowEvent.button.button == SDL_BUTTON_MIDDLE){
                rotacionaObjetoEixoZ(obj1, 0.1);
            }
        }

        //SDL_MOUSEBUTTONUP captura o soltar um botão do mouse
        if(windowEvent.type == SDL_MOUSEBUTTONUP){
            if(windowEvent.button.button == SDL_BUTTON_LEFT){
                printf("Botao esquerdo do mouse liberado\n");
            }
            else if(windowEvent.button.button == SDL_BUTTON_RIGHT){
                printf("Botao direito do mouse liberado\n");
            }
            else if(windowEvent.button.button == SDL_BUTTON_MIDDLE){
                printf("Botao do meio do mouse liberado\n");
            }
        }

        //SDL_MOUSEWHEEL captura o movimento no botão do meio do mouse
        if(windowEvent.type == SDL_MOUSEWHEEL){
            if(windowEvent.wheel.y > 0){
                escalaObjeto(obj1, 1.1, 1.1, 1.1);
                printf("Rodinha do mouse para cima\n");
            }
            if(windowEvent.wheel.y < 0){
                escalaObjeto(obj1, 0.9, 0.9, 0.9);
                printf("Rodinha do mouse para baixo\n");
            }
        }
        }




        //OBJETO 2
        if(mouseSobreObjeto(obj2, matFinal2, xmouse, ymouse)){
        //SDL_KEYDOWN captura o clicar em uma tecla
        if(windowEvent.type == SDL_KEYDOWN){
            if(windowEvent.key.keysym.sym == SDLK_LEFT){
                transladaObjeto(obj2, -0.1, 0, -0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_RIGHT){
                transladaObjeto(obj2, 0.1, 0, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_UP){
                transladaObjeto(obj2, 0, 0.1, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_DOWN){
                transladaObjeto(obj2, 0, -0.1, -0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_EQUALS){
                escalaObjeto(obj2, 1.1, 1.1, 1.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_MINUS){
                escalaObjeto(obj2, 0.9, 0.9, 0.9);
            }
            else if(windowEvent.key.keysym.sym == SDLK_a){
                rotacionaObjetoEixoX(obj2, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_b){
                rotacionaObjetoEixoY(obj2, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_c){
                rotacionaObjetoEixoZ(obj2, 0.1);
            }
        }

        //SDL_KEYUP captura o soltar uma tecla
        if(windowEvent.type == SDL_KEYUP){
            if(windowEvent.key.keysym.sym == SDLK_LEFT){
                printf("Seta da esquerda liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_RIGHT){
                printf("Seta da direita liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_UP){
                printf("Seta para cima liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_DOWN){
                printf("Seta para baixo liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_EQUALS){
                printf("Seta + liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_MINUS){
                printf("Seta - liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_a){
                printf("Seta a liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_b){
                printf("Seta b liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_c){
                printf("Seta c liberada\n");
            }
        }

        //SDL_MOUSEBUTTONDOWN captura o clicar um botão do mouse
        if(windowEvent.type == SDL_MOUSEBUTTONDOWN){
            if(windowEvent.button.button == SDL_BUTTON_LEFT){
                rotacionaObjetoEixoX(obj2, 0.1);
            }
            else if(windowEvent.button.button == SDL_BUTTON_RIGHT){
                rotacionaObjetoEixoY(obj2, 0.1);
            }
            else if(windowEvent.button.button == SDL_BUTTON_MIDDLE){
                rotacionaObjetoEixoZ(obj2, 0.1);
            }
        }

        //SDL_MOUSEBUTTONUP captura o soltar um botão do mouse
        if(windowEvent.type == SDL_MOUSEBUTTONUP){
            if(windowEvent.button.button == SDL_BUTTON_LEFT){
                printf("Botao esquerdo do mouse liberado\n");
            }
            else if(windowEvent.button.button == SDL_BUTTON_RIGHT){
                printf("Botao direito do mouse liberado\n");
            }
            else if(windowEvent.button.button == SDL_BUTTON_MIDDLE){
                printf("Botao do meio do mouse liberado\n");
            }
        }

        //SDL_MOUSEWHEEL captura o movimento no botão do meio do mouse
        if(windowEvent.type == SDL_MOUSEWHEEL){
            if(windowEvent.wheel.y > 0){
                escalaObjeto(obj2, 1.1, 1.1, 1.1);
                printf("Rodinha do mouse para cima\n");
            }
            if(windowEvent.wheel.y < 0){
                escalaObjeto(obj2, 0.9, 0.9, 0.9);
                printf("Rodinha do mouse para baixo\n");
            }
        }
        }




        //OS DOIS
        //OBJETO 2
        if(!mouseSobreObjeto(obj1, matFinal1, xmouse, ymouse) && !mouseSobreObjeto(obj2, matFinal2, xmouse, ymouse)){
        //SDL_KEYDOWN captura o clicar em uma tecla
        if(windowEvent.type == SDL_KEYDOWN){
            if(windowEvent.key.keysym.sym == SDLK_LEFT){
                transladaObjeto(obj1, -0.1, 0, -0.1);
                transladaObjeto(obj2, -0.1, 0, -0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_RIGHT){
                transladaObjeto(obj1, 0.1, 0, 0.1);
                transladaObjeto(obj2, 0.1, 0, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_UP){
                transladaObjeto(obj1, 0, 0.1, 0.1);
                transladaObjeto(obj2, 0, 0.1, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_DOWN){
                transladaObjeto(obj1, 0, -0.1, -0.1);
                transladaObjeto(obj2, 0, -0.1, -0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_EQUALS){
                escalaObjeto(obj1, 1.1, 1.1, 1.1);
                escalaObjeto(obj2, 1.1, 1.1, 1.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_MINUS){
                escalaObjeto(obj1, 0.9, 0.9, 0.9);
                escalaObjeto(obj2, 0.9, 0.9, 0.9);
            }
            else if(windowEvent.key.keysym.sym == SDLK_a){
                rotacionaObjetoEixoX(obj1, 0.1);
                rotacionaObjetoEixoX(obj2, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_b){
                rotacionaObjetoEixoY(obj1, 0.1);
                rotacionaObjetoEixoY(obj2, 0.1);
            }
            else if(windowEvent.key.keysym.sym == SDLK_c){
                rotacionaObjetoEixoZ(obj1, 0.1);
                rotacionaObjetoEixoZ(obj2, 0.1);
            }
        }

        //SDL_KEYUP captura o soltar uma tecla
        if(windowEvent.type == SDL_KEYUP){
            if(windowEvent.key.keysym.sym == SDLK_LEFT){
                printf("Seta da esquerda liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_RIGHT){
                printf("Seta da direita liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_UP){
                printf("Seta para cima liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_DOWN){
                printf("Seta para baixo liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_EQUALS){
                printf("Seta + liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_MINUS){
                printf("Seta - liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_a){
                printf("Seta a liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_b){
                printf("Seta b liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_c){
                printf("Seta c liberada\n");
            }
        }

        //SDL_MOUSEBUTTONDOWN captura o clicar um botão do mouse
        if(windowEvent.type == SDL_MOUSEBUTTONDOWN){
            if(windowEvent.button.button == SDL_BUTTON_LEFT){
                rotacionaObjetoEixoX(obj1, 0.1);
                rotacionaObjetoEixoX(obj2, 0.1);
            }
            else if(windowEvent.button.button == SDL_BUTTON_RIGHT){
                rotacionaObjetoEixoY(obj1, 0.1);
                rotacionaObjetoEixoY(obj2, 0.1);
            }
            else if(windowEvent.button.button == SDL_BUTTON_MIDDLE){
                rotacionaObjetoEixoZ(obj1, 0.1);
                rotacionaObjetoEixoZ(obj2, 0.1);
            }
        }

        //SDL_MOUSEBUTTONUP captura o soltar um botão do mouse
        if(windowEvent.type == SDL_MOUSEBUTTONUP){
            if(windowEvent.button.button == SDL_BUTTON_LEFT){
                printf("Botao esquerdo do mouse liberado\n");
            }
            else if(windowEvent.button.button == SDL_BUTTON_RIGHT){
                printf("Botao direito do mouse liberado\n");
            }
            else if(windowEvent.button.button == SDL_BUTTON_MIDDLE){
                printf("Botao do meio do mouse liberado\n");
            }
        }

        //SDL_MOUSEWHEEL captura o movimento no botão do meio do mouse
        if(windowEvent.type == SDL_MOUSEWHEEL){
            if(windowEvent.wheel.y > 0){
                escalaObjeto(obj1, 1.1, 1.1, 1.1);
                escalaObjeto(obj2, 1.1, 1.1, 1.1);
                printf("Rodinha do mouse para cima\n");
            }
            if(windowEvent.wheel.y < 0){
                escalaObjeto(obj1, 0.9, 0.9, 0.9);
                escalaObjeto(obj2, 0.9, 0.9, 0.9);
                printf("Rodinha do mouse para baixo\n");
            }
        }
        }



        //MOVIMENTAÇÃO DA CÂMERA
        //SDL_KEYDOWN captura o clicar em uma tecla
        if(windowEvent.type == SDL_KEYDOWN){
        if(windowEvent.key.keysym.sym == SDLK_d){
            posx += 0.1;
            defineCamera(camera, posx, posy, posz, focox, focoy, focoz, cimax, cimay, cimaz);

        }
        else if(windowEvent.key.keysym.sym == SDLK_e){
            posy += 0.1;
            defineCamera(camera, posx, posy, posz, focox, focoy, focoz, cimax, cimay, cimaz);

        }
        else if(windowEvent.key.keysym.sym == SDLK_f){
            posz += 0.1;
            defineCamera(camera, posx, posy, posz, focox, focoy, focoz, cimax, cimay, cimaz);

        }
        else if(windowEvent.key.keysym.sym == SDLK_g){
            focox += 0.05;
            defineCamera(camera, posx, posy, posz, focox, focoy, focoz, cimax, cimay, cimaz);

        }
        else if(windowEvent.key.keysym.sym == SDLK_h){
            focoy += 0.05;
            defineCamera(camera, posx, posy, posz, focox, focoy, focoz, cimax, cimay, cimaz);

        }
        else if(windowEvent.key.keysym.sym == SDLK_i){
            focoz += 0.05;
            defineCamera(camera, posx, posy, posz, focox, focoy, focoz, cimax, cimay, cimaz);

        }
        else if(windowEvent.key.keysym.sym == SDLK_j){
            cimax += 0.05;
            cimay += 0.05;
            cimaz += 0.05;
            defineCamera(camera, posx, posy, posz, focox, focoy, focoz, cimax, cimay, cimaz);

        }
        }

        //SDL_KEYUP captura o soltar uma tecla
        if(windowEvent.type == SDL_KEYUP){
            if(windowEvent.key.keysym.sym == SDLK_d){
                printf("Tecla d liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_e){
                printf("Tecla e liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_f){
                printf("Tecla f liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_g){
                printf("Tecla g liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_h){
                printf("Tecla h liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_i){
                printf("Tecla i liberada\n");
            }
            else if(windowEvent.key.keysym.sym == SDLK_j){
                printf("Tecla j liberada\n");
            }
        }

        criaIdentidade4d(matFinal1);
        multMatriz4d(obj1->modelMatrix, matFinal1);
        multMatriz4d(camera->viewMatrix, matFinal1);


        criaIdentidade4d(matFinal2);
        multMatriz4d(obj2->modelMatrix, matFinal2);
        multMatriz4d(camera->viewMatrix, matFinal2);


        //desenhar objetos
        desenhaObjetoTela(renderer, matFinal1, obj1);
        desenhaObjetoTela(renderer, matFinal2, obj2);


        SDL_RenderPresent(renderer);

    }


    desalocaTela(window);
    SDL_Quit();


    for(int i=0;i<identidade;i++){
        free(matFinal1[i]);
    }
    free(matFinal1);


    for(int i=0;i<identidade;i++){
        free(matFinal2[i]);
    }
    free(matFinal2);


    free(obj1);
    free(obj2);

    return EXIT_SUCCESS;
}

