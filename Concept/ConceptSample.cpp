/*

コンセプト

・概要：
　テンプレートで指定する型に制約を付ける機能

・メリット：
　テンプレートの制約の実装がしやすくなる。C++20より前でも制約を付ける方法はあったが、実装が複雑で可読性が悪かった。

・所感
 テンプレートを実装するときは、常にコンセプトで制約を設けると、読み手は理解しやすいコードになると思う。

・cpprefjp
　https://cpprefjp.github.io/lang/cpp20/concepts.html

*/

#include <concepts>
#include <iostream>
#include <iomanip>
#include <limits>
#include <typeinfo>


//------------------------------------------------------------------------------
// 例１　標準機能のコンセプトを使う
//------------------------------------------------------------------------------

//単純なテンプレートを実装する場合、コンパイルエラーにならなければ T の型に何でも指定できる
//この例だと数値だけでなく、文字列なども指定できる
template<class T>
class PointOld {
public:
	PointOld(T a, T b, T c)
		:x(a), y(b), z(c)
	{}
	T x, y, z;
};

//std::floating_point は float, double などの浮動小数点数型のみ T の型にできるコンセプト
//C++の標準機能として提供されているコンセプトがある
template<std::floating_point T>
class Point {
public:
	Point(T a, T b, T c)
		:x(a), y(b), z(c)
	{}
	T x, y, z;
};

void example1() {
	PointOld pf(1.0f, 2.0f, 3.0f);
	std::cout << std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10) << pf.x << ", " << pf.y << ", " << pf.z << std::endl;
	PointOld pd(1.0, 2.0, 3.0);
	std::cout << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << pd.x << ", " << pd.y << ", " << pd.z << std::endl;
	PointOld pi(1, 2, 3);
	std::cout << pi.x << ", " << pi.y << ", " << pi.z << std::endl;
	PointOld ps("a", "b", "c");
	std::cout << ps.x << ", " << ps.y << ", " << ps.z << std::endl;

	Point pf2(1.0f, 2.0f, 3.0f);
	std::cout << std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10) << pf2.x << ", " << pf2.y << ", " << pf2.z << std::endl;
	Point pd2(1.0, 2.0, 3.0);
	std::cout << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << pd2.x << ", " << pd2.y << ", " << pd2.z << std::endl;
	//↓はコンパイルエラー
	//Point pi(1, 2, 3);
	//Point ps("a", "b", "c");
}

//------------------------------------------------------------------------------
// 例2　独自のコンセプトを定義する
//------------------------------------------------------------------------------

class Shell {
public:
	Shell(int i) :id(i) {}
	int getElementID() const { return id; }
	int id;
};

class Solid {
public:
	Solid(int i) :id(i) {}
	int getElementID() const { return id; }
	int id;
};

class Beam {
public:
	Beam(int i) :id(i) {}
	int getElementID() const { return id; }
	int id;
};

class Node {
public:
	Node(int i) :id(i) {}
	int getNodeID() const { return id; }
	int id;
};

/*
コンセプトの定義方法

template <テンプレートパラメータ宣言>
concept コンセプト名 = requires式;
*/
template <class T>
concept Element = requires (T e) {
	//式が有効な場合に T に指定できる
	//このコンセプトでは、getElementID 関数をもつクラスを T に指定できる
	e.getElementID();
};

//特定の型だけ T に指定することもできる
template <class T>
concept ElementShellSolid = std::same_as<T, Shell> || std::same_as<T, Solid>;

class Window {
public:
	template <Element T>
	void displayElementID(T e) {
		std::cout << typeid(T).name() << "\t" << e.getElementID() << std::endl;
	}

	template <ElementShellSolid T>
	void displayElementShellSolidID(T e) {
		std::cout << typeid(T).name() << "\t" << e.getElementID() << std::endl;
	}
};

void example2() {
	Window window;
	window.displayElementID(Shell(1));
	window.displayElementID(Solid(2));
	window.displayElementID(Beam(3));
	//window.displayElementID(Node(4));	//コンパイルエラー
	window.displayElementShellSolidID(Shell(1));
	window.displayElementShellSolidID(Solid(2));
	//window.displayElementShellSolidID(Beam(3));	//コンパイルエラー
	//window.displayElementShellSolidID(Node(4));	//コンパイルエラー
}

//------------------------------------------------------------------------------
// 例3　コンセプトとオーバーロード
//------------------------------------------------------------------------------

//コンセプトを指定したテンプレート関数をオーバーロードできる
//複数のオーバーロード関数で T の型が条件を満たす場合、基本的には制約が強い関数が優先されるが、優先順位の仕様は複雑。
//そのため、各オーバーロード関数の間で、制約が被らないようにすると可読性が良い。

//template <std::integral T>	//こちらでもコンパイルは通るが、unsigned_integral と制約が被るため、優先順位を考慮する必要がある
template <std::signed_integral T>
bool isEqual(T a, T b) {
	const bool ret = (a == b);
	std::cout << std::boolalpha << "a == b\t" << ret << std::endl;
	return ret;
}

template <std::floating_point T>
bool isEqual(T a, T b) {
	const bool ret = (std::abs(a - b) <= std::numeric_limits<T>::epsilon());
	std::cout << std::boolalpha << "abs(a-b) <= epsilon\t" << ret << std::endl;
	return ret;
}

template <std::unsigned_integral T>
bool isEqual(T a, T b) {
	const bool ret = (a == b);
	std::cout << std::boolalpha << "unsigned a == b\t" << ret << std::endl;
	return ret;
}

void example3()
{
	isEqual(1, 2);
	isEqual(1LL, 1LL);
	isEqual(1.0f, 2.0f);
	isEqual(1.0, 1.0000000000000001);
	isEqual(1U, 2U);
	isEqual(0, 0);
}

int main()
{
	std::cout << "----- example1 -----" << std::endl;
	example1();
	std::cout << std::endl;
	std::cout << "----- example2 -----" << std::endl;
	example2();
	std::cout << std::endl;
	std::cout << "----- example3 -----" << std::endl;
	example3();
	std::cout << std::endl;

	return 0;
}