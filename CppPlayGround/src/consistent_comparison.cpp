#include <compare>  //<=>���p�̏ꍇ�K�{
#include <iostream>
#include <string>
#include "consistent_comparison.h"

struct C {
	int x;
	int y;
	double v;
	char str[32];

	//<=>��public�Œ�`���Ă������ƂŁA���̑��̉��Z�q�����o�����
	auto operator<=>(const C&) const = default;
	//std::partial_ordering operator<=>(const C&) const = default;
};

struct oldC {
	int x;
	int y;
	double v;
	char str[32];

	bool operator<(const oldC& rhs) const {
		std::string s(str);
		std::string rs(rhs.str);
		return std::tie(x, y, v, s)
			< std::tie(rhs.x, rhs.y, rhs.v, rs);
	}

	bool operator==(const oldC& rhs) const {
		std::string s(str);
		std::string rs(rhs.str);
		return std::tie(x, y, v, s)
			== std::tie(rhs.x, rhs.y, rhs.v, rs);
	}

	bool operator!=(const oldC& rhs) const {
		return !(*this == rhs);
	}

	bool operator>(const oldC& rhs) const {
		return rhs < *this;
	}

	bool operator<=(const oldC& rhs) const {
		return !(*this > rhs);
	}

	bool operator>=(const oldC& rhs) const {
		return !(*this < rhs);
	}
};

struct D {
	std::string str;

	//auto operator<=>(const D&) const = default;

	// �Ǝ��� <=> �����������B
	// ��r�J�e�S���^�����ɂ��邩�́A�ǂ̂悤�Ȕ�r�����邩�ŁA�g���������K�v�ɂȂ�
	std::weak_ordering operator<=>(const D& that) const {
		//�啶���������𓯒l�Ƃ��Ĉ����Ĕ�r
		for (std::size_t i = 0; i < this->str.size(); ++i) {
			char l1 = std::tolower(this->str[i]);
			char l2 = std::tolower(that.str[i]);
			if (l1 != l2) {
				return l1 <=> l2;
			}
		}
		return std::weak_ordering::equivalent;
	}

	//std::strong_ordering operator<=>(const D& that) const {
	//	//�啶���������𓯒l�Ƃ��Ĉ����Ĕ�r
	//	for (std::size_t i = 0; i < this->str.size(); ++i) {
	//		char l1 = std::tolower(this->str[i]);
	//		char l2 = std::tolower(that.str[i]);
	//		if (l1 != l2) {
	//			return l1 <=> l2;
	//		}
	//	}
	//	return std::strong_ordering::equivalent;
	//}

	//std::partial_ordering operator<=>(const D& that) const {
	//	//�啶���������𓯒l�Ƃ��Ĉ����Ĕ�r
	//	for (std::size_t i = 0; i < this->str.size(); ++i) {
	//		char l1 = std::tolower(this->str[i]);
	//		char l2 = std::tolower(that.str[i]);
	//		if (l1 != l2) {
	//			return l1 <=> l2;
	//		}
	//	}
	//	return std::partial_ordering::equivalent;
	//}

	// �Ǝ��� <=> ����������ꍇ�́A== �� != �͎����ňÖٓI�ɂ͒�`����Ȃ��Ȃ�B
	// ���̂��߁A== ����������K�v������B
	// != �͕s�v�ŁA== ����������ΈÖٓI�ɒ�`�����B
	bool operator==(const D& that) const {
		//�啶���������𓯒l�Ƃ��Ĉ����Ĕ�r
		for (std::size_t i = 0; i < this->str.size(); ++i) {
			if (std::tolower(this->str[i]) != std::tolower(that.str[i])) return false;
		}
		return true;
	}

};

struct E
{
	int i;
};

struct G
{
	int i;
	auto operator<=>(const G&) const = default;
};

struct H
{
	int i;
	bool operator==(const H& that) const { return this->i == that.i; }
	bool operator< (const H& that) const { return this->i < that.i; }
};

//struct F
//{
//	F(int f) :j(f), e{ f } {}
//	int j;
//	E e;
//
//	auto operator<=>(const F&) const = default;
//};

struct F
{
	F(int f) :j(f), g{ f } {}
	int j;
	G g;

	auto operator<=>(const F&) const = default;
};

//struct F
//{
//	F(int f) :j(f), h{ f } {}
//	int j;
//	H h;
//
//	//auto operator<=>(const F&) const = default;
//	std::strong_ordering operator<=>(const F&) const = default;
//};

