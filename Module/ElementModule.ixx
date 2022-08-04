export module elementLib;
import elementIDLib;

export class Element
{
public:
	Element() = delete;
	Element(ElementID id);
	void outputLog();
private:
	ElementID elementID_;
};
