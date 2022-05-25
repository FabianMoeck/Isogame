#ifndef FACTORY_H
#define FACTORY_H

#include "Building.h"

class Factory : public Building
{
public:
	Factory(std::string _name, int _team);
	~Factory();

private:
	int hp = 150;
	float bT = 7.0f;

	void onInit();
	void onHit();
	void onDeath();

};

#endif // !FACTORY_H

