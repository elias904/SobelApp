#include <stdio.h>
#include <math.h>
#include "imagem.h"
#include "Sup.h"


Imagem IntensidadeLuminosa(Imagem ImagemEntrada)
{
    for (int i = 0; i < ImagemEntrada.altura; i++)
    {
        for (int j = 0; j < ImagemEntrada.largura; j++)
        {
            ImagemEntrada.RGB[i][j].IL = 0.3 * ImagemEntrada.RGB[i][j].R + 0.59 * ImagemEntrada.RGB[i][j].G + 0.11 * ImagemEntrada.RGB[i][j].B;
        }
    }
    return ImagemEntrada;
}

Imagem FiltroSobel(Imagem ImagemEntrada)
{
    int LinhaTemp = ImagemEntrada.altura + 2;
    int ColunaTemp = ImagemEntrada.largura + 2;
    float MatrizTemp[LinhaTemp][ColunaTemp];

    int I = 0;
    for (int i = 1; i < ImagemEntrada.altura - 1; i++)
    {
        int J = 0;
        for (int j = 1; j < ImagemEntrada.largura - 1; j++)
        {
            MatrizTemp[i][j] = ImagemEntrada.RGB[I][J].Energia;
            J++;
        }
        I++;
    }

    // Duplicando a primeira linha
    for (int x = 1; x < ColunaTemp - 1; x++)
    {
        MatrizTemp[0][x] = MatrizTemp[1][x];
    }

    // Duplicando a ultima linha
    for (int x = 1; x < ColunaTemp - 1; x++)
    {
        MatrizTemp[LinhaTemp][x] = MatrizTemp[LinhaTemp - 1][x];
    }

    // Duplicando linha da esqueda
    for (int x = 1; x < LinhaTemp - 1; x++)
    {
        MatrizTemp[x][0] = MatrizTemp[x][1];
    }

    // Duplicando linha da direita
    for (int x = 1; x < LinhaTemp - 1; x++)
    {
        MatrizTemp[x][LinhaTemp] = MatrizTemp[x][LinhaTemp - 1];
    }

    MatrizTemp[0][0] = MatrizTemp[1][1];

    MatrizTemp[LinhaTemp][0] = MatrizTemp[LinhaTemp - 1][1];

    MatrizTemp[0][ColunaTemp] = MatrizTemp[1][ColunaTemp - 1];

    MatrizTemp[LinhaTemp][ColunaTemp] = MatrizTemp[LinhaTemp - 1][ColunaTemp - 1];

    // Aplicando a Convolução na Matriz;

    float pixel_x, pixel_y = 0;

    float sobel_x[3][3] =
        {{-1, 0, 1},
         {-2, 0, 2},
         {-1, 0, 1}};

    float sobel_y[3][3] =
        {{-1, -2, -1},
         {0, 0, 0},
         {1, 2, 1}};

    int i = 0;

    for (int x = 1; x < LinhaTemp - 1; x++)
    {
        int j = 0;
        for (int y = 1; y < ColunaTemp - 1; y++)
        {
            pixel_x += sobel_x[0][0] * MatrizTemp[x - 1][y - 1];
            pixel_x += sobel_x[0][1] * MatrizTemp[x - 1][y];
            pixel_x += sobel_x[0][2] * MatrizTemp[x - 1][y + 1];
            pixel_x += sobel_x[1][0] * MatrizTemp[x][y - 1];
            pixel_x += sobel_x[1][1] * MatrizTemp[x][y];
            pixel_x += sobel_x[1][2] * MatrizTemp[x][y + 1];
            pixel_x += sobel_x[2][0] * MatrizTemp[x + 1][y - 1];
            pixel_x += sobel_x[2][1] * MatrizTemp[x + 1][y];
            pixel_x += sobel_x[2][2] * MatrizTemp[x + 1][y + 1];

            pixel_y += sobel_y[0][0] * MatrizTemp[x - 1][y - 1];
            pixel_y += sobel_y[0][1] * MatrizTemp[x - 1][y];
            pixel_y += sobel_y[0][2] * MatrizTemp[x - 1][y + 1];
            pixel_y += sobel_y[1][0] * MatrizTemp[x][y - 1];
            pixel_y += sobel_y[1][1] * MatrizTemp[x][y];
            pixel_y += sobel_y[1][2] * MatrizTemp[x][y + 1];
            pixel_y += sobel_y[2][0] * MatrizTemp[x + 1][y - 1];
            pixel_y += sobel_y[2][1] * MatrizTemp[x + 1][y];
            pixel_y += sobel_y[2][2] * MatrizTemp[x + 1][y + 1];

            float e = (float)sqrt((pixel_x * pixel_x) + (pixel_y * pixel_y));
            ImagemEntrada.RGB[i][j].Energia = e;
            j++;
        }
        i++;
    }
    return ImagemEntrada;
}

