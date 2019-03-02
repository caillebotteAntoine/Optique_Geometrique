#pragma once
#include <iostream>  

class Vec2
{
public:
	Vec2();
	Vec2(double x, double y);
	
	const double x() const { return _x; }
	const int xint() const { return int(_x); }

	const double y() const { return _y; }
	const int yint() const { return int(_y); }

	void operator+=(const Vec2&);

	double const Abs();
	Vec2 Normalize();
	Vec2 const Ortho();
	Vec2 const Rotation(float angle);

	static double AngleBetween(Vec2, Vec2);
	static Vec2 Smouth(Vec2 v, int n, double e);

	//(0,0)
	static Vec2 Zero;
	//(0,1)
	static Vec2 Up;
	//(0,-1)
	static Vec2 Down;
	//(-1,0)
	static Vec2 Left;
	//(1,0)
	static Vec2 Right;
	//(1,1)
	static Vec2 Diag;
	//(1,-1)
	static Vec2 AntiDiag;


private :
	double _x;
	double _y;

};

Vec2 operator+(const Vec2&, const Vec2&);
Vec2 operator-(const Vec2&, const Vec2&);
Vec2 operator*(const Vec2&, const double);
Vec2 operator*(const double, const Vec2&);
bool operator==(const Vec2&, const Vec2&);


std::ostream& operator<<(std::ostream& os, const Vec2& v);

double Det(const Vec2&, const Vec2&);
double Dot(const Vec2&, const Vec2&);