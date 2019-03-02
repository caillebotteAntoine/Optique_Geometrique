#pragma once
#include "Component.h"
#include "Node.h"


class RectWithTwoNode : public Component
{
public:
	RectWithTwoNode() : tag(Loop::null) {}
	RectWithTwoNode( size_t t) : tag(t) {};

	void Affichage() override;
	void Init() override;
	void Update() override;

	bool GetActu() const { return actu; }
	
private : 
	void UpdateNodeAndRect();
	Position *position;

	size_t tag;

	Node nodeUp;
	Node nodeLeft;

	Rect nodeHitbox;
	Rect rect;

	bool actu;
};

