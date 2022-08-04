//#ifndef ELEMENT_ID_H
//#define ELEMENT_ID_H

class ElementID
{
public:
	ElementID() = delete;
	ElementID(int id);
	int getID() const;
private:
	int id_;
};

//#endif // ELEMENT_ID_H
