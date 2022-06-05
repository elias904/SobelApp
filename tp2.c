#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"
#include "Seamcarving.h"

int main()
{

    Imagem ImagemEntrada;


    int Caminhos = 0;
    printf("Quantos Caminhos: ");
    scanf("%d",&Caminhos);

    char *NomeArqEntrada = "anders.ppm";

    ImagemEntrada = lerArquivoPPM(NomeArqEntrada);

    ImagemEntrada = IntensidadeLuminosa(ImagemEntrada);

    ImagemEntrada = FiltroSobel(ImagemEntrada);

    ImagemEntrada = MatrizDinamica(ImagemEntrada);

    for(int count = 0; count < Caminhos; count++)
    {
        ImagemEntrada = RetiraCaminhos(ImagemEntrada);
    }

    return 1;
}