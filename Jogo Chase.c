#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define TAM 20

//Estrutura contendo coordenadas do jogador e dos robôs
typedef struct{

    //Coordenada X no primeiro [ ] e coordenada Y no segundo [ ] da matriz
    int x, y;

}CoordenadasPersonagem;

//Função preenche matriz com "espaços" em branco (matriz = mapa do jogo)
void preencheMatrizMapa(char mapa[TAM][TAM]){

    //Coloca caractere ' ' nos campos da matriz mapa
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            mapa[i][j] = ' ';
        }
    }

}

//Imprime Mapa do jogo com bordas delimitadas
void imprimeMapa(char mapa[TAM][TAM], int teleportesDisponiveis[], int nivel){

    //Imprime mapa e dá espaço entre os caracteres para melhor visualização
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            printf(" %c", mapa[i][j]);
        }

        if(i == 0){
            //Exibe o nivel atual após a primeira linha da matriz (+1 porque no código o primeiro nível é o nível 0)
            printf("| Nivel: %d\n", nivel + 1);
        }else if(i == 1){
            //Exibe o numero de teleportes seguros após a segunda linha da matriz
            printf("| Teleportes seguros disponiveis: %d\n", teleportesDisponiveis[0]);
        }else{
            //Coloca borda do mapa antes de passar para a próxima linha
            printf("|\n");
        }
    }

    //Borda inferior do mapa
    for(int i = 0; i < 41; i++){
        printf("-");
    }

    //Pula linha depois de imprimir o mapa
    printf("\n");
}

//Movimento aleatório do personagem
CoordenadasPersonagem teleportaJogador(CoordenadasPersonagem posicaoJogador, CoordenadasPersonagem posicaoRobo[], int quantidadeRobos, int teleportesDisponiveis[], char mapa[TAM][TAM]){

    //Variáveis para controle de loops
    int i, contaDiferentes = 0;

    //Inicializa gerador de aleatórios
    srand(time(NULL));

    //Teleporta para posição aleatória se os teleportes seguros estiverem zerados
    if(teleportesDisponiveis[0] == 0){
        posicaoJogador.x = rand() % 20;
        posicaoJogador.y = rand() % 20;
    }else{

        //Reduz contador de teleportes disponiveis quando um é utilizado
        teleportesDisponiveis[0]--;

        while(contaDiferentes != quantidadeRobos){

            //Nova posição aleatória é gerada
            posicaoJogador.x = rand() % 20;
            posicaoJogador.y = rand() % 20;

            contaDiferentes = 0;

            for(i = 0; i < quantidadeRobos; i++){

                if(posicaoJogador.x != posicaoRobo[i].x && posicaoJogador.y != posicaoRobo[i].y){
                    contaDiferentes++;
                }

            }

        //Fim while
        }

    //Fim else
    }

    //Retorna nova posição do jogador
    return posicaoJogador;
}

//Lê comando do usuário e retorna nova posição do personagem
CoordenadasPersonagem movimentaJogador(CoordenadasPersonagem posicaoJogador, int teleportesDisponiveis[], CoordenadasPersonagem posicaoRobo[], int quantidadeRobos, char mapa[TAM][TAM]){

    //Variável responsável pelo comando do usuário
    char comandoUsuario;

    //Lê novamente se o usuário tentar comando diferente dos comandos para ler movimento
    for(int i = 0; comandoUsuario != 'x' && comandoUsuario != 'w' && comandoUsuario != 'a' && comandoUsuario != 's' && comandoUsuario != 'd' && comandoUsuario != 'q' && comandoUsuario != 'e' && comandoUsuario != 'z' && comandoUsuario != 'c'; i++){
        scanf("%c%*c", &comandoUsuario);
    }

    //Configura W como movimento para cima
    if(comandoUsuario == 'w'){
        posicaoJogador.x--;
        posicaoJogador.y = posicaoJogador.y;
    }

    //Configura S como movimento para baixo
    if(comandoUsuario == 's'){
        posicaoJogador.x++;
        posicaoJogador.y = posicaoJogador.y;
    }

    //Configura A como movimento para a esquerda
    if(comandoUsuario == 'a'){
        posicaoJogador.y--;
        posicaoJogador.x = posicaoJogador.x;
    }

    //Configura D como movimento para a direita
    if(comandoUsuario == 'd'){
        posicaoJogador.y++;
        posicaoJogador.x = posicaoJogador.x;
    }

    //Configura Q como movimento para cima e para a esquerda
    if(comandoUsuario == 'q'){
        posicaoJogador.x--;
        posicaoJogador.y--;
    }

    //Configura E como movimento para cima e para a direita
    if(comandoUsuario == 'e'){
        posicaoJogador.x--;
        posicaoJogador.y++;
    }

    //Configura C como movimento para baixo e para a direita
    if(comandoUsuario == 'c'){
        posicaoJogador.x++;
        posicaoJogador.y++;
    }

    //Configura E como movimento para baixo e para a esquerda
    if(comandoUsuario == 'z'){
        posicaoJogador.x++;
        posicaoJogador.y--;
    }

    //Configura X como movimento especial
    if(comandoUsuario == 'x'){
        posicaoJogador = teleportaJogador(posicaoJogador, posicaoRobo, quantidadeRobos, teleportesDisponiveis, mapa);
    }

    //Bloqueia movimentos que saiam do mapa em X
    if(posicaoJogador.x > 19){
        posicaoJogador.x = 19;
    }else if(posicaoJogador.x < 0){
        posicaoJogador.x = 0;
    }

    //Bloqueia movimentos que saiam do mapa em Y
    if(posicaoJogador.y > 19){
        posicaoJogador.y = 19;
    }else if(posicaoJogador.y < 0){
        posicaoJogador.y = 0;
    }


    //Retorna nova posição do jogador
    return posicaoJogador;
}

