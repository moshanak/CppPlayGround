#include <concepts>
#include <iostream>

#if 1
// 数値型(整数or浮動小数点数)を判定するコンセプト
template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

// #1 プライマリ関数テンプレート(簡略構文)
void twice(auto) {
	std::cout << "*" << std::endl;
}

// #2 数値型に対して優先される関数テンプレート(簡略構文)
void twice(Number auto x) {
	std::cout << (x * 2) << std::endl;
}

// #3 符号なし整数型に対して最優先される関数テンプレート(簡略構文)
void twice(std::unsigned_integral auto x) {
	std::cout << (x * 2) << "u" << std::endl;
}
#elif 0
//C++17以前の書き方1

// 関数オーバーロード
void twice(...) {
	std::cout << "*" << std::endl;
}

void twice(int x) {
	std::cout << (x * 2) << std::endl;
}

void twice(long long x) {
	std::cout << (x * 2) << std::endl;
}

void twice(float x) {
	std::cout << (x * 2) << std::endl;
}

void twice(double x) {
	std::cout << (x * 2) << std::endl;
}

void twice(unsigned char x) {
	std::cout << (x * 2) << "u" << std::endl;
}

void twice(unsigned int x) {
	std::cout << (x * 2) << "u" << std::endl;
}

void twice(unsigned long long x) {
	std::cout << (x * 2) << "u" << std::endl;
}

#else
//C++17以前の書き方2
//関数テンプレート

// 数値型(整数or浮動小数点数)を判定するメタ関数
template <typename T>
struct is_number : std::integral_constant<bool, std::is_integral<T>::value || std::is_floating_point<T>::value>
{};

// #1 数値型以外に対して有効となる関数テンプレート
template <typename T>
typename std::enable_if<!is_number<T>::value>::type
twice(T) {
	std::cout << "*" << std::endl;
}

// #2 数値型かつ符号なし整数型以外に対して有効となる関数テンプレート
template <typename T>
typename std::enable_if<is_number<T>::value && !std::is_unsigned<T>::value>::type
twice(T x) {
	std::cout << (x * 2) << std::endl;
}

// #3 数値型かつ符号なし整数型に対して有効となる関数テンプレート
template <typename T>
typename std::enable_if<is_number<T>::value&& std::is_unsigned<T>::value>::type
twice(T x) {
	std::cout << (x * 2) << "u" << std::endl;
}
#endif

int main()
{
	//数値は2倍
	twice(1);
	twice(1.0);
	//符号なし整数は末尾にuを付ける
	twice(1u);
	//それ以外は * を出力
	twice("aaa");

	return 0;
}
