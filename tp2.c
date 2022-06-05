#include <stdio.h>
#include "imagem.h"
#include "SeamCarving.h"

void main()
{

    Imagem ImagemEntrada;
    char *NomeArqEntrada = "bird.ppm";

    ImagemEntrada = lerArquivoPPM(NomeArqEntrada);

    ImagemEntrada = IntensidadeLuminosa(ImagemEntrada);

    ImagemEntrada = FiltroSobel(ImagemEntrada);

    ImagemEntrada = MatrizDinamica(ImagemEntrada);

    ImagemEntrada = PintaDeVerde(ImagemEntrada);
}