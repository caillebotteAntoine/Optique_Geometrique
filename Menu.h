#pragma once

#include "Vecteur2.h"
#include "SDL.h"
#include <vector>


class Bouton
{
public:
	Bouton(Vec2 p, Vec2 d, size_t i, const char* t);

	Bouton(size_t i, const char* t);

	void Init(Vec2 p, int i);

	bool Update(Vec2 p);

	void Affichage();

	size_t GetId() const { return id; }
	const char* GetTag() const { return tag; }



private:
	Vec2 position;
	Vec2 dimension;

	bool motion;

	size_t id;
	const char* tag;
	SDL_Texture *tag_texture;
};


class Menu
{
public:
	Menu();
	Menu(int i);

	void Update();

	void Affichage();

	size_t GetId() const { return id; }
	void SetId(size_t i) { id = i; }

	bool Clic();

	void InitMenu1();
	void InitMenu2();
	void InitMenuMain();
	void InitMenuInfo();

	void SetPosition(Vec2 p) 
	{ 
		position = p; 

		for (unsigned int i(0); i < boutons.size(); i++)
		boutons[i].Init(decalage + position, i);
	}

	size_t SubMenuClic(Vec2 p, size_t r);

private:
	void GestionBouton(size_t b);

	std::vector<Bouton> boutons;
	std::vector<Menu> subMenu;

	Vec2 decalage;
	Vec2 position;
	Vec2 dimension;

	size_t id;

	bool keyUp;
	bool activate;


};
