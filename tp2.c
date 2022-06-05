#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"
#include "Seamcarving.h"

int main()
{

    Imagem ImagemEntrada;

    char *NomeArqEntrada = "Imagem.txt";

    ImagemEntrada = lerArquivoPPM(NomeArqEntrada);

    ImagemEntrada = IntensidadeLuminosa(ImagemEntrada);

    ImagemEntrada = FiltroSobel(ImagemEntrada);

    ImagemEntrada = MatrizDinamica(ImagemEntrada);

    ImagemEntrada = PintaDeVerde(ImagemEntrada);

    return 1;
}