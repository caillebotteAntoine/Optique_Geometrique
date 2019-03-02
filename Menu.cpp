#pragma once

#include "AffichageText.h"
#include "Menu.h"
#include "FctGraphiqueVec2.h"
#include "Collision.h"
#include "Loop.h"

#define hauteur 15
#define epaisseur 3
#define longueur 132


Bouton::Bouton(Vec2 p, Vec2 d, size_t i, const char* t) :position(p), dimension(d), id(i), tag(t), motion(false)
{
	tag_texture = Text::CreateTextTexture(Loop::renderer, t, true);
}

Bouton::Bouton(size_t i, const char* t) :position(Vec2::Zero), dimension(Vec2::Zero), id(i), tag(t) 
{ 
	tag_texture = Text::CreateTextTexture(Loop::renderer, t, true);
}

void Bouton::Init(Vec2 p, int i)
{
	position = p + Vec2(epaisseur, epaisseur +epaisseur * i+ hauteur*i);

	dimension = Vec2(longueur, hauteur);
}

bool Bouton::Update(Vec2 p)
{
	motion = Collision::RectCollision(position, p, dimension.x(), dimension.y());
	return motion;
}

void Bouton::Affichage()
{
	Vec2 a = position;
	Vec2 b = position + dimension.x() * Vec2::Right;
	Vec2 c = position + dimension;
	Vec2 d = position + dimension.y() * Vec2::Up;

	Text::DisplayText(Loop::renderer, tag_texture, position+Vec2::Diag*2+Vec2::Right);

	if (motion)
	{
		Line(a, b, 0, 255, 0);
		Line(c, b, 0, 255, 0);
		Line(c, d, 0, 255, 0);
		Line(a, d, 0, 255, 0);
	}
	else
	{
		Line(a, b, 255, 255, 255);
		Line(c, b, 255, 255, 255);
		Line(c, d, 255, 255, 255);
		Line(a, d, 255, 255, 255);
	}
}


Menu::Menu() : activate(false), id(0), keyUp(false)
{}
Menu::Menu( int i) : activate(false), id(i), keyUp(false)
{}

void Menu::InitMenuMain()
{
	subMenu.push_back(Menu(1));
	subMenu.push_back(Menu(2));
	subMenu.push_back(Menu(3));

	subMenu[0].InitMenu1();
	subMenu[1].InitMenu2();
	subMenu[2].InitMenuInfo();

	boutons.clear();
	boutons.push_back(Bouton(1, "Ajouter                          >"));
	boutons.push_back(Bouton(2, "Afficher                         >"));
	boutons.push_back(Bouton(3, "Reset"));
	boutons.push_back(Bouton(4, "Plein ecran"));
	boutons.push_back(Bouton(5, "Info                                >"));
	boutons.push_back(Bouton(6, "Quitter"));

	for (unsigned int i(0); i < boutons.size(); i++)
		boutons[i].Init(decalage + position, i);

	decalage = Vec2::Zero;
	dimension = Vec2(longueur + 2 * epaisseur, boutons.size() * (hauteur + epaisseur) + epaisseur);
}

void Menu::InitMenu1()
{
	boutons.clear();
	boutons.push_back(Bouton(1, "LASER"));
	boutons.push_back(Bouton(2, "Miroir"));
	boutons.push_back(Bouton(3, "Separatrice"));
	boutons.push_back(Bouton(4, "Lentille Convergente"));
	boutons.push_back(Bouton(5, "Lentille Divergente"));
	boutons.push_back(Bouton(6, "Ecran"));
	boutons.push_back(Bouton(7, "Michelson"));

	for (unsigned int i(0); i < boutons.size(); i++)
		boutons[i].Init(decalage + position, i);

	decalage = Vec2(longueur + 2 * epaisseur, 0);
	dimension = Vec2(longueur + 2 * epaisseur, boutons.size() * (hauteur + epaisseur) + epaisseur);
}
void Menu::InitMenu2()
{
	boutons.clear();
	boutons.push_back(Bouton(1, "Rayons de construction"));
	boutons.push_back(Bouton(2, "Origine des rayons"));
	boutons.push_back(Bouton(3, "Nom des points"));
	boutons.push_back(Bouton(4, "Gomme"));
	boutons.push_back(Bouton(5, "ReAfficher les rayons"));
	boutons.push_back(Bouton(6, "Fond : noir / blanc"));
	
	for (unsigned int i(0); i < boutons.size(); i++)
		boutons[i].Init(decalage + position, i);

	decalage = Vec2(longueur + 2 * epaisseur, hauteur + epaisseur);
	dimension = Vec2(longueur + 2 * epaisseur, epaisseur + boutons.size() * (hauteur + epaisseur));
}
void Menu::InitMenuInfo()
{
	boutons.clear();
	boutons.push_back(Bouton(1, "Caillebotte Antoine"));
	boutons.push_back(Bouton(2, "2018"));

	for (unsigned int i(0); i < boutons.size(); i++)
		boutons[i].Init(decalage + position, i);

	decalage = Vec2(longueur + 2 * epaisseur, 4*(hauteur + epaisseur));
	dimension = Vec2(longueur + 2 * epaisseur, epaisseur + boutons.size() * (hauteur + epaisseur));
}

