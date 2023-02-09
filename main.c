//Alunos: Francisco Palermo e Samuel Rech. Jogo de tabuleiro que gera peças (1,2,3 ou multiplos de 3) a serem colocadas no mesmo com o objetivo que é empilhar peças virtuais formando novos valores e liberando espaço no tabuleiro para ganhar pontos. 

//CONSTANTES - DADOS DO TABULEIRO E DA TELA
#define X1_TAB 180
#define Y1_TAB 20
#define X2_TAB 580
#define Y2_TAB 560
#define LARGURA_TELA 600
#define ALTURA_TELA  600

//Bibliotecas
#include "tela.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  //Matriz do jogo
  int mat[4][4];
  //Próxima peça
  int prox_peca;
  //Direção que irão as peças
  int lin, col; 
  //Pontuação
  int pontos;
  //Ranking
  int ranking[10];
} jogo;

//Desenha o mundo
void desenha_mundo(jogo *jog);
//Faz o desenho do mundo inical
void desenho_inicial(jogo *jog);
//Função que faz a troca das cores das peças
int escolhe_cor(int n);
//Cor do texto
int cor_texto(int n);
//Função inical do jogo
void inicio_jogo(jogo *jog, int pecas[]);
//Preenche a matriz
void preenche_mat(jogo *jog);
//Atualiza a matriz depois fazer o movimento da jogada
bool atualiza_mat(jogo *jog);
//Faz o sorteio das peças que serão adionadas conforme o jogo avança
int sorteio_peca(int peca_j[], int peca_m[]);
//Coloca a peça na matriz
void coloca_peca(int n);
//Processa as entradas
void processa_entradas(jogo *jog);
//Prepara as variáveis
void inicializa_mundo(jogo *jog);
//Sorteia números aleátorios
int sorteio(int quant);
//Verifica a derrota
bool derrota(jogo *jog);
//Situação de derrota observando os comandos 'w' e 'a'
bool condicaoWA(int mat[4][4], int lin, int col);
//Situalçao de derrota observando os comandos 's' e 'd'
bool condicaoSD(int mat[4][4], int lin, int col);
//Calcula a pontuação
void pontuacao(jogo *jog);
//Mostra a pontuação e o ranking dentro do jogo
void tela_final(jogo *jog);
//Adiciona a pontuação do jogador no ranking do jogo
void ranking(jogo *jog);
//Ordena o ranking no sentido decrescente
void ordem_decrescente(jogo *jog);

