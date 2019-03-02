#pragma once

#include <vector>
#include "Vecteur2.h"
#include "Position.h"
#include "FctGraphiqueVec2.h"
#include "Collision.h"

#define rayonNode 5

class Node
{
public :
	Node() {}

	void Affichage(Vec2 position)
	{
		if (activate)
		{
			if(Loop::couleurFond)
				Cercle(position + node, rayonNode, 255, 255, 255);
			else
				Cercle(position + node, rayonNode, 0, 0,0 );
		}
	}

	void On() { activate = true; }
	void Off() { activate = false; }
	bool GetStat() const { return activate; }

	bool GetMoveIn() const { return moveIn; }
	bool GetClic() const { return clic; }
	void SetNode(Vec2 p) { node = p; }
	Vec2 GetNode() const { return node; }


	void Update(Vec2 position)
	{
		node = node + position;
		if (Loop::event.type == SDL_MOUSEMOTION)
		{
			if(clic)
				node = Vec2(Loop::event.motion.x, Loop::event.motion.y);

			moveIn = false;
			if (Collision::CircleCollision(node, rayonNode, Loop::p))
				moveIn = true;
		}
		if (clic && Loop::event.type == SDL_MOUSEBUTTONUP)
		{
			if (Loop::event.button.button == SDL_BUTTON_LEFT)
				clic = false;
		}
		if (!clic && Loop::event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (Loop::event.button.button == SDL_BUTTON_LEFT)
			{
				if (Collision::CircleCollision(node, rayonNode, Loop::p))
					clic = true;
			}
		}

		node = node - position;
	}


private :
	Vec2 node;

	bool activate;

	bool moveIn;
	bool clic;

};

class Rect
{
public:
	Rect() {}

	void On() { activate = true; }
	void Off() { activate = false; }

	bool GetMoveIn() const { return moveIn; }
	bool GetClic() const { return clic; }
	Vec2 GetClicPostion() const { return clicPostion; }

	void Affichage()
	{
		if (activate)
		{
			Vec2 A = points[points.size() - 1];
			Vec2 B(0, 0);
			for (unsigned int i(0); i < points.size(); i++)
			{
				B = points[i];

				if (Loop::couleurFond)
					Line(A, B, 255, 255, 255);
				else
					Line(A, B, 0, 0, 0);
				A = B;
			}
		}
	}

	void Update(Vec2 o)
	{
		if (Loop::event.type == SDL_MOUSEMOTION)
		{
			moveIn = false;
			Vec2 p(Loop::event.motion.x, Loop::event.motion.y);
			if (Collision::PolygoneCollision(points, p))
				moveIn = true;
		}

		if (clic && Loop::event.type == SDL_MOUSEBUTTONUP)
		{
			if (Loop::event.button.button == SDL_BUTTON_LEFT)
				clic = false;
		}
		if (!clic && Loop::event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (Loop::event.button.button == SDL_BUTTON_LEFT)
			{
				Vec2 p(Loop::event.button.x, Loop::event.button.y);
				if (Collision::PolygoneCollision(points, p))
				{
					clicPostion = o - p;
					clic = true;
				}
			}
		}
	}

	void UpdateRect(Vec2 a, Vec2 d, int w, int h)
	{
		b = a + d;
		d.Normalize();

		points.clear();
		points.push_back( a + d.Ortho()*w - h*d);
		points.push_back(a - d.Ortho()*w - h*d);
		points.push_back(b - d.Ortho()*w + h*d);
		points.push_back(b + d.Ortho()*w + h*d);
	}

private:
	bool activate;

	bool moveIn;
	bool clic;
	Vec2 clicPostion;

	Vec2 a;
	Vec2 b;
	
	std::vector<Vec2> points;

};
