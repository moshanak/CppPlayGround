/*

テンプレートに関する変更

・概要
　　関数テンプレートをより簡単に書けるようになる。
  　テンプレートに関係する一部の制約が緩和される。
  　

・詳細（★は下部にサンプルコードあり）
	★4.1 autoパラメータによる関数テンプレートの簡易定義
	https://cpprefjp.github.io/lang/cpp20/function_templates_with_auto_parameters.html

	4.2 型しか現れない文脈でtypenameの省略を許可。たとえば、エイリアス宣言、static_cast等のキャスト時の型名など。
	https://cpprefjp.github.io/lang/cpp20/down_with_typename.html

	4.3 クラス型の非型テンプレート引数を許可。float 等の浮動小数も指定できる。
	https://cpprefjp.github.io/lang/cpp20/class_types_in_non-type_template_parameters.html

	4.4 集成体テンプレートの実引数からのテンプレート引数推論
	https://cpprefjp.github.io/lang/cpp20/class_template_argument_deduction_for_aggregates.html

	4.5 エイリアステンプレート経由でのクラステンプレートのテンプレート引数推論
	https://cpprefjp.github.io/lang/cpp20/class_template_argument_deduction_for_alias_templates.html

	4.6 explicit(bool)　explicit をつけるかどうか (bool) 内の評価式で切り替えることができるようになった
	https://cpprefjp.github.io/lang/cpp20/explicit_bool.html
	参考：https://komorinfo.com/blog/cpp-explicit-specifier/

・所感
　　4.1 の簡易定義は、コンセプトと組み合わせると便利なので、おすすめ。
　　4.2 以降は少しマニアックな内容なので、知っておく程度で良いと思う。
*/


#include <concepts>
#include <iostream>
#include <iomanip>
#include <limits>
#include <cassert>
#include <array>
#include <vector>
#include <type_traits>

#if 1	// 簡易定義なし
template<class T>
class Point {
public:
	Point(T a, T b, T c)
		:x(a), y(b), z(c)
	{}

	template<class I>
	const T& operator[](const I& i) const {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			assert(false);
			return x;
		}
	}

	Point<T> operator-(const Point<T>& p) const {
		return Point<T>(this->x - p.x, this->y - p.y, this->z - p.z);
	}

	T x, y, z;
};

class Math {
public:
	template<class T>
	static auto getLength(const T& p) {
		return std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
	}
};

template<class T>
class Line {
public:
	template<class P1, class P2>
	Line(const P1& p1, const P2& p2)
		:p1_(p1[0], p1[1], p1[2]),
		p2_(p2[0], p2[1], p2[2])
	{}

	auto getLength() {
		return Math::getLength(p1_ - p2_);
	}

private:
	Point<T> p1_;
	Point<T> p2_;
};
#elif 0	// 簡易定義あり
template<class T>
class Point {
public:
	Point(T a, T b, T c)
		:x(a), y(b), z(c)
	{}

	// template<class I> を書かず、auto 引数でOK
	const T& operator[](const auto& i) const {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			assert(false);
			return x;
		}
	}

	Point<T> operator-(const Point<T>& p) const {
		return Point<T>(this->x - p.x, this->y - p.y, this->z - p.z);
	}

	T x, y, z;
	//auto x, y, z;	//メンバ変数でautoパラメータによる簡易表記はできない
};

class Math {
public:
	// template<class T> を書かず、auto 引数でOK
	static auto getLength(const auto& p) {
		return std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
	}
};

template<class T>
class Line {
public:
	//template<class P1, class P2>を書かず、auto 引数でOK
	Line(const auto& p1, const auto& p2)
		:p1_(p1[0], p1[1], p1[2]),
		p2_(p2[0], p2[1], p2[2])
	{}

	auto getLength() {
		return Math::getLength(p1_ - p2_);
	}

private:
	Point<T> p1_;
	Point<T> p2_;
};
#else	//簡易定義あり。かつ、コンセプトも使う。
template<std::floating_point T>
class Point {
public:
	Point(T a, T b, T c)
		:x(a), y(b), z(c)
	{}

	// auto 引数の簡易表記でも、コンセプトを指定できる
	const T& operator[](const std::integral auto& i) const {
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			assert(false);
			return x;
		}
	}

	Point<T> operator-(const Point<T>& p) const {
		return Point<T>(this->x - p.x, this->y - p.y, this->z - p.z);
	}

	T x, y, z;
	//std::floating_point auto x, y, z;		//メンバ変数では、この書き方はできない
};

//p[0],p[1],p[2]を呼び出し可能で、その戻り値が浮動小数のみに限定するコンセプト
template <class T>
concept PointConcept = requires (T p) {
	requires std::is_floating_point_v<decltype(+p[0])>;
	requires std::is_floating_point_v<decltype(+p[1])>;
	requires std::is_floating_point_v<decltype(+p[2])>;
};

class Math {
public:
	// auto 引数の簡易表記でも、コンセプトを指定できる
	static std::floating_point auto getLength(const PointConcept auto& p) {
		return std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
	}
};

template<std::floating_point T>
class Line {
public:
	// auto 引数の簡易表記でも、コンセプトを指定できる
	Line(const PointConcept auto& p1, const PointConcept auto& p2)
		:p1_(p1[0], p1[1], p1[2]),
		p2_(p2[0], p2[1], p2[2])
	{}

	std::floating_point auto getLength() {
		return Math::getLength(p1_ - p2_);
	}

private:
	Point<T> p1_;
	Point<T> p2_;
};
#endif

int main()
{
	Point<float> pf1(1.0f, 1.0f, 1.0f);
	Point<float> pf2(1.0f, 2.0f, 1.0f);
	std::array<float, 3> af2{ 1.0f, 2.0f, 1.0f };
	std::vector<float> vf2{ 1.0f, 2.0f, 1.0f };

	Line<float> linef1(pf1, pf2);
	Line<float> linef2(pf1, af2);
	Line<float> linef3(pf1, vf2);

	std::cout << std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10);
	std::cout << pf1.x << ", " << pf1.y << ", " << pf1.z << std::endl;
	std::cout << pf2.x << ", " << pf2.y << ", " << pf2.z << std::endl;
	std::cout << linef1.getLength() << std::endl;
	std::cout << linef2.getLength() << std::endl;
	std::cout << linef3.getLength() << std::endl;

	Point<double> pd1(1.0, 1.0, 1.0);
	Point<double> pd2(1.0, 2.0, 1.0);
	std::array<double, 3> ad2{ 1.0, 2.0, 1.0 };
	std::vector<double> vd2{ 1.0, 2.0, 1.0 };

	Line<double> lined1(pd1, pd2);
	Line<double> lined2(pd1, ad2);
	Line<double> lined3(pd1, vd2);

	std::cout << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10);
	std::cout << pd1.x << ", " << pd1.y << ", " << pd1.z << std::endl;
	std::cout << pd2.x << ", " << pd2.y << ", " << pd2.z << std::endl;
	std::cout << lined1.getLength() << std::endl;
	std::cout << lined2.getLength() << std::endl;
	std::cout << lined3.getLength() << std::endl;
}