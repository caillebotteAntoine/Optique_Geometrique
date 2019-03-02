#pragma once

#include <vector>

class Vec2;
class RayLight;
class Collider;
class SegmentCollider;

class Collision
{
public:	
	static Vec2 SymOrigine(RayLight ray, SegmentCollider collider);

	static bool RayCollision(RayLight *ray, SegmentCollider collider);
	static bool RayMouseCollision(RayLight *ray, Vec2 p, int r);

	static double Vec2Collision(Vec2 A, Vec2 B, Vec2 I, Vec2 P);

	static bool PolygoneCollision(std::vector<Vec2> points, Vec2 P);
	static bool RectCollision(Vec2 o, Vec2 p, double w, double h);

	static bool CircleCollision(Vec2 c, int r, Vec2 p);

};

