#include "Loop.h"
#include "AffichageText.h"
#include "Component.h"
#include "Menu.h"
#include "Vecteur2.h"
#include <ctime>

//#include "Collision.h"
//#include <iostream>

#define maxStep 1000
#define delay 800

#define black 0,0,0
#define white 255,255,255

#define red 255,0,0
#define blue 0,0,255
#define green 0,255,0
#define cyan 0,255,255
#define orange 255,128,0
#define purple 128,0,128
#define yellow 255,255,0

Manager manager;
Menu m;

SDL_Renderer* Loop::renderer = nullptr;
SDL_Window* Loop::window = nullptr;
SDL_Event Loop::event;
bool Loop::isRunning = true;
Vec2 Loop::p;
bool Loop::couleurFond = true;
int Loop::nbrSource;
bool Loop::rayonsDeConstruction = true;
bool Loop::origine = true;
bool Loop::Lettre = false;
bool Loop::MettreAJour = true;
bool Loop::Effacer = false;

bool Loop::clic = false;

unsigned int Loop::step = 250;
const char* Loop::fps = "120";

bool fullscreen = false;

int Loop::Init(const char *title, int xpos, int ypos, int width, int height)
{
	isRunning = false;

	// Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return -1;
	}
	// Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return -1;
	}
	// Initialisation de la TTF
	if (TTF_Init() == -1)
	{
		std::cout << "Init TTF  failt : " << SDL_GetError() << std::endl;
		return -1;
	}
	if (!Text::Init())
		return -1;

	window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_RESIZABLE);
	if (window == 0)
	{
		std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if( renderer == 0)
	{
		std::cout << "Erreur lors de la creation du renderer : " << SDL_GetError() << std::endl;
		SDL_Quit();

		return -1;
	}

	isRunning = true;
	keyUp = true;

	keyPress = 0;

	nbrSource = 0;
	fullscreen = false;
	cpt = 0;
	fps_texture = Text::CreateTextTexture(Loop::renderer, fps, false);

	m.InitMenuMain();

	return 0;
}

void Loop::ManageEvent()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		if (Loop::event.button.button == SDL_BUTTON_LEFT)
			Loop::clic = (Loop::event.type == SDL_MOUSEBUTTONDOWN);
		break;
	case SDL_MOUSEMOTION:
		Loop::p = Vec2(Loop::event.motion.x, Loop::event.motion.y);
		break;
	case SDL_QUIT:
		Loop::isRunning = false;
		break;
	case SDL_KEYDOWN :
		if (keyUp || delay < SDL_GetTicks() - keyPress)
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				step++;
				if (step > maxStep)
					step = maxStep;
				break;
			case SDLK_BACKSPACE:
				step--;
				if (step <= 0)
					step = 1;
				break;
			case SDLK_F11:
				SwitchFullScreen();
				break;
			default:
				break;
			}
		if(keyUp)
			keyUp = false;
		break;
	case SDL_KEYUP :
		keyUp = true;
		keyPress = SDL_GetTicks();

		if(event.key.keysym.sym == SDLK_ESCAPE)
			isRunning = false;
		break;
	default:
		break;
	}
}

void Loop::UpDate()
{
	m.Update();

	manager.Update(groupColliders);
	manager.Update(groupNonColliders);

	MettreAJour = false;

	cpt++;	
}

void Loop::Render()
{
	if (Loop::couleurFond)
		SDL_SetRenderDrawColor(renderer, black, 255);
	else
		SDL_SetRenderDrawColor(renderer, white, 255);

	SDL_RenderClear(renderer);

	manager.Affichage();
	m.Affichage();
		
	SDL_DestroyTexture(fps_texture);
	fps_texture = Text::CreateTextTexture(Loop::renderer, fps, false);
	Text::DisplayText(Loop::renderer, fps_texture, Vec2::Diag * 10);

	if (Effacer)
		Cercle(Loop::p, 4, 255, 255, 255);

	SDL_RenderPresent(renderer);
}

