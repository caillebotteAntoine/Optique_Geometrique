#include "FctGraphiqueVec2.h"
#include "SDL.h"
#include "Vecteur2.h"

#define largeurLinePara 10
#define intervalle 10

void Line(Vec2 p_1, Vec2 p_2, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(Loop::renderer, r, g, b, 255);
	SDL_RenderDrawLine(Loop::renderer, p_1.xint(), p_1.yint(), p_2.xint(), p_2.yint());
}

void Rectangle(Vec2 p, Vec2 d, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(Loop::renderer, r, g, b, 255);

	SDL_Rect rect;
	rect.x = p.xint();
	rect.y = p.yint();

	rect.w = d.xint();
	rect.h = d.yint();

	SDL_RenderFillRect(Loop::renderer, &rect);	
}

void LinePara(Vec2 p_1, Vec2 p_2, Uint8 r, Uint8 g, Uint8 b)
{
	Vec2 d = p_2 - p_1;

	int n = int(d.Abs() / intervalle);
	d = 1.0f/n * d;

	Vec2 o = d.Ortho() - d;
	o.Normalize();
	o = largeurLinePara * o;

	for (int i(0); i < n + 1; i++)
		Line(p_1 + d * i, p_1 + d*i + o, r, g, b);
}


void LineDotted(Vec2 p_1, Vec2 p_2, Uint8 r, Uint8 g, Uint8 b)
{
	Vec2 d = p_2 - p_1;

	int n = int(d.Abs() / intervalle);

	d = (0.5f / n) * d;

	for (int i(0); i < n; i++)
		Line(p_1 + 2 * d * i, p_1 + d*(2 * i + 1), r, g, b);	
}


void Cercle(Vec2 c, int rayon, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(Loop::renderer, r, g, b, 255);

	int d(3 - 2 * rayon);
	int x(0);
	int y(rayon);

	while (y >= x)
	{
		SDL_RenderDrawPoint(Loop::renderer, c.xint() + x, c.yint() + y);
		SDL_RenderDrawPoint(Loop::renderer, c.xint() + y, c.yint() + x);
		SDL_RenderDrawPoint(Loop::renderer, c.xint() - x, c.yint() + y);
		SDL_RenderDrawPoint(Loop::renderer, c.xint() - y, c.yint() + x);
		SDL_RenderDrawPoint(Loop::renderer, c.xint() + x, c.yint() - y);
		SDL_RenderDrawPoint(Loop::renderer, c.xint() + y, c.yint() - x);
		SDL_RenderDrawPoint(Loop::renderer, c.xint() - x, c.yint() - y);
		SDL_RenderDrawPoint(Loop::renderer, c.xint() - y, c.yint() - x);

		if (d < 0)
			d = d + (4 * x) + 6;
		else {
			d = d + 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
}
