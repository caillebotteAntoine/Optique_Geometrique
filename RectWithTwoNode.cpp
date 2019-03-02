#include "RectWithTwoNode.h"

#define distance 5

void RectWithTwoNode::Affichage()
{
	nodeUp.Affichage(position->position);
	nodeLeft.Affichage(position->position);
	rect.Affichage();
}

void RectWithTwoNode::Init()
{
	if (!entity->HasComponent<Position>())
		entity->AddComponent<Position>();

	position = &entity->GetComponent<Position>();

	Vec2 d = position->direction.Normalize();
	position->direction = 6 * defaultfocale*d.Normalize();

	if (tag == Loop::null)
	{
		if (entity->HasComponent<SegmentCollider>())
			tag = entity->GetComponent<SegmentCollider>().GetTag();

		if (entity->HasComponent<Source>())
			tag = Loop::ObjetSourceLumineuse;
	}
	
	UpdateNodeAndRect();
}

void RectWithTwoNode::Update()
{
	actu = false;
	nodeUp.Update(position->position);
	nodeUp.Off();

	if (nodeUp.GetClic())
	{
		Vec2 d = nodeUp.GetNode();

		float len = float(d.Abs());
		d.Normalize();

		Vec2 m = position->position + 0.5 * position->direction;

		d = Vec2::Smouth(d, 24, 0.04);
		position->direction = (len - 3 * distance)* d;

		position->position = m - 0.5* position->direction;

		UpdateNodeAndRect();
		//node.On();
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
	
	if (nodeHitbox.GetMoveIn() || nodeUp.GetMoveIn())
		nodeUp.On();
}

void RectWithTwoNode::UpdateNodeAndRect()
{
	Loop::MettreAJour = true;
	actu = true;

	Vec2 d = position->direction;
	float len = float(d.Abs());
	d.Normalize();

	Vec2 o = position->position - distance *d;

	nodeUp.SetNode(nodeUp.GetNode().Abs() *d);

	rect.UpdateRect(position->position, position->direction, distance, distance);
	nodeHitbox.UpdateRect(o, (len + 4 * distance)*d, distance, distance);
}