int main()
{
  //Aqui é guardada a probabilidade de cair as peças 
  int pecasI[100] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                     3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6,6};
  int pecas_j[50] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                     2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                     3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                     0,0,0};
  int pecas_m[20] = {6,6,6,6,6,6,6,6,6,6,
                     12,12,12,12,12,12,
                     24,24,24,48};
  jogo jog;
  //Prepara o visual do jogo
  tela_inicio(LARGURA_TELA, ALTURA_TELA, "Threes");
  inicializa_mundo(&jog);
  //Coloca as peças iniciais
  inicio_jogo(&jog, pecasI);
  bool termina_jogada = false;
  //Laço que repete até o fim do jogo
  while(!derrota(&jog)) {
    //Sorteia a próxima peça
    jog.prox_peca = sorteio_peca(pecas_j, pecas_m);
    termina_jogada = false;
    //Laço que repete até que seja feita alguma jogada
    while(termina_jogada == false) {
      jog.lin = 0;
      jog.col = 0;
      desenha_mundo(&jog);
      coloca_peca(jog.prox_peca);
      processa_entradas(&jog);
      termina_jogada = atualiza_mat(&jog);
    }
  }
  //Calcula a pontuação do jogador
  pontuacao(&jog);
  //Cadastra a pontuação
  ranking(&jog);
  //Exibe a tela final para o jogador
  tela_final(&jog);
  sleep(5);
  tela_fim();   
}
//Sorteia números aleátorios
int sorteio(int quant)
{
  return rand() % quant;
}
//Lê a tecla digitada e se for compatível com os comandos de jogada do jogo, ela é feita.
void processa_entradas(jogo *jog)
{
  switch (tela_le_tecla()) {
    case 'w':
      jog->lin = -1;
      jog->col = 0;
      break;
    case 'a':
      jog->col = -1;
      jog->lin = 0;
      break;
    case 's':
      jog->lin = 1;
      jog->col = 0;
      break;
    case 'd':
      jog->col = 1;
      jog->lin = 0;
      break;
  }
}
//Sorteia as peças que irão entrar no jogo de acordo com a probabilidade de cada peças
int sorteio_peca(int peca_j[], int peca_m[])
{
  int peca;
  srand(time(NULL));
  peca = peca_j[sorteio(50)];

  if(peca == 0){
    peca = peca_m[sorteio(20)];
  }
  return peca;
}
//Mostra na tela qual vai ser a próxima peça a entrar no tabuleiro
void coloca_peca(int n)
{
  int cor_pec = escolhe_cor(n);
  tela_retangulo(56, 66, 124, 174, 1, cor_pec, cor_pec);
  tela_termina_desenho();
}
//Faz o desenho inicial do jogo
void desenho_inicial(jogo *jog)
{
  //inicia o desenho
  tela_inicia_desenho();
  //cor da tela
  int cor_tela = tela_cria_cor(0.7,1,0.9);
  //cor do tabuleiro
  int cor_tab = tela_cria_cor(0.816 , 0.902, 0.875);
  //cor dos espa vazios
  int cor_esp = tela_cria_cor(0.714, 0.851, 0.851);
  //tela  
  tela_retangulo(0, 0, 600, 600, 1, cor_tela, cor_tela);
  //tabuleiro
  tela_retangulo(X1_TAB, Y1_TAB, X2_TAB, Y2_TAB, 1, cor_tab, cor_tab);
  //local da próxima peca
  tela_retangulo(40, 50, 140, 190, 1, cor_tab, cor_tab);
  //posições dos espações para as peças
  int x1e, y1e = Y1_TAB, x2e, y2e = Y1_TAB;
  for(int i = 0; i < 4; i++) {
    x1e = X1_TAB + 20;
    x2e = X1_TAB + 95;
    y1e = y2e + 20;
    y2e += 130; 
    for(int j = 0; j < 4; j++) {
      //espaços vazios
      tela_retangulo(x1e, y1e, x2e, y2e, 1, cor_esp, cor_esp);
      x1e = x2e + 20;
      x2e += 95;
    }
  }
}
//Atualiza a matriz do jogo de acordo com a jogada feita e retorna um booleano (true or false)
bool atualiza_mat(jogo *jog)
{
  int contC = 0, contL[4]= {0, 0, 0 , 0}, maiorL = 0, maiorC = 0, lin = 0, col = 0;
  bool deu = false;
  if(jog->lin == -1 || jog->col == -1) {    
    lin = 3;
    col = 3;
    for(int i = 0; i < 4; i++) {
      contC = 0;
      for(int j = 0; j < 4; j++) {
        if(jog->mat[i][j] != 0) {
          if((i+jog->lin) >=0 && (j+jog->col) >= 0) {
            if(jog->mat[i+jog->lin][j+jog->col] == 0) {
              jog->mat[i+jog->lin][j+jog->col] = jog->mat[i][j];
              jog->mat[i][j] = 0;
              deu = true;
              contC++;
              contL[j]++;      
            } else if(((jog->mat[i][j] == jog->mat[(i+jog->lin)][(j+jog->col)]) || ((jog->mat[i][j] + jog->mat[(i+jog->lin)][(j+jog->col)]) == 3)) && 
                     (((jog->mat[i][j] + jog->mat[i+jog->lin][j+jog->col]) % 3) == 0)) {
              if(((i+jog->lin) == 0 || (j+jog->col) == 0) || (jog->mat[i+jog->lin+jog->lin][j+jog->col+jog->col] != 0)) {
                jog->mat[(i+jog->lin)][(j+jog->col)] += jog->mat[i][j];
                jog->mat[i][j] = 0;
                contC++;
                contL[j]++;
                deu = true;
              }
            }
          }
        }
        if((jog->lin == -1 && contL[j] > maiorL)) {
          maiorL = contL[j];
          col = j;
        }
      }
      if((jog->col == -1 && contC > maiorC)) {
        maiorC = contC;
        lin = i;
      }
    }
  } else if(jog->lin == 1 || jog->col == 1) {
    for(int i = 3; i >=0 ; i--) {
      contC = 0;
      for(int j = 3; j >=0; j--) {
        if(jog->mat[i][j] != 0) {
          if((i+jog->lin) <= 3 && (j+jog->col) <= 3) {
            if(jog->mat[i+jog->lin][j+jog->col] == 0) {
              jog->mat[i+jog->lin][j+jog->col] = jog->mat[i][j];
              jog->mat[i][j] = 0;
              deu = true;
              contC++;
              contL[j]++; 
            } else if(((jog->mat[i][j] == jog->mat[(i+jog->lin)][(j+jog->col)]) || ((jog->mat[i][j] + jog->mat[(i+jog->lin)][(j+jog->col)]) == 3)) && 
                     (((jog->mat[i][j] + jog->mat[i+jog->lin][j+jog->col]) % 3) == 0)) {
              if(((i+jog->lin) == 3 || (j+jog->col) == 3) || (jog->mat[i+jog->lin+jog->lin][j+jog->col+jog->col] != 0)) {
                jog->mat[(i+jog->lin)][(j+jog->col)] += jog->mat[i][j];
                jog->mat[i][j] = 0;
                deu = true;
                contC++;
                contL[j]++;
              }
            }
          }
        }
        if((jog->lin == 1 && contL[j] > maiorL)) {
          maiorL = contL[j];
          col = j;
        }
      }
      if((jog->col == 1 && contC > maiorC)) {
        maiorC = contC;
        lin = i;
      }
    }
  }
  if(deu == true) {
    jog->mat[lin][col] = jog->prox_peca;
  }
  return deu;
}
//Start do jogo
void inicializa_mundo(jogo *jog)
{
  preenche_mat(jog);
  jog->lin = 0;
  jog->col = 0;
  jog->pontos = 0;
  for(int i = 0; i < 10; i++){
    jog->ranking[i] = 0;
  }
}
//Desenha o mundo conforme são feitas as jogadas
void desenha_mundo(jogo *jog)
{
  //desenha o tabuleiro padão
  desenho_inicial(jog);
  //cor pontuação
  //int cor_tab = tela_cria_cor(0.816 , 0.902, 0.875);
  char s[20];
  //cor do texto da peça 
  int cor_txt;
  //cor da peça
  int cor_pec;
  //posição das peças
  int x1e, y1e = Y1_TAB, x2e, y2e = Y1_TAB;
  for(int i = 0; i < 4; i++) {
    x1e = X1_TAB + 20;
    x2e = X1_TAB + 95;
    y1e = y2e + 20;
    y2e += 130;
    for(int j = 0; j < 4; j++) {
      if(jog->mat[i][j] != 0) {
        cor_pec = escolhe_cor(jog->mat[i][j]);
        //coloca peças nas posições de acordo com a matriz
        tela_retangulo(x1e, y1e, x2e, y2e, 1, cor_pec, cor_pec);
        sprintf(s, "%d", jog->mat[i][j]);
        cor_txt = cor_texto(jog->mat[i][j]);   
        //coloca o número referente a peça
        tela_texto((x1e+x2e)/2, (y1e + y2e)/2, 26, cor_txt, s);
      }
      x1e = x2e + 20;
      x2e += 95;
    }
  }
}
//Preenche a matriz com zeros  
void preenche_mat(jogo *jog)
{
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      jog->mat[i][j] = 0;
    }
  }
}
//Coloca as peças iniciais no tabuleiro
void inicio_jogo(jogo *jog, int pecas[])
{
  int posicao_peca, posicao_lin, posicao_col;
  int i;
  srand(time(NULL));
  for(i = 0; i < 9; i++) {
    posicao_peca = sorteio(100);
    do {
      posicao_lin = sorteio(4);
      posicao_col = sorteio(4); 
    } while(jog->mat[posicao_lin][posicao_col] != 0);
    jog->mat[posicao_lin][posicao_col] = pecas[posicao_peca];
  }
}
//Função para escolher qual vai ser a cor dos textos
int cor_texto(int n)
{
  int cor;
  if(n == 1 || n == 2) {
    cor = 8;
  } else {
    cor = 5;
  }
  return cor;
}
//Função para escolher a cor da peça
int escolhe_cor(int n)
{
  int cor;
  switch (n){
  case 1:
    cor = tela_cria_cor(0.4, 0.796, 0.988);
    break;
  case 2:
    cor = tela_cria_cor(0.941, 0.384, 0.494);
    break;
  default:
    cor = 8;
    break;
  }
  return cor;
}
//Retorna um booleano (V ou F) que confirma se o jogo acabou ou não
bool derrota(jogo *jog)
{
  bool perdeu = false;
  if(!condicaoWA(jog->mat,-1, 0) && !condicaoWA(jog->mat, 0,-1) && !condicaoSD(jog->mat, 1,0) && !condicaoSD(jog->mat, 0,1)) {
    perdeu = true;
  }
  return perdeu;
}
//Condição de derrota para quando são dados os comandos 'w' ou 'a'
bool condicaoWA(int mat[4][4], int lin, int col)
{
  bool flag = false;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(mat[i][j] != 0) {
        if((i+lin) >=0 && (j+col) >= 0) {
          if(mat[i+lin][j+col] == 0) {
            flag = true;               
          } else if(((mat[i][j] == mat[(i+lin)][(j+col)]) || ((mat[i][j] + mat[(i+lin)][(j+col)]) == 3)) && (((mat[i][j] + mat[i+lin][j+col]) % 3) == 0)) {
            if(((i+lin) == 0 || (j+col) == 0) || (mat[i+lin+lin][j+col+col] != 0)) {
              flag = true;
            }
          }
        }
      }
    }
  }
  return flag;
}
//Condição de derrota para quando são dados os comandos 's' ou 'd'
bool condicaoSD(int mat[4][4], int lin, int col)
{
  bool flag = false;
  for(int i = 3; i >=0 ; i--) {
    for(int j = 3; j >=0; j--) {
      if(mat[i][j] != 0) {
        if((i+lin) < 4 && (j+col) < 4) {
          if(mat[i+lin][j+col] == 0) {
            flag = true;               
          } else if(((mat[i][j] == mat[(i+lin)][(j+col)]) || ((mat[i][j] + mat[(i+lin)][(j+col)]) == 3)) && (((mat[i][j] + mat[i+lin][j+col]) % 3) == 0)) {
            if(((i+lin) == 3 || (j+col) == 3) || (mat[i+lin+lin][j+col+col] != 0)){
              flag = true;
            }
          }
        }
      }
    }
  }
  return flag;
}
//Calcula a pontuação adquirida
void pontuacao(jogo *jog) 
{
  int x,y, n = 0;
  for(x = 0; x < 4; x++) {
    for(y = 0; y < 4; y++) {
      if(jog->mat[x][y] == 3) {
        jog->pontos = jog->pontos + 3;
      } else if(jog->mat[x][y] == 1 || jog->mat[x][y] == 2) {
      } else {
        n = jog->mat[x][y];
        jog->pontos = jog->pontos + (3*(n/2));
      }
    }
  }
}
//Faz o desenho da tela final, onde são mostrados a pontuação e ranking
void tela_final(jogo *jog) {
  tela_inicia_desenho();
  
  int cor_tab = tela_cria_cor(0.816 , 0.902, 0.875);
  char s[20], tab[50];
  int x1 = 0, y1 = 0, x2 = LARGURA_TELA, y2 = ALTURA_TELA;
  //fundo da tela final
  tela_retangulo(x1, y1, x2, y2, 1, cor_tab, cor_tab);
  //coloca pontução na variável
  sprintf(s, "%d", jog->pontos);
  sprintf(tab, "Pontuação");
  //coloca pontuação na tela
  tela_texto((x1+x2)/2, 40, 26, preto, tab);
  tela_texto((x1+x2)/2, 75, 26, preto, s);
  //coloca o ranking
  sprintf(tab, "Ranking");
  tela_texto((x1+x2)/2, 110, 26, preto, tab);
  //posicao das pontuações do ranking
  int posicao = 145;
  for(int i = 0; i < 9; i++) {
    sprintf(s, "%d", jog->ranking[i]);
    if(jog->ranking[i] != 0) {
      tela_texto((x1+x2)/2, posicao, 26, preto, s);
      posicao +=40;
    }
  }
  tela_termina_desenho();
}
//Salva a pontuação no arquivo "ranking" e ordena ele para que seja um ranking em ordem decrescente
void ranking(jogo *jog)
{
  FILE *file;
  file = fopen("ranking","a");
  if(file == NULL) {
    printf("Arquivo não pode ser aberto");
  }
  //escreve o jogador no ranking
  fprintf(file, "%d\n",jog->pontos);
  fclose(file);
  //Ordena em ordem crescente
  ordem_decrescente(jog);
}
//Ordena o arquivo em ordem decrescente
void ordem_decrescente(jogo *jog)
{
  int acc;
  FILE *file;
  file=fopen("ranking","r");
  for(int i=0;i<10;i++) {
    fscanf(file,"%d", &jog->ranking[i]);
  }
  fclose(file);
  file=fopen("ranking","w");
  for(int i = 0; i < 10; i++) {
    for(int j = i+1; j < 10; j++) {
      if(jog->ranking[i] < jog->ranking[j]) {
        acc = jog->ranking[i];
        jog->ranking[i] = jog->ranking[j];
        jog->ranking[j] = acc;
      }
    }
    if(jog->ranking[i] != 0) {
      fprintf(file," %d\n", jog->ranking[i]);
    }
  }
  fclose(file);
}
