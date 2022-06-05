#ifndef IMAGE_H_
#define IMAGE_H_

/*------Estruturas-------------------------------------------------------------------------------*/

typedef struct
{
    int R,G,B;
    int x,y;
    float Energia;
    float IL;
} Pixel;

typedef struct
{
    int largura;
    int altura;
    Pixel **RGB; // todos os pixels da imagem
} Imagem;

/*--------Funcoes--------------------------------------------------------------------------------*/

Imagem lerArquivoPPM(char *nomeDoArquivo);
Imagem criaImagem(int altura, int largura);
void destruirImagem(Imagem *img);
void imprimirImagem(Imagem imagem);

#endif