void Loop::Clean()
{
	SDL_DestroyTexture(fps_texture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	Text::Clean();
	TTF_Quit();

	SDL_Quit();
}

bool Loop::OutOfWindows(Vec2 p)
{
	int w;
	int h;

	SDL_GetWindowSize(window, &w, &h);

	if (p.x() > w || p.x() < 0)
		return true;

	if (p.y() > h || p.y() < 0)
		return true;

	return false;
}

void Loop::AddOneNodeObject(Vec2 p, size_t tag)
{
	MettreAJour = true;
	auto& object = manager.AddEntity();
	
	object.AddComponent<Position>(p, Vec2::Down);
	object.AddComponent<SegmentCollider>(tag);
	object.AddComponent<OneNodeObject>();
	//object.AddComponent<RectWithTwoNode>();

	object.AddGroup(groupColliders);
}

void Loop::AddSource(Vec2 p)
{
	MettreAJour = true;
	auto& object = manager.AddEntity();

	object.AddComponent<Position>(p, Vec2::Down);
	object.AddComponent<OneNodeObject>(ObjetSourceLumineuse);

	switch (nbrSource % 7)
	{
	case 0:
		object.AddComponent<Source>(red);
		break;
	case 1:
		object.AddComponent<Source>(blue);
		break;
	case 2:
		object.AddComponent<Source>(green);
		break;
	case 3:
		object.AddComponent<Source>(purple);
		break;
	case 4:
		object.AddComponent<Source>(cyan);
		break;
	case 5:
		object.AddComponent<Source>(orange);
		break;
	case 6:
		object.AddComponent<Source>(yellow);
		break;

	default:
		object.AddComponent<Source>(red);
		break;
	}
	object.AddGroup(groupNonColliders);

	nbrSource++;
}

void Loop::AddLentille(Vec2 p, size_t tag)
{
	Loop::MettreAJour = true;
	auto& object = manager.AddEntity();

	object.AddComponent<Position>(p, Vec2::Up);
	object.AddComponent<TwoNodeObject>();
	object.AddComponent<SegmentCollider>(tag);

	object.AddGroup(groupColliders);
}

void Loop::AddMichelson(Vec2 p)
{
	Loop::MettreAJour = true;
	Vec2 a = Vec2::Zero;

	a = Vec2(-2 * defaultfocale+15, 6 * defaultfocale);
	auto& source = manager.AddEntity();

	source.AddComponent<Position>(p + a, Vec2(-0.211,-0.976));
	source.AddComponent<OneNodeObject>(ObjetSourceLumineuse);
	source.AddComponent<Source>(red);
	source.AddGroup(groupNonColliders);
	
	a = 2* Vec2(0, 3*defaultfocale);
	auto& separatrice = manager.AddEntity();

	separatrice.AddComponent<Position>(p + a, Vec2::AntiDiag);
	separatrice.AddComponent<OneNodeObject>();
	separatrice.AddComponent<SegmentCollider>(ObjetSeparatrice);
	separatrice.AddGroup(groupColliders);

	a =  2*Vec2(3*defaultfocale, 6* defaultfocale);
	auto& lentille = manager.AddEntity();

	lentille.AddComponent<Position>(p + a, Vec2::Left);
	lentille.AddComponent<SegmentCollider>(ObjetLentilleConvergente);
	lentille.AddComponent<TwoNodeObject>();
	lentille.AddGroup(groupColliders);

	a = 6 * Vec2(0, 3*defaultfocale);
	auto& ecran = manager.AddEntity();

	ecran.AddComponent<Position>(p + a, Vec2::Right);
	ecran.AddComponent<OneNodeObject>();
	ecran.AddComponent<SegmentCollider>(ObjetEcran);
	ecran.AddGroup(groupColliders);
	

	a = 2 * Vec2(3*defaultfocale,0);
	auto& m_1 = manager.AddEntity();

	m_1.AddComponent<Position>(p + a, Vec2::Left);
	m_1.AddComponent<OneNodeObject>();
	m_1.AddComponent<SegmentCollider>(ObjetMiroire);

	m_1.AddGroup(groupColliders);

	a = Vec2(8*defaultfocale, 6*defaultfocale);
	auto& m_2 = manager.AddEntity();

	m_2.AddComponent<Position>(p + a, Vec2::Down);
	m_2.AddComponent<OneNodeObject>();
	m_2.AddComponent<SegmentCollider>(ObjetMiroire);

	m_2.AddGroup(groupColliders);
}

void Loop::Reset()
{
	manager = Manager();
	nbrSource = 0;
}

void Loop::ResetAffichage()
{
	std::vector<Entity*>group = manager.GetGroup(Loop::groupNonColliders);
	for (auto &e : group)
		if (e->HasComponent<Source>())
			e->GetComponent<Source>().ResetAffichage();
}

void Loop::UpdateTexture()
{
	std::vector<Entity*>group = manager.GetGroup(Loop::groupNonColliders);
	for (auto &e : group)
		if (e->HasComponent<Source>())
			e->GetComponent<Source>().UpdateTexture();
}

void Loop::SwitchFullScreen()
{
	if (!fullscreen)
	{
		fullscreen = true;
		SDL_SetWindowFullscreen(Loop::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		fullscreen = false;
		SDL_SetWindowFullscreen(Loop::window, SDL_WINDOW_RESIZABLE);
	}
}

void Loop::SwitchCouleurFond()
{
	Loop::couleurFond = !Loop::couleurFond;
}