void consistent_comparison::demo()
{
	std::cout << std::boolalpha;

	oldC oldC1 = { 10, 20, 3.1415, "Three-way Comparison" };
	oldC oldC2 = { 10, 20, 3.1415, "Spaceship Operator" };

	//���܂ł̔�r
	std::cout << (oldC1 < oldC2) << "\t(oldC1  < oldC2)" << std::endl;
	std::cout << (oldC1 <= oldC2) << "\t(oldC1  <= oldC2)" << std::endl;
	std::cout << (oldC1 > oldC2) << "\t(oldC1  > oldC2)" << std::endl;
	std::cout << (oldC1 >= oldC2) << "\t(oldC1  >= oldC2)" << std::endl;
	std::cout << (oldC1 == oldC2) << "\t(oldC1  == oldC2)" << std::endl;
	std::cout << (oldC1 != oldC2) << "\t(oldC1  != oldC2)" << std::endl;
	std::cout << std::endl;

	C c1 = { 10, 20, 3.1415, "Three-way Comparison" };
	C c2 = { 10, 20, 3.1415, "Spaceship Operator" };
	//c2 = c1;

	//�O����r���Z�q���̂��̂ɂ���r
	std::cout << ((c1 <=> c2) == 0) << "\t((c1 <=> c2) == 0)" << std::endl;
	std::cout << ((c1 <=> c2) < 0) << "\t((c1 <=> c2) < 0)" << std::endl;
	std::cout << ((c1 <=> c2) > 0) << "\t((c1 <=> c2) > 0)" << std::endl;
	std::cout << std::endl;

	//�O����r���Z�q�̌��ʂ� bool �^�ł͂Ȃ��A��r�J�e�S���^�ɂȂ�
	//��r�J�e�S���^�́A��q�̂悤�� 0 �Ƃ�����r�\�Ȍ^
	std::cout << typeid(c1 <=> c2).name() << std::endl;
	std::cout << typeid(1 <=> 2).name() << std::endl;
	std::cout << typeid(std::string("aaa") <=> std::string("bbb")).name() << std::endl;
	std::cout << typeid(1.1 <=> 2.1).name() << std::endl;
	std::cout << typeid(c1 < c2).name() << std::endl;
	std::cout << typeid(c1 <= c2).name() << std::endl;
	std::cout << typeid(c1 > c2).name() << std::endl;
	std::cout << typeid(c1 >= c2).name() << std::endl;
	std::cout << typeid(c1 == c2).name() << std::endl;
	std::cout << typeid(c1 != c2).name() << std::endl;
	std::cout << std::endl;

	//�N���XC��6�̉��Z�q�ɂ���r���\
	//default�ł͐錾���ɔ�r����邽�߁Ax -> y -> v -> str[0]�E�E�Estr[31] �̏��ɔ�r�����
	std::cout << (c1 < c2) << "\t(c1 < c2)" << std::endl;
	std::cout << (c1 <= c2) << "\t(c1 <= c2)" << std::endl;
	std::cout << (c1 > c2) << "\t(c1 > c2)" << std::endl;
	std::cout << (c1 >= c2) << "\t(c1 >= c2)" << std::endl;
	std::cout << (c1 == c2) << "\t(c1 == c2)" << std::endl;
	std::cout << (c1 != c2) << "\t(c1 != c2)" << std::endl;
	std::cout << std::endl;

	//�Ǝ��� <=> ���`������
	D d1{ "test" };
	D d2{ "Test" };
	std::cout << (d1 < d2) << "\t(d1 < d2)" << std::endl;
	std::cout << (d1 <= d2) << "\t(d1 <= d2)" << std::endl;
	std::cout << (d1 > d2) << "\t(d1 > d2)" << std::endl;
	std::cout << (d1 >= d2) << "\t(d1 >= d2)" << std::endl;
	std::cout << (d1 == d2) << "\t(d1 == d2)" << std::endl;
	std::cout << (d1 != d2) << "\t(d1 != d2)" << std::endl;
	std::cout << std::endl;

	//<=> �� default ��`���Ă��A������ delete �����P�[�X������
	F f1{ 1 };
	F f2{ 2 };
	std::cout << (f1 < f2) << "\t(f1 < f2)" << std::endl;
	std::cout << (f1 <= f2) << "\t(f1 <= f2)" << std::endl;
	std::cout << (f1 > f2) << "\t(f1 > f2)" << std::endl;
	std::cout << (f1 >= f2) << "\t(f1 >= f2)" << std::endl;
	std::cout << (f1 == f2) << "\t(f1 == f2)" << std::endl;
	std::cout << (f1 != f2) << "\t(f1 != f2)" << std::endl;
	std::cout << std::endl;

	//[����]
	// �O����r���Z�q���̂��̂ɂ���r�́A�����b�g�������Ȃ��B
	// ��r�J�e�S���^�̎g�������͖ʓ|�Ɋ�����B
	// �N���X��default�łU�̉��Z�q���`����ꍇ�͕֗��B
	// �Ǝ��̒�`���������ꍇ�́A�킴�킴�O����r���Z�q���g��Ȃ��Ă��ǂ��Ǝv���B�i��r�J�e�S���^���g�����Ȃ��K�v������B�j
}
