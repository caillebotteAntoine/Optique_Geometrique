#include "Vecteur2.h"
#include <cmath>// importation de cos et sin

#define PI 3.14159265

Vec2 Vec2::Zero = Vec2(0, 0);
Vec2 Vec2::Up = Vec2(0, 1);
Vec2 Vec2::Down = Vec2(0, -1);
Vec2 Vec2::Left = Vec2(-1, 0);
Vec2 Vec2::Right = Vec2(1, 0);
Vec2 Vec2::Diag = Vec2(sqrtf(0.5f), sqrtf(0.5f));
Vec2 Vec2::AntiDiag = Vec2(sqrtf(0.5f), -sqrtf(0.5f));

Vec2::Vec2(): _x(0.0), _y(0.0) {}

Vec2::Vec2(double x, double y):  _x(x), _y(y) {}

double const Vec2::Abs()
{
	double n = x()*x() + y()*y();
	return std::sqrt(n);
}

Vec2 Vec2::Normalize()
{
	double len(Abs());
	_x /= len;
	_y /= len;

	return *this;
}

Vec2 const Vec2::Ortho() 
{
	Vec2 o(-y(), x());
	return o;
}

Vec2 const Vec2::Rotation(float angle)
{
	double c = cos(angle * PI / 180.0);
	double s = sin(angle * PI / 180.0);

	double x(_x*c - _y*s);
	double y(_x*s + _y*c);

	_x = x;
	_y = y;

	return *this;
}

Vec2 Vec2::Smouth(Vec2 v, int n, double e)
{
	Vec2 ref = Vec2::Right;
	Vec2 d;
	int angle = 360 / n;


	for (int i(0); i < n+1; i++)
	{
		d = ref - v;
		if (d.Abs() < e)
			return ref;

		ref.Rotation(float(angle));
	}
	return v;
}

void Vec2::operator+=(const Vec2& a)
{
	_x += a.x();
	_y += a.y();
}

double Vec2::AngleBetween(Vec2 from, Vec2 to)
{
	double det = Det(from, to);
	double dot = Dot(from, to);
	Vec2 a = from - to;

	double angle = atan2(a.x(), a.y());
	return angle;
}

Vec2 operator+(const Vec2& a, const Vec2& b)
{
	Vec2 c(a.x() + b.x(), a.y() + b.y());

	return c;
}

Vec2 operator-(const Vec2& a, const Vec2& b)
{
	Vec2 c(a.x() - b.x(), a.y() - b.y());

	return c;
}

Vec2 operator*(const Vec2& a, const double s)
{
	Vec2 c(s*a.x(), s*a.y());

	return c;
}

Vec2 operator*(const double s, const Vec2& a)
{
	Vec2 c(s*a.x(), s*a.y());

	return c;
}

std::ostream& operator<<(std::ostream& os, const Vec2& v)
{
	os << "(" << v.x() << "," << v.y() << ')';
	return os;
}

bool operator==(const Vec2& a, const Vec2& b)
{
	if (a.x() == b.x() && a.y() == b.y())
		return true;
	return false;
}

double Det(const Vec2& a, const Vec2& b)
{
	double d = a.x()*b.y() - b.x() * a.y();

	return d;
}

double Dot(const Vec2& a, const Vec2& b)
{
	double d = a.x()*b.x() + b.y() * a.y();

	return d;
}