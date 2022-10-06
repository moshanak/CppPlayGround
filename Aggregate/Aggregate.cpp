/*

集成体に関する変更

集成体とは
https://onihusube.hatenablog.com/entry/2019/02/22/201044

・概要
	* 集成体の初期化方法の変更

・詳細（★は下部にサンプルコードあり）
	★9.1 指示付き初期化
	https://cpprefjp.github.io/lang/cpp20/designated_initialization.html
	★9.2 () による集成体初期化
	https://cpprefjp.github.io/lang/cpp20/allow_initializing_aggregates_from_a_parenthesized_list_of_values.html
	★9.3 ユーザー宣言したコンストラクタを持つクラスの集成体初期化を禁止
	https://cpprefjp.github.io/lang/cpp20/prohibit_aggregates_with_user-declared_constructors.html

・所感
	現状の開発では、集成体を使うケースはあまり無いため、利用場面は少ない印象。
	下の例の Point の場合、個人的には xyz は想像しやすく、指示付き初期化はむしろ冗長に感じるため、指示付き初期化を積極的に使わなくても良いと思う。
*/

#include <iostream>

struct Point
{
	float x, y, z;
};

struct PointNotAggregate
{
	PointNotAggregate() = default;
	float x, y, z;
};


int main()
{
	Point p1{ 0.0f, 1.0f, 2.0f };	// C++17 OK
	Point p2{ .x = 0.0f, .y = 1.0f, .z = 2.0f };	// C++17 NG,  C++20 OK

	//Point p{ .y = 0.0f, .x = 1.0f, .z = 2.0f };	// 宣言順でなければ NG
	//Point p{ .x = 0.0f, .y = 1.0f, 2.0f };	// .z = だけ書かないとかはできない


	Point p3(0.0f, 1.0f, 2.0f);	// C++17 NG,  C++20 OK

	//PointNotAggregate p{ 0.0f, 1.0f, 2.0f };	// C++17 OK,  C++20 NG コンストラクタが宣言されていれば集成体ではない

	return 0;
}