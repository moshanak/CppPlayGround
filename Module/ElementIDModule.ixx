export module elementIDLib;

export class ElementID
{
public:
	ElementID() = delete;
	ElementID(int id);
	int getID() const;
private:
	int id_;
};