bool Menu::Clic()
{
	return Collision::RectCollision(position, Loop::p, dimension.x(), dimension.y());
}

size_t Menu::SubMenuClic(Vec2 p, size_t r)
{
	size_t res = r;
	for (auto &b : boutons)
	{
		if (b.Update(Loop::p))
			res = b.GetId();
	}
	return res;
}

void Menu::Update()
{
	if (activate)
	{
		for (auto &b : boutons)
			b.Update(Loop::p);

		for (auto& m : subMenu)
			m.Update();
	}

	switch (Loop::event.type)
	{
	case SDL_MOUSEMOTION : 
	case SDL_MOUSEBUTTONDOWN:
		keyUp = true;
		break;

	case SDL_MOUSEBUTTONUP:
		switch (Loop::event.button.button)
		{
		case SDL_BUTTON_LEFT:
			if (activate)
			{
				size_t res = 0;
				if(keyUp)
				{
					res = SubMenuClic(Loop::p, res);

					keyUp = false;
				}
				if (res != 0)
				{
					for (auto& m : subMenu)
						m.activate = false;
					GestionBouton(res);
				}
				else if(id == 0)
				{
					int t = 0;

					if (Clic())
						t += 1;

					for (auto m : subMenu)
						if (Clic())
							t += 1;

					if (t == 0)
					{
						activate = false;
					}
				}
			}
			break;
		case SDL_BUTTON_RIGHT:
			if (keyUp)
			{
				if (id == 0)
					activate = true;
				else
					activate = false;

				SetPosition(Loop::p);

				for (auto& m : subMenu)
					m.SetPosition(Loop::p);


				keyUp = false;
			}
			break;
		default:
			break;
		}

		break;
	default:
		break;
	}


}

void Menu::Affichage()
{
	if (activate)
	{		
		Rectangle(decalage + position, dimension, 0, 0, 0);
		
		for (auto &b : boutons)
			b.Affichage();

		Vec2 a = decalage + position;
		Vec2 b = decalage + position + dimension.x() * Vec2::Right;
		Vec2 c = decalage + position + dimension;
		Vec2 d = decalage + position + dimension.y() * Vec2::Up;

		Line(a, b, 255, 255, 255);
		Line(c, b, 255, 255, 255);
		Line(c, d, 255, 255, 255);
		Line(a, d, 255, 255, 255);

		for (auto m : subMenu)
			m.Affichage();
	}
}

void Menu::GestionBouton(size_t b)
{
	switch (id)
	{
	case 0://menu principale
		switch (b)
		{
		case 1://Ajouter...
			subMenu[0].activate = true;
			break;
		case 2://Afficher..
			subMenu[1].activate = true;
			break;
		case 3://Reset
			Loop::Reset();
			break;
		case 4://Pleine ecran
			Loop::SwitchFullScreen();
			break;
		case 5://Info
			subMenu[2].activate = true;
			break;
		case 6://Quitter
			Loop::isRunning = false;
			break;
		default:
			break;
		}
		break;
	case 1://menu d'ajout
		activate = false;
		switch (b)
		{
		case 1:
			Loop::AddSource(position);
			break;
		case 2:
			Loop::AddOneNodeObject(position, Loop::ObjetMiroire);
			break;
		case 3:
			Loop::AddOneNodeObject(position, Loop::ObjetSeparatrice);
			break;
		case 4:
			Loop::AddLentille(position, Loop::ObjetLentilleConvergente);
			break;
		case 5:
			Loop::AddLentille(position, Loop::ObjetLentilleDivergente);
			break;
		case 6:
			Loop::AddOneNodeObject(position, Loop::ObjetEcran);
			break;
		case 7:
			Loop::AddMichelson(position);
			break;
		default:
			break;
		}
		break;
	case 2://menu d'ajout
		activate = false;
		switch (b)
		{
		case 1:
			Loop::rayonsDeConstruction = !Loop::rayonsDeConstruction;
			break;
		case 2:
			Loop::origine = !Loop::origine;
			break;
		case 3:
			Loop::Lettre = !Loop::Lettre;
			Loop::UpdateTexture();
			break;
		case 4:
			Loop::Effacer = !Loop::Effacer;
			break;
		case 5:
			Loop::ResetAffichage();
			break;
		case 6:
			Loop::SwitchCouleurFond();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
