#ifndef ELEMENT_H
#define ELEMENT_H
#include "ElementID.h"

class Element
{
public:
	Element() = delete;
	Element(ElementID id);
	void outputLog();
private:
	ElementID elementID_;
};

#endif // ELEMENT_H
