#ifndef IUNIT_H
#define IUNIT_H

#include <iostream>
#include <string>

class IUnit
{
public:
	int health = 0;
	std::string name = "";
	float speed = 0.0f;
	int cost = 0.0f;
	int team;
	int type;

	float buildTime = 0.0f;


	enum class unitHealthState
	{
		alive,
		dead
	};
	unitHealthState hpState;

	IUnit(std::string _name, int _team, int _type);
	void takeDamage(int _amount);

	virtual void onInit() = 0;					//called when initiating the Unit
	virtual void onHit() = 0;					//hitting the Unit
	virtual void onDeath() = 0;					//Unit died

protected:

	virtual ~IUnit();

private:

};

#endif // !UNIT_H

