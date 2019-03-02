#pragma once
#include "SDL.h"

class Vec2;

class Loop
{
public:
	int Init(const char *title, int xpos, int ypos, int width, int height);

	void ManageEvent();
	void UpDate();
	void Render();
	void Clean();
	
	enum groupLabels : size_t
	{
		null,
		groupColliders,
		groupNonColliders,
		ObjetSourceLumineuse,
		ObjetEcran,
		ObjetMiroire,
		ObjetSeparatrice,
		ObjetLentilleConvergente,
		ObjetLentilleDivergente
	};
	
	static bool OutOfWindows(Vec2 p);

	static SDL_Renderer *renderer;
	static SDL_Event event;

	static unsigned int step;
	static const char* fps;

	static Vec2 p;

	static void AddOneNodeObject(Vec2 p, size_t tag);
	static void AddSource(Vec2 p);
	static void AddLentille(Vec2 p, size_t tag);
	static void AddMichelson(Vec2 p);

	static void Reset();
	static void ResetAffichage();
	static void UpdateTexture();
	
	static bool isRunning;
	static bool rayonsDeConstruction;
	static bool origine;
	static bool Lettre;
	static bool Effacer;
	//pour les mouvements de la souris avec maintient du clic 
	static bool clic;
	//il y a eut un mouvement un collider -> mettre a jour les sources
	static bool MettreAJour;
	//passage ecran plein/fenetre
	static void SwitchFullScreen();
	static void SwitchCouleurFond();

	static bool couleurFond;

private :	
	static int nbrSource;

	bool keyUp;
	int keyPress;
	
	int cpt;
		
	static SDL_Window *window;
	SDL_Texture *fps_texture;
};
