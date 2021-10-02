#pragma once
#include <source_location>

//ソースコード上の位置を表すクラス
class Try_source_location
{
public:

	//この関数の呼び出し元のソースコード上の位置を返す
	void current();

	//呼び出し元のソースコードの位置を取得する方法
	//デフォルト引数にして、呼び出し元では引数なしで呼んでも、呼び出し元の情報を表示できる
	void currentCaller(std::source_location location = std::source_location::current());
};