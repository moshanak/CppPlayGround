#include <compare>  //<=>利用の場合必須
#include <iostream>
#include <string>

struct C {
	int x;
	int y;
	double v;
	char str[32];

	//<=>をpublicで定義しておくことで、その他の演算子が導出される
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

	// 独自に <=> を実装する例。
	// 比較カテゴリ型を何にするかは、どのような比較をするかで、使い分けが必要になる
	std::weak_ordering operator<=>(const D& that) const {
		//大文字小文字を同値として扱って比較
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
	//	//大文字小文字を同値として扱って比較
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
	//	//大文字小文字を同値として扱って比較
	//	for (std::size_t i = 0; i < this->str.size(); ++i) {
	//		char l1 = std::tolower(this->str[i]);
	//		char l2 = std::tolower(that.str[i]);
	//		if (l1 != l2) {
	//			return l1 <=> l2;
	//		}
	//	}
	//	return std::partial_ordering::equivalent;
	//}

	// 独自に <=> を実装する場合は、== と != は自動で暗黙的には定義されなくなる。
	// そのため、== も実装する必要がある。
	// != は不要で、== を実装すれば暗黙的に定義される。
	bool operator==(const D& that) const {
		//大文字小文字を同値として扱って比較
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

int main()
{
	std::cout << std::boolalpha;

	oldC oldC1 = { 10, 20, 3.1415, "Three-way Comparison" };
	oldC oldC2 = { 10, 20, 3.1415, "Spaceship Operator" };

	//今までの比較
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

	//三方比較演算子そのものによる比較
	std::cout << ((c1 <=> c2) == 0) << "\t((c1 <=> c2) == 0)" << std::endl;
	std::cout << ((c1 <=> c2) < 0) << "\t((c1 <=> c2) < 0)" << std::endl;
	std::cout << ((c1 <=> c2) > 0) << "\t((c1 <=> c2) > 0)" << std::endl;
	std::cout << std::endl;

	//三方比較演算子の結果は bool 型ではなく、比較カテゴリ型になる
	//比較カテゴリ型は、上述のように 0 とだけ比較可能な型
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

	//クラスCは6つの演算子による比較が可能
	//defaultでは宣言順に比較されるため、x -> y -> v -> str[0]・・・str[31] の順に比較される
	std::cout << (c1 < c2) << "\t(c1 < c2)" << std::endl;
	std::cout << (c1 <= c2) << "\t(c1 <= c2)" << std::endl;
	std::cout << (c1 > c2) << "\t(c1 > c2)" << std::endl;
	std::cout << (c1 >= c2) << "\t(c1 >= c2)" << std::endl;
	std::cout << (c1 == c2) << "\t(c1 == c2)" << std::endl;
	std::cout << (c1 != c2) << "\t(c1 != c2)" << std::endl;
	std::cout << std::endl;

	//独自に <=> を定義した例
	D d1{ "test" };
	D d2{ "Test" };
	std::cout << (d1 < d2) << "\t(d1 < d2)" << std::endl;
	std::cout << (d1 <= d2) << "\t(d1 <= d2)" << std::endl;
	std::cout << (d1 > d2) << "\t(d1 > d2)" << std::endl;
	std::cout << (d1 >= d2) << "\t(d1 >= d2)" << std::endl;
	std::cout << (d1 == d2) << "\t(d1 == d2)" << std::endl;
	std::cout << (d1 != d2) << "\t(d1 != d2)" << std::endl;
	std::cout << std::endl;

	//<=> を default 定義しても、自動で delete されるケースもある
	F f1{ 1 };
	F f2{ 2 };
	std::cout << (f1 < f2) << "\t(f1 < f2)" << std::endl;
	std::cout << (f1 <= f2) << "\t(f1 <= f2)" << std::endl;
	std::cout << (f1 > f2) << "\t(f1 > f2)" << std::endl;
	std::cout << (f1 >= f2) << "\t(f1 >= f2)" << std::endl;
	std::cout << (f1 == f2) << "\t(f1 == f2)" << std::endl;
	std::cout << (f1 != f2) << "\t(f1 != f2)" << std::endl;
	std::cout << std::endl;

	//[所感]
	// 三方比較演算子そのものによる比較は、メリットを感じない。
	// 比較カテゴリ型の使い分けは面倒に感じる。
	// クラスにdefaultで６つの演算子を定義する場合は便利。
	// 独自の定義をしたい場合は、わざわざ三方比較演算子を使わなくても良いと思う。（比較カテゴリ型を使いこなす必要がある。）

	return 0;
}
