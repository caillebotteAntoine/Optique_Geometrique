
#include "Source.h"
#include "Collision.h"

#define longueur 5000

#define delay 500

#define nbrPoint 50
#define pulsasion 6.0
#define amplitude 5

#define decalage 20
#define vitesse 1

class point
{
public:
	point(Vec2 _a, Vec2 _b, size_t i) : a(_a), b(_b), t(0), id(i), d(Vec2::Zero), len(0)
	{
		d = b - a;
		len = d.Abs();
		d.Normalize();
	}
	
	Vec2 GetPosition() { return a + vitesse * t *d; }

	bool GetStat() { return vitesse * t > len; }

	Vec2 Affichage(double k)
	{
		return GetPosition() + (k - nbrPoint/2) * d
			+ amplitude*
			cos((k + t) * pulsasion*M_PI / nbrPoint) *d.Ortho();
	}

	Vec2 d;
	int t;
	size_t id;
private :
	double len;
	Vec2 a;
	Vec2 b;
};

Source::Source(int r, int g, int b) : red(r), green(g), blue(b), keyPress(0), keyUp(false)
{}

void Source::Init()
{
	if (!entity->HasComponent<Position>())
		entity->AddComponent<Position>();

	position = &entity->GetComponent<Position>();
	
	segments.push_back(RayLight(position->position, position->direction, longueur, false, -1));
	affichage.push_back(true);

	nbrSource = 1;
}

void  Source::Affichage()
{
	unsigned int len = segments.size();
	if (len > Loop::step)
		len = Loop::step;

	for (unsigned int i(0); i < len; i++)
		if(affichage[i])
			segments[i].Affichage(red, green, blue);
	
	Vec2 o = position->position;
	Vec2 d = 1.0f / nbrSource *position->direction;

	for(unsigned int i(0); i < nbrSource;i++)
	{
		o = position->position + i*d;

		Cercle(o, tailleSource/2, red, green, blue);

		Vec2 a = Vec2::Left * tailleSource;
		Line(o - a, o + a, red, green, blue);

		a = Vec2::Down * tailleSource;
		Line(o - a, o + a, red, green, blue);

		a = Vec2::Diag * tailleSource;
		Line(o - a, o + a, red, green, blue);

		a = Vec2::AntiDiag * tailleSource;
		Line(o - a, o + a, red, green, blue);
	}

	for (std::list<point>::iterator it = points.begin(); it != points.end(); ++it)
	{
		Vec2 a = (*it).Affichage(0);
		Vec2 b;

		for (int k(0); k < nbrPoint; k++)
		{
			b = (*it).Affichage(k);
			Line(a, b, 255 - red, 255 - green, 255 - blue);
			a = b;
		}
	}
}

