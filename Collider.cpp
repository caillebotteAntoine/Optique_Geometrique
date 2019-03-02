
#include "Collider.h"

#define distance 5
#define white 255
#define black 0

void SegmentCollider::Affichage()
{
	Vec2 a = GetOrigine();
	Vec2 d = GetDirection();

	Vec2 e = a + d;

	Uint8 r(0), g(0), b(0);
	if (Loop::couleurFond)
	{
		r = white;
		g = white;
		b = white;
	}
	else
	{
		r = black;
		g = black;
		b = black;
	}

	Line(a, e, r, g, b);

	if (tag == Loop::ObjetMiroire)
		LinePara(a, e, r, g, b);

	if (tag == Loop::ObjetLentilleConvergente)
	{
		d.Normalize();
		d = 2 * distance * d;
		Vec2 angle = d.Rotation(-45 - 90);
		Line(e, e + angle, r, g, b);

		angle = d.Rotation(90 + 180);
		Line(e, e + angle, r, g, b);

		angle = d.Rotation(-90);
		Line(a, a + angle, r, g, b);

		angle = d.Rotation(-90);
		Line(a, a + angle, r, g, b);
	}

	if (tag == Loop::ObjetLentilleDivergente)
	{
		d.Normalize();
		d = 2 * distance * d;
		Vec2 angle = d.Rotation(-45 - 90);
		Line(a, a + angle, r, g, b);

		angle = d.Rotation(90 + 180);
		Line(a, a + angle, r, g, b);

		angle = d.Rotation(-90);
		Line(e, e + angle, r, g, b);

		angle = d.Rotation(-90);
		Line(e, e + angle, r, g, b);
	}
}

void SegmentCollider::Init()
{
	if (!entity->HasComponent<Position>())
		entity->AddComponent<Position>();

	position = &entity->GetComponent<Position>();
}


void OneNodeObject::Affichage()
{
	node.Affichage(position->position);
	rect.Affichage();
}

void OneNodeObject::Init()
{
	if (!entity->HasComponent<Position>())
		entity->AddComponent<Position>();

	position = &entity->GetComponent<Position>();

	Vec2 d = position->direction.Normalize();
	position->direction = 6* defaultfocale*d.Normalize();

	if (tag == Loop::null)
	{
		if (entity->HasComponent<SegmentCollider>())
			tag = entity->GetComponent<SegmentCollider>().GetTag();

		if (entity->HasComponent<Source>())
			tag = Loop::ObjetSourceLumineuse;
	}
	
	if (tag == Loop::ObjetSourceLumineuse)
		position->direction = decalageSource *position->direction.Normalize();

	node.SetNode(Vec2::Up* (position->direction.Abs() + 3 * distance));

	UpdateNodeAndRect();
}

void OneNodeObject::Update()
{
	actu = false;
	node.Update(position->position);

	node.Off();
	if(node.GetMoveIn())
		node.On();
	if (node.GetClic())
	{
		Vec2 d = node.GetNode();

		float len = float(d.Abs());
		d.Normalize();

		Vec2 m = position->position + 0.5 * position->direction;

		d = Vec2::Smouth(d, 24, 0.04);
		position->direction = (len - 3*distance)* d;
				
		if(tag == Loop::ObjetSourceLumineuse)
		{
			int nbrSource =  int(len / decalageSource)-1;
			if (nbrSource < 1)
				nbrSource = 1;
			position->direction =( nbrSource *decalageSource + 1)* d;
		}
		else
			position->position = m - 0.5* position->direction;
		
		UpdateNodeAndRect();
		node.On();
	}
	else
	{
		rect.Update(position->position);

		rect.Off();
		if (rect.GetMoveIn())
			rect.On();
		if (rect.GetClic())
		{
			position->position = Loop::p + rect.GetClicPostion();

			UpdateNodeAndRect();
			rect.On();
		}
	}

	nodeHitbox.Update(Vec2::Zero);
	if (nodeHitbox.GetMoveIn())
		node.On();
}

void OneNodeObject::UpdateNodeAndRect()
{
	//std::cout << "cc" << std::endl;
	Loop::MettreAJour = true;
	actu = true;
	
	Vec2 d = position->direction;
	float len = float(d.Abs());
	d.Normalize();

	Vec2 o = position->position - distance *d;
	//std::cout << d<< std::endl;
	node.SetNode( node.GetNode().Abs() *d );

	rect.UpdateRect(position->position, position->direction, distance, distance);
	nodeHitbox.UpdateRect(o, (len + 4 * distance)*d, distance, distance);
}


void TwoNodeObject::Affichage()
{
	Vec2 o = position->position + 0.5*position->direction;
	Vec2 a = o + node.GetNode();
	Vec2 e = o - node.GetNode();

	Uint8 r(0), g(0), b(0);
	if (Loop::couleurFond)
	{
		r = white;
		g = white;
		b = white;
	}
	else
	{
		r = black;
		g = black;
		b = black;
	}

	LineDotted(a, o, r, g, b);

	a = Vec2::Diag * 5;
	Line(e - a, e + a, r, g, b);
	a = Vec2::AntiDiag * 5;
	Line(e - a, e + a, r, g, b);
	
	node.Affichage(o);
}

void TwoNodeObject::Init()
{
	if (!entity->HasComponent<Position>())
		entity->AddComponent<Position>();

	position = &entity->GetComponent<Position>();

	if (!entity->HasComponent<OneNodeObject>())
		entity->AddComponent<OneNodeObject>();

	ono = &entity->GetComponent<OneNodeObject>();

	Vec2 d = position->direction.Ortho();
	node.SetNode(d);

	UpdateNodeAndRect();
}

void TwoNodeObject::Update()
{
	if (ono->GetActu())
		UpdateNodeAndRect();
	
	node.Update(position->position + 0.5*position->direction);

	node.Off();
	if (node.GetMoveIn())
		node.On();
	if (node.GetClic())
	{
		Vec2 d = position->direction;
		Vec2 y = -1* node.GetNode();

		d.Normalize();

		node.SetNode(abs(Det(d, y)) * d.Ortho() );

		UpdateNodeAndRect();
		node.On();
	}

	nodeHitbox.Update(Vec2::Zero);
	if (nodeHitbox.GetMoveIn())
		node.On();
}

void TwoNodeObject::UpdateNodeAndRect()
{
	Loop::MettreAJour = true;

	Vec2 d = position->direction;
	Vec2 o = position->position + 0.5*d;
	Vec2 f = node.GetNode();
	double focale = f.Abs();
	
	double len = d.Abs();
	d.Normalize();

	node.SetNode( focale*d.Ortho());

	o = o - (focale +  distance )*d.Ortho();

	d = 2 *  (focale +  distance)*d.Ortho();

	nodeHitbox.UpdateRect(o, d, distance, distance);
}

void TwoNodeObject::SetNode(double f)
{
	Vec2 d = position->direction;
	d.Normalize();

	node.SetNode(f*d.Ortho());
}