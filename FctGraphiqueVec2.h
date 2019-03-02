#pragma once
#include "Loop.h"
#include "Vecteur2.h"

void Line(Vec2, Vec2, Uint8 r, Uint8 g, Uint8 b);

void Rectangle(Vec2 p, Vec2 d, Uint8 r, Uint8 g, Uint8 b);

void LinePara(Vec2 p_1, Vec2 p_2, Uint8 r, Uint8 g, Uint8 b);
void LineDotted(Vec2, Vec2, Uint8 r, Uint8 g, Uint8 b);

void Cercle(Vec2 c, int rayon, Uint8 r, Uint8 g, Uint8 b);
