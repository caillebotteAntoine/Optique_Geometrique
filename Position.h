#pragma once

#include "Component.h"
#include "Vecteur2.h"

class Position : public Component
{
public:

	Vec2 position;
	Vec2 direction;

	Position() { position = Vec2::Zero; direction = Vec2::Right; }
	Position( Vec2 p, Vec2 d) { position = p; direction = d; }

	Position(float x, float y):position(x,y){ direction = Vec2::Right; }
	
};