//Movimenta robô na direção do usuário
CoordenadasPersonagem movimentaRobo(CoordenadasPersonagem posicaoRobo[], CoordenadasPersonagem posicaoJogador, int k){

    //Robôs se movem na diagonal para maior eficiência
    //Se movem na vertical ou horizontal apenas se estiverem na mesma linha/coluna do jogador

    //Verifica se estão na mesma linha
    if(posicaoRobo[k].x == posicaoJogador.x){

        //Caso positivo, move na linha
        if(posicaoRobo[k].y < posicaoJogador.y){
            posicaoRobo[k].y++;
        }else{
            posicaoRobo[k].y--;
        }

    //Verifica se estão na mesma coluna
    }else if(posicaoRobo[k].y == posicaoJogador.y){

        //Caso positivo, move na coluna
        if(posicaoRobo[k].x < posicaoJogador.x){
            posicaoRobo[k].x++;
        }else{
            posicaoRobo[k].x--;
        }

    }else{

        //Caso não estejam na mesma linha e nem na mesma coluna, robô se move em diagonal

        //Configura movimento em X
        if(posicaoRobo[k].x < posicaoJogador.x){
            posicaoRobo[k].x++;
        }else{
            posicaoRobo[k].x--;
        }

        //Configura movimento em Y
        if(posicaoRobo[k].y < posicaoJogador.y){
            posicaoRobo[k].y++;
        }else{
            posicaoRobo[k].y--;
        }
    }

    //Retorna nova posição do robô
    return posicaoRobo[k];
}

