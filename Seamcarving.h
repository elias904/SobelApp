#ifndef SEAMCARVING_H_
#define SEAMCARVING_H_

#include "imagem.h"



Imagem IntensidadeLuminosa(Imagem ImagemEntrada);
Imagem FiltroSobel(Imagem ImagemEntrada);
Imagem MatrizDinamica(Imagem ImagemEntrada);
Imagem PintaDeVerde(Imagem ImagemEntrada);
Imagem RetiraCaminhos(Imagem ImagemEntrada);

#endif