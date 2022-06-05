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
    for (int i = 1; i < LinhaTemp - 1; i++)
    {
        int J = 0;
        for (int j = 1; j < ColunaTemp - 1; j++)
        {
            MatrizTemp[i][j] = ImagemEntrada.RGB[I][J].IL;
            J++;
        }
        I++;
    }

    // Duplicando a primeira linha
    for (int x = 1; x < ColunaTemp - 2; x++)
    {
        MatrizTemp[0][x] = MatrizTemp[1][x];
    }

    // Duplicando a ultima linha
    for (int x = 1; x < ColunaTemp - 2; x++)
    {
        MatrizTemp[LinhaTemp - 1][x] = MatrizTemp[LinhaTemp - 2][x];
    }

    // Duplicando linha da esqueda
    for (int x = 1; x < LinhaTemp - 2; x++)
    {
        MatrizTemp[x][0] = MatrizTemp[x][1];
    }

    // Duplicando linha da direita
    for (int x = 1; x < LinhaTemp - 2; x++)
    {
        MatrizTemp[x][LinhaTemp - 1] = MatrizTemp[x][LinhaTemp - 2];
    }

    MatrizTemp[0][0] = MatrizTemp[1][1];

    MatrizTemp[LinhaTemp - 1][0] = MatrizTemp[LinhaTemp - 2][1];

    MatrizTemp[0][ColunaTemp - 1] = MatrizTemp[1][ColunaTemp - 2];

    MatrizTemp[LinhaTemp - 1][ColunaTemp - 1] = MatrizTemp[LinhaTemp - 2][ColunaTemp - 2];

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
    for (int i = 1; i < ImagemEntrada.altura - 1; i++)
    {
        for (int j = 0; j < ImagemEntrada.largura; j++)
        {
            if (j > 0 && j < ImagemEntrada.largura - 1)
            {
                float A = ImagemEntrada.RGB[i - 1][j - 1].Energia;
                float B = ImagemEntrada.RGB[i - 1][j].Energia;
                float C = ImagemEntrada.RGB[i - 1][j + 1].Energia;

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
                float A = ImagemEntrada.RGB[i - 1][j].Energia;
                if (j == 0)
                {
                    float B = ImagemEntrada.RGB[i - 1][j + 1].Energia;
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
                    float B = ImagemEntrada.RGB[i - 1][j - 1].Energia;
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

    // Após ter montado a matriz dinamica, sabemos que a soma dos caminhos ficou na ultima linha
    // a partir disso basta pegar a menor soma na ultima linha
    for (int x = 0; x < ImagemEntrada.largura; x++)
    {
        float Menor = ImagemEntrada.RGB[ImagemEntrada.altura - 1][0].Energia;

        if (ImagemEntrada.RGB[ImagemEntrada.altura - 1][x].Energia < Menor)
            Start = x;
    }

    int i = ImagemEntrada.altura - 1; // Ultima linha
    int j = Start;

    printf("\nValores: %d e %d\n", i, j);

    // Pinto o primeiro de Verde
    ImagemEntrada.RGB[ImagemEntrada.altura - 1][j].R = 0;
    ImagemEntrada.RGB[ImagemEntrada.altura - 1][j].G = 255;
    ImagemEntrada.RGB[ImagemEntrada.altura - 1][j].B = 0;

    while (i > 0)
    {
        printf("- %d-", i);
        if (j > 0 && j < ImagemEntrada.largura - 1)
        {
            printf("Centro");
            float A = 0;
            float B = 0;
            float C = 0;

            A = ImagemEntrada.RGB[i - 1][j - 1].Energia;

            B = ImagemEntrada.RGB[i - 1][j].Energia;

            C = ImagemEntrada.RGB[i - 1][j + 1].Energia;

            float Temp = ValorDinamicoMid(A, B, C);
            if (Temp == A)
            {
                ImagemEntrada.RGB[i - 1][j - 1].R = 0;
                ImagemEntrada.RGB[i - 1][j - 1].G = 255;
                ImagemEntrada.RGB[i - 1][j - 1].B = 0;
                i = i - 1;
                j = j - 1;
            }
            else if (Temp == B)
            {
                ImagemEntrada.RGB[i - 1][j].R = 0;
                ImagemEntrada.RGB[i - 1][j].G = 255;
                ImagemEntrada.RGB[i - 1][j].B = 0;
                i = i - 1;
            }
            else
            {
                ImagemEntrada.RGB[i - 1][j + 1].R = 0;
                ImagemEntrada.RGB[i - 1][j + 1].G = 255;
                ImagemEntrada.RGB[i - 1][j + 1].B = 0;
                i = i - 1;
                j = j + 1;
            }
        }
        else // Borda
        {
            if (j == 0) // Esquerda
            {
                printf("Esquerda");
                float A = 0;
                float B = 0;

                A = ImagemEntrada.RGB[i - 1][j].Energia;

                B = ImagemEntrada.RGB[i - 1][j + 1].Energia;

                float Temp = ValorDinamicoBorda(A, B);
                if (Temp == A)
                {
                    ImagemEntrada.RGB[i - 1][j].R = 0;
                    ImagemEntrada.RGB[i - 1][j].G = 255;
                    ImagemEntrada.RGB[i - 1][j].B = 0;
                    i = i - 1;
                }
                else if (Temp == B)
                {
                    ImagemEntrada.RGB[i - 1][j + 1].R = 0;
                    ImagemEntrada.RGB[i - 1][j + 1].G = 255;
                    ImagemEntrada.RGB[i - 1][j + 1].B = 0;
                    i = i - 1;
                    j = j + 1;
                }
            }
            if (j == ImagemEntrada.largura - 1) // Direita
            {
                printf("Direita");
                float A = 0;
                float B = 0;

                A = ImagemEntrada.RGB[i - 1][j].Energia;

                B = ImagemEntrada.RGB[i - 1][j - 1].Energia;

                float Temp = ValorDinamicoBorda(A, B);
                if (Temp == A)
                {
                    ImagemEntrada.RGB[i - 1][j].R = 0;
                    ImagemEntrada.RGB[i - 1][j].G = 255;
                    ImagemEntrada.RGB[i - 1][j].B = 0;
                    i = i - 1;
                }
                else
                {
                    ImagemEntrada.RGB[i - 1][j - 1].R = 0;
                    ImagemEntrada.RGB[i - 1][j - 1].G = 255;
                    ImagemEntrada.RGB[i - 1][j - 1].B = 0;
                    i = i - 1;
                    j = j - 1;
                }
            }
        }
    }
    imprimirImagem(ImagemEntrada);
}

Imagem RetiraCaminhos(Imagem ImagemEntrada)
{

    Imagem ImagemSaida;

    ImagemSaida = criaImagem(ImagemEntrada.altura, ImagemEntrada.largura - 1);
    int Start = 0;

    // Após ter montado a matriz dinamica, sabemos que a soma dos caminhos ficou na ultima linha
    // a partir disso basta pegar a menor soma na ultima linha
    for (int x = 0; x < ImagemEntrada.largura; x++)
    {
        float Menor = ImagemEntrada.RGB[ImagemEntrada.altura - 1][0].Energia;

        if (ImagemEntrada.RGB[ImagemEntrada.altura - 1][x].Energia < Menor)
            Start = x;
    }

    int i = ImagemEntrada.altura - 1; // Ultima linha
    int j = Start;

    ImagemEntrada.RGB[ImagemEntrada.altura - 1][j].Energia = -1;

    while (i > 0)
    {
        printf("- %d-", i);
        if (j > 0 && j < ImagemEntrada.largura - 1)
        {
            printf("Centro");
            float A = 0;
            float B = 0;
            float C = 0;

            A = ImagemEntrada.RGB[i - 1][j - 1].Energia;

            B = ImagemEntrada.RGB[i - 1][j].Energia;

            C = ImagemEntrada.RGB[i - 1][j + 1].Energia;

            float Temp = ValorDinamicoMid(A, B, C);
            if (Temp == A)
            {
                ImagemEntrada.RGB[i - 1][j - 1].Energia = -1;
                i = i - 1;
                j = j - 1;
            }
            else if (Temp == B)
            {
                ImagemEntrada.RGB[i - 1][j].Energia = -1;
                i = i - 1;
            }
            else
            {
                ImagemEntrada.RGB[i - 1][j + 1].Energia = -1;
                i = i - 1;
                j = j + 1;
            }
        }
        else // Borda
        {
            if (j == 0) // Esquerda
            {
                printf("Esquerda");
                float A = 0;
                float B = 0;

                A = ImagemEntrada.RGB[i - 1][j].Energia;

                B = ImagemEntrada.RGB[i - 1][j + 1].Energia;

                float Temp = ValorDinamicoBorda(A, B);
                if (Temp == A)
                {
                    ImagemEntrada.RGB[i - 1][j].Energia= -1;
                    i = i - 1;
                }
                else if (Temp == B)
                {
                    ImagemEntrada.RGB[i - 1][j + 1].Energia = -1;
                    i = i - 1;
                    j = j + 1;
                }
            }
            if (j == ImagemEntrada.largura - 1) // Direita
            {
                printf("Direita");
                float A = 0;
                float B = 0;

                A = ImagemEntrada.RGB[i - 1][j].Energia;

                B = ImagemEntrada.RGB[i - 1][j - 1].Energia;

                float Temp = ValorDinamicoBorda(A, B);
                if (Temp == A)
                {
                    ImagemEntrada.RGB[i - 1][j].Energia = -1;
                    i = i - 1;
                }
                else
                {
                    ImagemEntrada.RGB[i - 1][j - 1].Energia = -1;
                    i = i - 1;
                    j = j - 1;
                }
            }
        }
    }
    return ImagemSaida;
}