//Executa parte prática do jogo
void jogo(char mapa[TAM][TAM]){

    char proximoNivel;                                                  //Variável responsável por avançar nas telas entre níveis ou após derrota
    int i, a = 0, k, j;                                                 //Variáveis de controle de loops
    int teleportesDisponiveis[1];                                       //Variável responsável por controlar o número de teleportes seguros disponíveis
    int reposicionaJogador;                                             //Variável responsável por garantir que o usuário não inicie o jogo na mesma posição que um robô
    int contaRobosDestruidos;                                           //Variável responsável por avisar quando o número de robôs destruídos se iguala ao de robôs que iniciaram o nível
    int finalizaJogo = 0;                                               //Variável responsável por finalizar o jogo ao destruir todos os robôs ou ser capturado
    CoordenadasPersonagem posicaoJogador;                               //Estrutura responsável por armazenar a posição dos robôs
    CoordenadasPersonagem posicaoRobo[100];                             //Vetor de estruturas responsável por armazenar a posição dos robôs (máximo 100)

    srand(time(NULL));                                                  //Inicializa gerador de números aleatórios
    teleportesDisponiveis[0] = 0;                                       //Inicializa teleportes seguros com 0

    //Controla níveis do jogo
    for(int nivel = 0; finalizaJogo == 0; nivel++){

        //Caso = 1, reposiciona jogador
        reposicionaJogador = 0;

        //Inicializa quantidade de robôs como 2 a mais que o nível atual (nível começa em 0)
        int quantidadeRobos = nivel + 2;;

        //Inicializa jogador em posição aleatória
        posicaoJogador.x = rand() % 20;
        posicaoJogador.y = rand() % 20;


        //Inicializa os "k" robôs em posição aleatória
        for(k = 0; k < quantidadeRobos; k++){
            posicaoRobo[k].x = rand() % 20;
            posicaoRobo[k].y = rand() % 20;

            //Reposiciona jogador caso algum robô seja gerado na posição dele
            if(posicaoJogador.x == posicaoRobo[k].x && posicaoJogador.y == posicaoRobo[k].y){
                reposicionaJogador = 1;
            }

            //Verifica se tem robôs na mesma posição
            for(int k2 = 0; k2 < k; k2++){

                //Caso positivo, reposiciona robô
                if(posicaoRobo[k].x == posicaoRobo[k2].x && posicaoRobo[k].y == posicaoRobo[k2].y){
                    posicaoRobo[k].x = rand() % 20;
                    posicaoRobo[k].y = rand() % 20;
                }
            }
        }

        //Caso robô seja gerado na mesma posição do usuário, reposiciona usuário
        if(reposicionaJogador == 1){
            posicaoJogador.x = rand() % 20;
            posicaoJogador.y = rand() % 20;
        }


        //Loop para registrar cada movimento do jogador no mapa do jogo
        for(i = 0; a == 0; i++){

            //Variável responsável por finalizar o jogo
            finalizaJogo = 0;

            //Variável responsável por contar robôs destruídos para comparar com os que iniciaram o nível
            contaRobosDestruidos = 0;

            //Limpa a tela para mostrar novo mapa após movimento do usuário
            system("CLS");

            //Define posição atual do jogador como 'O'
            mapa[posicaoJogador.x][posicaoJogador.y] = 'O';

            //Coloca 'X' na posição atual de todos os robôs restantes
            for(k = 0; k < quantidadeRobos; k++){

                if(mapa[posicaoRobo[k].x][posicaoRobo[k].y] != 'W'){
                    mapa[posicaoRobo[k].x][posicaoRobo[k].y] = 'X';
                }
            }

            //Imprime mapa do jogo
            imprimeMapa(mapa, teleportesDisponiveis, nivel);

            //Limpa casa do jogagor e movimenta para nova casa
            mapa[posicaoJogador.x][posicaoJogador.y] = ' ';
            posicaoJogador = movimentaJogador(posicaoJogador, teleportesDisponiveis, posicaoRobo, quantidadeRobos, mapa);

            //Testa todas as combinações de possíveis colisões
            for(k = 0; k < quantidadeRobos; k++){

                //Testa as possíveis colisões até o robô anterior ao ultimo, pois o ultimo ja foi testado com todos
                if(k <= quantidadeRobos - 1){

                    for(j = k + 1; j < quantidadeRobos; j++){

                        //Muda apresentação dos robôs que colidiram
                        if(posicaoRobo[k].x == posicaoRobo[j].x && posicaoRobo[k].y == posicaoRobo[j].y){
                            mapa[posicaoRobo[k].x][posicaoRobo[k].y] = 'W';
                        }

                    }
                }

                //Move todos os robôs que não colidiram
                if(mapa[posicaoRobo[k].x][posicaoRobo[k].y] == 'X'){
                    mapa[posicaoRobo[k].x][posicaoRobo[k].y] = ' ';
                    movimentaRobo(posicaoRobo, posicaoJogador, k);
                }

            }


            //Testa se o jogador foi capturado por algum robô
            for(k = 0; k < quantidadeRobos; k++){

                //Caso tenha sido capturado, encerra o jogo
                if(posicaoJogador.x == posicaoRobo[k].x && posicaoJogador.y == posicaoRobo[k].y){
                    system("CLS");
                    printf("\n\tJogo encerrado. Voce perdeu!");
                    finalizaJogo = 1;

                    //Zera mapa
                    for(i = 0; i < TAM; i++){
                        for(j = 0; j < TAM; j++){
                            mapa[i][j] = ' ';
                        }
                    }

                    //Controla sequência do programa
                    printf("\n\nPressione ESPACO e depois ENTER para voltar ao menu inicial!");
                    scanf("%c%*c", &proximoNivel);

                    //Chama menu inicial para reiniciar o jogo
                    if(proximoNivel == 32 || proximoNivel == 13){
                        menu(mapa);
                    }

                }

            }

            //Testa se o jogador ganhou
            for(k = 0; k < quantidadeRobos; k++){

                //Se posição do robô apresenta 'W', ele foi destruído
                if(mapa[posicaoRobo[k].x][posicaoRobo[k].y] == 'W'){
                    contaRobosDestruidos++;
                }
            }

            //Verifica se o jogador ganhou (número de robôs destruídos = número de robôs que iniciaram o nível)
            if(contaRobosDestruidos == quantidadeRobos){
                system("CLS");
                printf("\n\tParabens nivel %d concluido!\n\n", nivel + 1);

                //Controla sequência do jogo
                printf("Pressione ESPACO e depois ENTER para comecar o proximo nivel!");


                //Garante que o programa não fecha se o usuário digitar errado
                for(int t = 1; t > 0; t++){
                    scanf("%c%*c", &proximoNivel);

                    if(proximoNivel == 32 || proximoNivel == 13){
                        break;
                    }
                }


                //Ao avançar o nível, usuário ganha um teleporte seguro e aumenta-se a quantidade de robôs
                teleportesDisponiveis[0]++;
                quantidadeRobos++;

                //Zera mapa
                for(i = 0; i < TAM; i++){
                        for(j = 0; j < TAM; j++){
                            mapa[i][j] = ' ';
                        }
                    }

                //Controla sequência do jogo
                if(proximoNivel == 32 || proximoNivel == 13){

                    //Interrompe loop que marca jogadas do usuário
                    break;
                }

            }

            //Encerra jogo caso condição anterior indique que o jogo acabou
            if(finalizaJogo == 1){
                break;
            }

         //Final do loop de jogadas
        }

    //Final do loop de níveis
    }


}

