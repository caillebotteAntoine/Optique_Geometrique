#pragma once

#include "Vecteur2.h"
#include "Loop.h"
#include "SDL.h"

class RayLight
{
public:
	RayLight(Vec2 o, Vec2 d, int l, bool a, int last);
		
	Vec2 GedDirection() const { return direction*longueur*(intersection_2 - intersection_1); }	
	Vec2 GetOrigine() const { return origine + direction * longueur*intersection_1; }
	Vec2 GetIntersection() const { return origine + direction * longueur*intersection_2;  }
	int GetLast() const { return segmentPrecedent; }
	
	bool SetIntersection(float i) 
	{ 
		float t = intersection_1 + i * (intersection_2 - intersection_1);

		if (t < intersection_2)
		{
			intersection_2 = t;
			return true;
		}
		return false;
	}

	void Affichage(int r, int g, int b);
	void UpdateTexture();
	void Update(Vec2 o, Vec2 d, int l, int last, bool ao, int _lettre);
	
private:
	Vec2 origine;
	Vec2 direction;

	bool affOrigine;

	int longueur;

	int segmentPrecedent;
	int lettre;
	//size_t id;

	float intersection_1;//=intersection_2 du segment qui lui a donne naissance
	float intersection_2;

	SDL_Texture *origine_texture;
	SDL_Texture *intersection_texture;
};

