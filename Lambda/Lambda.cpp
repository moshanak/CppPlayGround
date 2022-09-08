/*

ラムダ式に関する変更

・概要
	* ラムダ式でテンプレート構文を用いた書き方ができるようになる。
	* C++20から非推奨のラムダ式の書き方がある。
	* ラムダ式の細かな制約の緩和。

・詳細（★は下部にサンプルコードあり）
	5.1 ジェネリックラムダのテンプレート構文
	https://cpprefjp.github.io/lang/cpp20/familiar_template_syntax_for_generic_lambdas.html
	★5.2 [=, this] と this の暗黙キャプチャの非推奨
	https://cpprefjp.github.io/lang/cpp20/allow_lambda_capture_equal_this.html
	https://cpprefjp.github.io/lang/cpp20/deprecate_implicit_capture_of_this_via_defcopy.html
	5.3 ステートレスラムダのクロージャ型のデフォルトコンストラクタと代入演算子の定義。
		例えば、set の比較クラスや、unique_ptr のカスタムデリータにラムダ式を指定するときに、記述しやすくなる。
	https://cpprefjp.github.io/lang/cpp20/default_constructible_and_assignable_stateless_lambdas.html
	5.4 評価（実行）されない文脈でのラムダ式の記述を許可。例えば、コンセプトの中でラムダ式を記述できるようになる。
	https://cpprefjp.github.io/lang/cpp20/wording_for_lambdas_in_unevaluated_contexts.html
	5.5 初期化キャプチャにおけるパック展開。可変長テンプレートのパラメータをラムダ式内で扱いやすくなる。
	https://cpprefjp.github.io/lang/cpp20/allow_pack_expansion_in_lambda_init_capture.html
	5.6 構造化束縛した変数のキャプチャを許可
	https://cpprefjp.github.io/lang/cpp20/reference_capture_of_structured_bindings.html

・所感
	5.2 の非推奨の書き方は、既存のコードでもありそうなので、注意した方が良い。
	5.1 ラムダ式でのテンプレート構文は新しい書き方として覚えておくと良い。ただし、使用頻度はあまり無いだろう。
	それ以外は、マニアックな印象。
*/

#include <iostream>
#include <vector>

class LambdaDiff {
public:
	int lambdaEqual1() {
		int value = 1;

		// C++20からは非推奨
		// 暗黙的に this ポインタをコピーキャプチャ
		return [=] {
			member_ = 100;	//C++20から非推奨になる理由。 = によるコピーキャプチャだが、暗黙的に this ポインタがコピーキャプチャされることで、member_ の値が変わる。
			return value + member_;
		}();
	}

	int lambdaEqual2() {
		int value = 1;

		// C++20 からは lambdaEqual1 の代わりに推奨される書き方
		// C++17 ではコンパイルエラー
		return [=, this] {
			member_ = 100;	//明示的に this ポインタをコピーキャプチャすることが分かるため、lambdaEqual1 のような罠はない。
			return value + member_;
		}();
	}

	int lambdaEqual3() {
		int value = 1;

		// C++20でも、ラムダ式内で this を使わないならば、OK
		return [=] {
			return value;
		}();
	}

	// ----------------
	// 以降は、C++20 の話ではなく余談。ラムダ式のキャプチャの違いを説明するサンプルコード。 
	// ----------------

	int lambdaEqual4() {
		int value = 1;

		// lambdaEqual1 や lambdaEqual2 と異なり、this ポインタではなく、*this をコピーキャプチャする
		return[=, *this]{
			//member_ = 100;	//コンパイルエラー。コピーキャプチャした変数は mutable がないと変更不可。
			return value + member_;
		}();
	}

	int lambdaEqual5() {
		int value = 1;

		// mutable を付けると、コピーキャプチャした変数も変更可能になる。
		return[=, *this]() mutable {
			value = 10;
			member_ = 100;
			return value + member_;
		}();
	}

	int lambdaRef1() {
		int value = 1;

		// 参照キャプチャ。暗黙的に this ポインタをコピーキャプチャ。
		return [&] {
			value = 10;
			member_ = 100;
			return value + member_;
		}();
	}

	int lambdaRef2() {
		int value = 1;

		// 参照キャプチャ。明示的に this ポインタをコピーキャプチャ。lambdaRef1 と同じ。
		return [&, this] {
			value = 10;
			member_ = 100;
			return value + member_;
		}();
	}

	int lambdaRef3() {
		int value = 1;

		// 参照キャプチャ。this ポインタではなく、*this をコピーキャプチャする。
		return[&, *this]{
			value = 10;
		//member_ = 100;	//コンパイルエラー。コピーキャプチャした変数は mutable がないと変更不可。
		return value + member_;
		}();
	}

	void member() {
		std::cout << "member_: " << member_ << std::endl;
	}

private:
	int member_ = 0;
};

int main()
{
	{
		LambdaDiff lambdaDiff;
		std::cout << "lambdaEqual1: " << lambdaDiff.lambdaEqual1() << std::endl;
		lambdaDiff.member();
	}
	{
		LambdaDiff lambdaDiff;
		std::cout << "lambdaEqual2: " << lambdaDiff.lambdaEqual2() << std::endl;
		lambdaDiff.member();
	}
	{
		LambdaDiff lambdaDiff;
		std::cout << "lambdaEqual3: " << lambdaDiff.lambdaEqual3() << std::endl;
		lambdaDiff.member();
	}
	{
		LambdaDiff lambdaDiff;
		std::cout << "lambdaEqual4: " << lambdaDiff.lambdaEqual4() << std::endl;
		lambdaDiff.member();
	}
	{
		LambdaDiff lambdaDiff;
		std::cout << "lambdaEqual5: " << lambdaDiff.lambdaEqual5() << std::endl;
		lambdaDiff.member();
	}
	{
		LambdaDiff lambdaDiff;
		std::cout << "lambdaRef1: " << lambdaDiff.lambdaRef1() << std::endl;
		lambdaDiff.member();
	}
	{
		LambdaDiff lambdaDiff;
		std::cout << "lambdaRef2: " << lambdaDiff.lambdaRef2() << std::endl;
		lambdaDiff.member();
	}
	{
		LambdaDiff lambdaDiff;
		std::cout << "lambdaRef3: " << lambdaDiff.lambdaRef3() << std::endl;
		lambdaDiff.member();
	}
}
