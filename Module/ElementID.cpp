#include "ElementID.h"

ElementID::ElementID(int id)
{
	id_ = id;
}

int ElementID::getID() const
{
	return id_;
}