//Imprime instrucoes antes de começar o jogo
void instrucoes(char mapa[TAM][TAM]){

    //Cria string para cada intrução e exibe em seguida
    char regra[100] = "Regras do jogo:";
    puts(regra);
    char regra1[100] = "1 - Personagem exibido como 'O'";
    puts(regra1);
    char regra2[100] = "2 - Robos exibidos como 'X'";
    puts(regra2);
    char regra3[100] = "3 - Quando os robos colidem entre si, se transformam em uma pilha de escombros exibida como 'W'";
    puts(regra3);
    char regra4[1000] = "4 - Objetivo do jogo eh fazer com que todos os robos colidam entre si, sem deixar que eles capturem o usuario";
    puts(regra4);
    char regra5[1000] = "5 - Jogo termina quando os robos capturam o usuario ou apos todos os robos se transformarem em pilha de escombros";
    puts(regra5);
    char regra6[100] = "6 - Ao passar de nivel, todos os robos destruidos retornam e acrescenta-se mais um robo";
    puts(regra6);
    char regra7[1000] = "7 - Eh possivel teleportar aleatoriamente a qualquer momento, mas caso os teleportes seguros estejam zerados voce pode teleportar para o caminho de um robo do mau!";
    puts(regra7);
    char movimentacao[100] = "\nMovimentacao do usuario:\n";
    puts(movimentacao);

    //Cria setas para indicar a direção que cada comando representa
    char comandos[5][30] = {"  Q  W  E ", " \n   \\ | / \n", "\n A -- -- D  \n", "\n   / | \\", "\n  Z  S  C\n"};
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 11; j++){
            printf("%c", comandos[i][j]);
        }
    }
    printf("\n");
    char teleportar[100] = "Teleportar para local aleatorio (seguro se disponivel): Pressione X";
    puts(teleportar);

    printf("\n\tPressione ESPACO e em seguida ENTER para voltar...\n");
    char continuar;

    //Controla sequência do programa
    scanf("%c", &continuar);
    if(continuar == 32){
        menu(mapa);
    }


}

//Sobre o jogo
void sobre(char mapa[TAM][TAM]){

    system("CLS");

    printf("\n- Jogo desenvolvido por Gabriel Farias Alves em 08/2021");

    //Controla sequência do programa
    printf("\n\n\tPressione ESPACO e em seguida ENTER para voltar...\n\n\n");
    char continuar;

    scanf("%c", &continuar);
    if(continuar == 32){
        menu(mapa);
    }

}

//Menu inicial
int menu(char mapa[TAM][TAM]){

    system("CLS");

    //Variável para registrar opção do usuário no menu
    int controleMenu;



        //Menu do jogo
        printf("\tBem vindo ao CHASE!\n\n");
        printf("\tSelecione uma opcao para continuar:\n\n");
        printf("\t1 - Jogar\n");
        printf("\t2 - Como jogar\n");
        printf("\t3- Sobre o jogo\n");
        printf("\t4- Sair\n");

        scanf("%d%*c", &controleMenu);

        //Impede usuário de digitar opção não disponível
        if(controleMenu < 1 || controleMenu > 4){
            while(controleMenu < 1 || controleMenu > 3){
                scanf("%d%*c", &controleMenu);
            }
        }

        if(controleMenu == 1){
            //Função que executa o jogo
            jogo(mapa);
        }else if(controleMenu == 2){
            //Limpa a tela
            system("CLS");

            //Exibe instruções para jogar
            instrucoes(mapa);
        }else if(controleMenu == 3){
            //Exibe tela "sobre"
            sobre(mapa);
        }else{
            return 0;
        }


}

//Função principal
int main(){

    //Declara mapa do jogo como matriz de caracteres
    char mapa[TAM][TAM];

    //Inicializa mapa do jogo
    preencheMatrizMapa(mapa);

    //Chama função que controla o programa
    menu(mapa);

    return 0;
}
