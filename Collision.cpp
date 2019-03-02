
#include "Collision.h"
#include "RayLight.h"
#include "Vecteur2.h"
#include "Collider.h"


bool Collision::RayMouseCollision(RayLight *ray, Vec2 p, int r)
{
	Vec2 I = ray->GetOrigine();
	Vec2 P = ray->GetIntersection();

	Vec2 u = P - I;

	Vec2 o = ray->GetOrigine();
	Vec2 v = p - o;

	double d = abs(Det(u, v));
	double n = u.Abs();
	double c = d / n;

	if (c >= r)
		return false;

	Vec2 w = p - o - u;
	double pscal1 = Dot(u, v);// produit scalaire    
	double pscal2 = -Dot(u, w);  // produit scalaire    
	if (pscal1 >= 0 && pscal2 >= 0)
		return true;   // I entre A et B, ok.    

	// dernière possibilité, A ou B dans le cercle    		   
	if (v.Abs() < r)
		return true;
	if (w.Abs() < r)
		return true;
	return false;
}

bool Collision::RayCollision(RayLight *ray, SegmentCollider collider)
{
	Vec2 A = collider.GetOrigine();
	Vec2 B = A + collider.GetDirection();

	Vec2 I = ray->GetOrigine();
	Vec2 P = ray->GetIntersection();

	double t = Collision::Vec2Collision(A, B, I, P);

	if (t == -1)
		return false;

	/*il y a intersection avec la droite passant par le rayon
	on check si il y a intersection avec le rayon 
	ie intersection_1 <t <intersection_2*/
	if (ray->SetIntersection(float(t)))
		return true;

	return false;
}

double Collision::Vec2Collision(Vec2 A, Vec2 B, Vec2 I, Vec2 P)
{
	Vec2 D = B - A;
	Vec2 E = P - I;

	double d = Det(D, E);
	if (d == 0)
		return -1;

	double t = (Det(E, A) - Det(E, I)) / d;

	if (t < 0 || t >= 1)
		return -1;

	t = (Det(D, A) - Det(D, I)) / d;

	if (t < 0 || t >= 1)
		return -1;

	return t;//il y a intersection
}

bool Collision::PolygoneCollision(std::vector<Vec2> points, Vec2 P)
{
	Vec2 I(10000, 10000);
	int nbintersections = 0;

	Vec2 A = points[points.size() - 1];
	Vec2 B(0, 0);
	for (unsigned int i(0); i < points.size(); i++)
	{
		B = points[i];
		if (Collision::Vec2Collision(A, B, P, I) == -1)
			nbintersections++;
		A = B;
	}
	return (nbintersections % 2 == 1);//nbintersections est-il impair ?      
}

bool Collision::RectCollision(Vec2 o, Vec2 p, double w, double h)
{
	if (p.x() >= o.x() 
		&& p.x() < o.x() + w 
		&& p.y() >= o.y() 
		&& p.y() < o.y() + h)
		return true;
	return false;
}

Vec2 Collision::SymOrigine(RayLight ray, SegmentCollider collider)
{
	Vec2 o = collider.GetOrigine();
	Vec2 u = ray.GetOrigine() - o;

	double len = u.Abs();
	u.Normalize();
	
	Vec2 d = collider.GetDirection();
	d.Normalize();

	float a = acosf( float(Dot(u, d)));
	if (Det(u, d) < 0)
		a *= -1;
	
	a = a*float( 180 / M_PI);
	u.Rotation(2*a);

	Vec2 S = o + len * u;
	return S;
}

bool Collision::CircleCollision( Vec2 c, int r, Vec2 p)
{
	Vec2 d = c - p;
		
	return (d.Abs() < r);
}

