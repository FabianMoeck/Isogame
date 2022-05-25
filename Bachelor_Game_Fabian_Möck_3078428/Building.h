#ifndef BUILDING_H
#define BUILDING_H

#include "IUnit.h"

class Building : public IUnit
{
public:
	Building(std::string _name, int _team);

protected:
	virtual ~Building();

};

#endif // !BUILDING_H

