#pragma once

#include "AffichageText.h"
#include "RayLight.h"
#include "FctGraphiqueVec2.h"
#include <sstream>

#define dis 18

const char* alphabet[26] = { "A" ,"B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };

RayLight::RayLight(Vec2 o, Vec2 d, int l, bool a, int last) : 
	origine(o), longueur(l),
	direction(d.Normalize()),
	intersection_1(0.0f), intersection_2(1.0f),
	//affOrigine(a), 
	segmentPrecedent(last), lettre(0)
{
	origine_texture = Text::CreateTextTexture(Loop::renderer, "c", true);
	intersection_texture = Text::CreateTextTexture(Loop::renderer, "c", false);
}

void RayLight::Update(Vec2 o, Vec2 d, int l, int last, bool ao, int _lettre)
{
	origine = o;
	longueur = l; 
	intersection_1 = float(d.Abs() / l) + 1.0f / l;
	intersection_2 = 1.0f;
	direction = d.Normalize();
	affOrigine = ao;
	segmentPrecedent = last;
	lettre = _lettre;

	if (Loop::Lettre)
		UpdateTexture();
}

void RayLight::UpdateTexture()
{
	std::stringstream temp;
	temp << segmentPrecedent;
	std::string str = temp.str();

	SDL_DestroyTexture(origine_texture);
	origine_texture = Text::CreateTextTexture(Loop::renderer, str.c_str(), true);

	if (lettre != -1)
	{
		SDL_DestroyTexture(intersection_texture);
		intersection_texture = Text::CreateTextTexture(Loop::renderer, alphabet[lettre % 26], false);
	}
}

void RayLight::Affichage(int r, int g, int b)
{
	Vec2 a = GetOrigine();
	Vec2 i = GetIntersection();

	Line( a, i, r, g, b);
	
	Vec2 d = i - a;
	d.Normalize();
	//Loop::DisplayText(origine_texture, i +  dis*d);

	if(affOrigine)
	{
		if(Loop::rayonsDeConstruction)
			LineDotted( origine, a, 255, 255, 0);

		if (Loop::origine)
		{
			Cercle(origine, 2, 0, 255, 255);

			if (Loop::Lettre)
				Text::DisplayText(Loop::renderer, origine_texture, origine - dis*d);
		}
	}		
	if(Loop::Lettre && lettre != -1)
		Text::DisplayText(Loop::renderer, intersection_texture, i - dis*d);
}