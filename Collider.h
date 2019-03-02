#pragma once

#include "Component.h"
#include "Node.h"

#define defaultfocale 30
#define decalageSource 20
#define tailleSource 8

class SegmentCollider : public Component
{
public:
	SegmentCollider(size_t t): tag(t) {}

	Vec2 GetOrigine() const { return position->position; }
	Vec2 GetDirection() const { return position->direction; }
	size_t GetTag() const { return tag; }

	void Affichage() override;
	void Init() override;

private:
	const size_t tag;
	Position *position;

};

class OneNodeObject : public Component
{
public:
	OneNodeObject() : tag(Loop::null) {}
	OneNodeObject(size_t t) : tag(t) {}

	void Affichage() override;
	void Init() override;
	void Update() override;


	bool GetActu() const { return actu; }
private:
	void UpdateNodeAndRect();
	Position *position;

	size_t tag;

	Node node;
	Rect nodeHitbox;
	Rect rect;

	bool actu;
};

class TwoNodeObject : public Component
{
public:
	TwoNodeObject() {}

	void Affichage() override;
	void Init() override;
	void Update() override;

	Vec2 GetNode() { return node.GetNode(); }
	void SetNode(double f);

private:
	void UpdateNodeAndRect();
	Position *position;

	OneNodeObject *ono;

	Node node;
	Rect nodeHitbox;
};
