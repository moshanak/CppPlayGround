/*

マイナーな変更と Defect Report

・概要
	* どれも機能がバラバラなので一言でまとめにくい。概要は省略。

・詳細（★は下部にサンプルコードあり）
	12.1 関数テンプレートに明示的に型指定した場合に ADL で見つからない問題を修正
	https://cpprefjp.github.io/lang/cpp20/adl_and_function_templates_that_are_not_visible.html
	12.2 要素数不明の配列型への変換
	https://cpprefjp.github.io/lang/cpp20/permit_conversions_to_arrays_of_unknown_bound.html
	12.3 特殊化のアクセスチェック

	12.4 default コピーコンストラクタの const ミスマッチを delete するようにする

	12.5 const 修飾されたメンバポインタの制限を修正
	https://cpprefjp.github.io/lang/cpp20/fixing_const_qualified_pointers_to_members.html
	12.6 destroying operator delete

	13.1 new 式における配列要素数の推論
	https://cpprefjp.github.io/lang/cpp20/array_size_deduction_in_new-expressions.html
	★13.2 ポインタ型から bool への変換を縮小変換とする
	https://cpprefjp.github.io/lang/cpp20/converting_from_pointer_to_bool_should_be_considered_narrowing.html
	13.3 暗黙のムーブ対象の拡大

	13.4 異なる例外指定がなされた default メンバ関数の許可

	13.5 不完全型を用いた宣言の抽象クラスのチェックを遅延する

・所感
	業務で使うケースはほぼないだろう。
	13.2は遭遇する可能性がありそうなので、サンプルコードで説明を補足している。
*/

#include <variant>
#include <string>
#include <iostream>

int main()
{
	//const char* を variant 型に入れて、string 型で保持するつもりだったが、下記の挙動になってしまう
	std::variant<std::string, bool> x = "abc";

	if (std::holds_alternative<std::string>(x)) {
		std::cout << std::get<std::string>(x) << std::endl;		//C++20ではこちらが呼ばれる
	}
	else if (std::holds_alternative<bool>(x)) {
		std::cout << std::get<bool>(x) << std::endl;			//C++17ではこちらが呼ばれ、これはバグだった
	}

	return 0;
}