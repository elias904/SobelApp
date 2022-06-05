#include "imagem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Imagem criaImagem(int altura, int largura)
{
    Imagem img;
    int i;

    img.altura = altura;
    img.largura = largura;

    img.RGB = (Pixel **)malloc(sizeof(Pixel *) * altura); // Aloca a primeira coluna da imagem

    for (i = 0; i < altura; i++)
    {
        img.RGB[i] = (Pixel *)malloc(sizeof(Pixel) * largura); // Aloca todas as linhas a partir da coluna alocada anteriormente
    }

    return img;
}

void destruirImagem(Imagem *img)
{
    if (img != NULL)
    {
        int i;

        for (i = 0; i < img->altura; i++)
        {
            free(img->RGB[i]); // Libera todas as linhas da memoria
        }

        free(img->RGB); // Libera a coluna "base"
        free(img);      // Libera a imagem
    }
}

Imagem lerArquivoPPM(char *nomeDoArquivo)
{
    FILE *arqEntrada;
    char primeiraLinha[3], aux, num[3];
    int maxRGBValue;
    int altura, largura;
    Imagem img;

    arqEntrada = fopen(nomeDoArquivo, "r");

    if (arqEntrada == NULL)
    {
        printf("Ocorreu um erro na abertura do arquivo!");
        exit(1);
    }

    fgets(primeiraLinha, sizeof(primeiraLinha), arqEntrada); // Le a primeira linha do arquivo (string magica)
    fscanf(arqEntrada, "%d %d", &largura, &altura);
    fscanf(arqEntrada, "%d", &maxRGBValue);

    img = criaImagem(altura, largura); // Aloca o espaco da imagem

    int i, j, k;

    i = 0;
    j = 0;

    for (i = 0; i < altura; i++)
    {
        for (j = 0; j < largura; j++)
        {
            fscanf(arqEntrada, "%d", &img.RGB[i][j].R);
            fscanf(arqEntrada, "%d", &img.RGB[i][j].G);
            fscanf(arqEntrada, "%d", &img.RGB[i][j].B);
        }
    }

    return img;
}

void imprimirImagem(Imagem imagem)
{
    int i, j, rgbint;
    FILE *saida;
    char stringmagica[3];

    strcat(stringmagica, "P3");
    rgbint = 255;

    saida = fopen("output.ppm", "w");
    fprintf(saida, "%s\n", stringmagica);
    fprintf(saida, "%d %d\n", imagem.largura, imagem.altura);
    fprintf(saida, "%d\n", rgbint);
    for (i = 0; i < imagem.altura; i++)
    {
        for (j = 0; j < imagem.largura; j++)
        {
            fprintf(saida, "%d ", imagem.RGB[i][j].R);
            fprintf(saida, "%d ", imagem.RGB[i][j].G);
            fprintf(saida, "%d ", imagem.RGB[i][j].B);
        }
        fprintf(saida, "\n");
    }
    fclose(saida);
}