void Source::Update()
{
	switch (Loop::event.type)
	{
	case SDL_MOUSEMOTION:
		if (Loop::clic && Loop::Effacer)
			for (unsigned int i(0); i < segments.size(); i++)
				if (Collision::RayMouseCollision(&segments[i], Loop::p, 4))
					affichage[i] = false;
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (Loop::event.button.button)
		{
		case SDL_BUTTON_MIDDLE:
			InitTrainOnde();
			break;
		case SDL_BUTTON_LEFT:
			if(Loop::Effacer)
				for (unsigned int i(0); i < segments.size(); i++)
					if (Collision::RayMouseCollision(&segments[i], Loop::p, 4))
						affichage[i] = false;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYDOWN:
		switch (Loop::event.key.keysym.sym)
		{
		case SDLK_RSHIFT:
			ResetAffichage();
			break;
		case SDLK_SPACE:
			if (keyUp || delay < SDL_GetTicks() - keyPress)
			{
				InitTrainOnde();
				keyPress = SDL_GetTicks();
				keyUp = false;
			}
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		keyUp = true;
		break;
	default:
		break;
	}
		
	while (segments.size() < Loop::step)
	{
		segments.push_back(RayLight(position->position, position->direction, longueur, false, -1));
		affichage.push_back(true);
		Loop::MettreAJour = true;
	}
	
	if (Loop::MettreAJour)
	{
		for (auto& s : segments)
			s.Update(-25*Vec2::Diag, Vec2::Up,1, 0, false,-1);
		
		Manager *manager = entity->manager;
		std::vector<Entity*> colliders = manager->GetGroup(Loop::groupColliders);
		
		Vec2 a = position->position;
		Vec2 b = position->position + position->direction;

		Vec2 d = b - a;

		nbrSource = int(d.Abs() / decalageSource);
		if (nbrSource > segments.size())
			nbrSource = segments.size();
		if (nbrSource <= 0)
			nbrSource = 1;
		
		d = 1.0f / nbrSource * d;
		Vec2 direction = position->direction.Ortho();
		direction.Normalize();

		for (unsigned int i(0); i < nbrSource; i++)
			segments[i].Update(a + i*d, direction, longueur, 0, false, i);
		
		int lastDone = nbrSource;
		unsigned int lettre = nbrSource;
		for (unsigned int i(0); i < nbrSource; i++)
			lastDone = GestionCollision(i, colliders, lastDone, &lettre);
	}
	UpdateTrainOnde();
}

int Source::GestionCollision( unsigned int id, std::vector<Entity*> colliders, unsigned int step, unsigned int* lettre)
{
	RayLight *ray = &segments[id];
	if (!affichage[id])
		return step;

	unsigned int lastFree(step);
	SegmentCollider* s = nullptr;
	Entity* e = nullptr;

	for (auto& c : colliders)
		if (Collision::RayCollision(ray, c->GetComponent<SegmentCollider>()))
			e = c;

	if (e != nullptr && step <Loop::step)
	{
		s = &e->GetComponent<SegmentCollider>();

		size_t tag = s->GetTag();
		if (tag == Loop::ObjetMiroire || tag == Loop::ObjetSeparatrice)
		{
			Vec2 o = Collision::SymOrigine(*ray, *s);
			Vec2 d = ray->GetIntersection() - o;

			segments[lastFree].Update(o, d, longueur, id, true, *lettre);
			*lettre += 1;
			lastFree = GestionCollision(lastFree, colliders, lastFree + 1, lettre);
		}
		
		if (tag == Loop::ObjetSeparatrice && lastFree < Loop::step)
		{
			Vec2 o = ray->GetOrigine();
			Vec2 d = ray->GetIntersection() - o;

			segments[lastFree].Update(o, d, longueur, id, false, *lettre);
			*lettre += 1;
			lastFree = GestionCollision( lastFree, colliders, lastFree + 1, lettre);
		}
		
		if (tag == Loop::ObjetLentilleConvergente)
		{
			/*
			
							^y (repere origine = A)
			A___			|
				\____		|
					 \______|I___			
							|	 \____		
							|		 \
							|
							|
			________________|__________x F(focale)
							|B
							|
							|
							|
							|
							||d (vec dir)	B'	F'
			
			
			*/
			
			Vec2 A = ray->GetOrigine();
			Vec2 B = s->GetOrigine() + 0.5*s->GetDirection();

			B = A + longueur * (B - A);

			Vec2 y = -1 * A + s->GetOrigine();
			Vec2 d = s->GetDirection();
			d.Normalize();

			Vec2 I = s->GetOrigine() + abs(Det(d.Ortho(), y))*d;

			Vec2 F = s->GetOrigine() + 0.5*s->GetDirection() - e->GetComponent<TwoNodeObject>().GetNode();
			F = I + longueur * (F - I);

			double t = Collision::Vec2Collision(A, B, I, F);
			
			if ( t < 1)
			{
				Vec2 o = ray->GetIntersection();
				Vec2 intersection = I + t * (F - I);
				d = intersection - o;

				d.Normalize();
				segments[lastFree].Update(o, d, longueur, id, false,*lettre);
				*lettre += 1;
				lastFree = GestionCollision(lastFree, colliders, lastFree + 1, lettre);
			}
		}
		if (tag == Loop::ObjetLentilleDivergente)
		{
			Vec2 I = ray->GetIntersection();
			Vec2 F = s->GetOrigine() + 0.5*s->GetDirection() + e->GetComponent<TwoNodeObject>().GetNode();
			Vec2 d = I - F;
			d.Normalize();
			segments[lastFree].Update(I, d, longueur, id, false, *lettre);
			*lettre += 1;
			lastFree = GestionCollision(lastFree, colliders, lastFree + 1, lettre);
		}
	}
	return lastFree;
}

int Source::GetNextSegments(int id, unsigned int min)
{
	if (min < nbrSource-1)
		min = nbrSource-1;
	for (unsigned int i(min+1); i < segments.size(); i++)
		if (segments[i].GetLast() == id && affichage[i] )
			return i;

	return Loop::step;
}

void Source::UpdateTrainOnde()
{
	std::list<point>::iterator it;

	for (it = points.begin(); it != points.end(); ++it)
		(*it).t++;

	std::list<point>L;

	unsigned  len = points.size();
	
	for (it = points.begin(); it != points.end()	; ++it)
	{
		if ((*it).GetStat())
		{
			unsigned int searchedId = (*it).id;

			unsigned int id(searchedId);

			id = GetNextSegments(searchedId, id);

			while (id < Loop::step)
			{
				RayLight ray = segments[id];
				
				L.push_back(point(ray.GetOrigine(), ray.GetIntersection(), id));

				id = GetNextSegments(searchedId, id);
			}
		}
		else if ( !Loop::OutOfWindows((*it).GetPosition()))
		{
			L.emplace(L.begin(), *it);
		}
	}

	points.swap(L);

}

void Source::InitTrainOnde()
{
	if (segments.size() > nbrSource)
		for (unsigned int i(0); i < nbrSource; i++)
			if (affichage[i])
			{
				RayLight ray = segments[i];
				points.push_back(point(ray.GetOrigine(), ray.GetIntersection(), i));
			}
}