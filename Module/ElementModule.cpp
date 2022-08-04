#include <iostream>
import elementLib;

Element::Element(ElementID id)
	:elementID_(id)
{
}

void Element::outputLog()
{
	std::cout << "ElementID: " << elementID_.getID() << std::endl;
}