Imagem MatrizDinamica(Imagem ImagemEntrada)
{
    for (int i = 1; i < ImagemEntrada.altura; i++)
    {
        for (int j = 0; j < ImagemEntrada.largura; j++)
        {
            if (j > 0 && j < ImagemEntrada.largura - 1)
            {
                float A = ImagemEntrada.RGB[i + 1][j - 1].Energia;
                float B = ImagemEntrada.RGB[i + 1][j].Energia;
                float C = ImagemEntrada.RGB[i + 1][j + 1].Energia;

                float Temp = ValorDinamicoMid(A, B, C);
                if (Temp == A)
                {
                    ImagemEntrada.RGB[i][j].Energia = ImagemEntrada.RGB[i][j].Energia + A;
                }
                else if (Temp == B)
                {
                    ImagemEntrada.RGB[i][j].Energia = ImagemEntrada.RGB[i][j].Energia + B;
                }
                else
                {
                    ImagemEntrada.RGB[i][j].Energia = ImagemEntrada.RGB[i][j].Energia + C;
                }
            }
            else
            {
                float A = ImagemEntrada.RGB[i + 1][j].Energia;
                if (j == 0)
                {
                    float B = ImagemEntrada.RGB[i + 1][j + 1].Energia;
                    float Temp = ValorDinamicoBorda(A, B);
                    if (Temp == A)
                    {
                        ImagemEntrada.RGB[i][j].Energia = ImagemEntrada.RGB[i][j].Energia + A;
                    }
                    else
                    {
                        ImagemEntrada.RGB[i][j].Energia = ImagemEntrada.RGB[i][j].Energia + B;
                    }
                }
                else
                {
                    float B = ImagemEntrada.RGB[i + 1][j - 1].Energia;
                    float Temp = ValorDinamicoBorda(A, B);
                    if (Temp == A)
                    {
                        ImagemEntrada.RGB[i][j].Energia = ImagemEntrada.RGB[i][j].Energia + A;
                    }
                    else
                    {
                        ImagemEntrada.RGB[i][j].Energia = ImagemEntrada.RGB[i][j].Energia + B;
                    }
                }
            }
        }
    }
    return ImagemEntrada;
}

Imagem PintaDeVerde(Imagem ImagemEntrada)
{
    int Start = 0;

    //Pega o indice do menor elemento da primeira linhas
    for (int x = 1; x < ImagemEntrada.largura; x++)
    {
        float Menor = ImagemEntrada.RGB[0][0].Energia;

        if (ImagemEntrada.RGB[0][x].Energia < Menor)
            Menor = ImagemEntrada.RGB[0][x].Energia, Start = x;
    }

    int Parametro = ImagemEntrada.altura;
    int i = 0;
    int j = Start;

    // Pinto o primeiro de Verde
    ImagemEntrada.RGB[0][j].R = 0;
    ImagemEntrada.RGB[0][j].G = 128;
    ImagemEntrada.RGB[0][j].B = 0;

    while (Parametro - 1)
    {
        if (j > 0 && j < ImagemEntrada.largura - 1)
        {
            float A = ImagemEntrada.RGB[i + 1][j - 1].Energia;
            float B = ImagemEntrada.RGB[i + 1][j].Energia;
            float C = ImagemEntrada.RGB[i + 1][j + 1].Energia;

            float Temp = ValorDinamicoMid(A, B, C);
            if (Temp == A)
            {
                ImagemEntrada.RGB[i + 1][j - 1].R = 0;
                ImagemEntrada.RGB[i + 1][j - 1].G = 128;
                ImagemEntrada.RGB[i + 1][j - 1].B = 0;
                i = i + 1;
                j = j - 1;
            }
            else if (Temp == B)
            {
                ImagemEntrada.RGB[i + 1][j].R = 0;
                ImagemEntrada.RGB[i + 1][j].G = 128;
                ImagemEntrada.RGB[i + 1][j].B = 0;
                i = i + 1;
            }
            else
            {
                ImagemEntrada.RGB[i + 1][j + 1].R = 0;
                ImagemEntrada.RGB[i + 1][j + 1].G = 128;
                ImagemEntrada.RGB[i + 1][j + 1].B = 0;
                i = i + 1;
                j = j + 1;
            }
        }
        else
        {
            float A = ImagemEntrada.RGB[i + 1][j].Energia;
            if (j == 0)
            {
                float B = ImagemEntrada.RGB[i + 1][j + 1].Energia;
                float Temp = ValorDinamicoBorda(A, B);
                if (Temp == A)
                {
                    ImagemEntrada.RGB[i + 1][j].R = 0;
                    ImagemEntrada.RGB[i + 1][j].G = 128;
                    ImagemEntrada.RGB[i + 1][j].B = 0;
                    i = i + 1;
                }
                else
                {
                    ImagemEntrada.RGB[i + 1][j + 1].R = 0;
                    ImagemEntrada.RGB[i + 1][j + 1].G = 128;
                    ImagemEntrada.RGB[i + 1][j + 1].B = 0;
                    i = i + 1;
                    j = j + 1;
                }
            }
            else
            {
                float B = ImagemEntrada.RGB[i + 1][j - 1].Energia;
                float Temp = ValorDinamicoBorda(A, B);
                if (Temp == A)
                {
                    ImagemEntrada.RGB[i + 1][j].R = 0;
                    ImagemEntrada.RGB[i + 1][j].G = 128;
                    ImagemEntrada.RGB[i + 1][j].B = 0;
                    i = i + 1;
                }
                else
                {
                    ImagemEntrada.RGB[i + 1][j - 1].R = 0;
                    ImagemEntrada.RGB[i + 1][j - 1].G = 128;
                    ImagemEntrada.RGB[i + 1][j - 1].B = 0;
                    i = i + 1;
                    j = j - 1;
                }
            }
            Parametro--;
        }
    }
    imprimirImagem(ImagemEntrada);
}