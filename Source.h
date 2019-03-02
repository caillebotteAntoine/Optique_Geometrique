#pragma once

#include "Component.h"
#include "RayLight.h"
#include <vector>
#include <list>

class point;

class Source : public Component
{
public:
	Source(int r, int g, int b);

	void Affichage() override;
	void Init() override;
	void Update() override;

	int GetNextSegments(int id, unsigned int min);
	RayLight GetSegment(int id) { return segments[id]; }
	unsigned int GetNbrSource() { return nbrSource; }


	void ResetAffichage()
	{
		for (unsigned int i(0); i < affichage.size(); i++)
			affichage[i] = true;
	}
	void UpdateTexture()
	{
		for (unsigned int i(0); i < segments.size(); i++)
			segments[i].UpdateTexture();
	}
		
private:
	int GestionCollision(unsigned int id, std::vector<Entity*> colliders, unsigned int step, unsigned int* lettre);

	Position* position;
	std::vector<RayLight> segments;
	std::vector<bool> affichage;

	unsigned int nbrSource;

	int red;
	int green;
	int blue;

	int keyPress;
	bool keyUp;

	std::list<point>points;

	void UpdateTrainOnde();
	void InitTrainOnde();	
};
