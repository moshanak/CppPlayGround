/*

定数式に関する変更

・概要
	constexpr の制約が減る。
	consteval, constinitのように、コンパイル時のみ実行可能にする機能が追加。（constexpr はコンパイル時「も」実行可能にする。）

・詳細（★は下部にサンプルコードあり）
	3.1 仮想関数に constexpr を指定可能
	https://cpprefjp.github.io/lang/cpp20/allow_virtual_function_calls_in_constant_expressions.html

	3.2 constexpr 関数内で dynamic_cast を使用可能。また、多態的な型に対する typeid も使用可能。
	https://cpprefjp.github.io/lang/cpp20/allowing_dynamic_cast_polymorphic_typeid_in_constant_expressions.html

	3.3 constexpr 関数内で try-catch を使用可能。
	https://cpprefjp.github.io/lang/cpp20/try-catch_blocks_in_constexpr_functions.html

	3.4 constexpr 関数内で共用体 (union) のアクティブメンバの切り替え可能。
	https://cpprefjp.github.io/lang/cpp20/changing_the_active_member_of_a_union_inside_constexpr.html

	3.5 constexpr関数内でのトリビアルなデフォルト初期化を許可。
	トリビアルなデフォルト初期化とは
　	 ・T t; と変数宣言のように書き、ユーザー定義のコンストラクタがないクラスや構造体を初期化し、メンバ変数は不定になること。
　	 ・int n; と変数宣言のように書き、組み込み型（基本型、プリミティブ型、原始型とも呼ばれる）を初期化し、組み込み型の値は不定になること。
	https://cpprefjp.github.io/lang/cpp20/permitting_trivial_default_initialization_in_constexpr_contexts.html

	★3.6 constevalキーワード。コンパイル時のみ実行可能にする定数式を定義できる。constevalを付けた関数は即時間数と呼ばれる。
	https://cpprefjp.github.io/lang/cpp20/immediate_functions.html

	★3.7 is_constant_evaluated関数。コンパイル時のみ実行可能であれば true を返す。コンパイル時か実行時かで処理を切り替えることができる。
	https://cpprefjp.github.io/reference/type_traits/is_constant_evaluated.html

	3.8 constexpr 関数内でインラインアセンブラを使用可能。
	https://cpprefjp.github.io/lang/cpp20/enabling_constexpr_intrinsics_by_permitting_unevaluated_inline-assembly_in_constexpr_functions.html

	★3.9 constinitキーワード。コンパイル時のみ初期化する変数を定義できる。グローバル変数や static 変数などに使用可能。
	https://cpprefjp.github.io/lang/cpp20/constinit.html

	3.10 constexpr 関数内で動的メモリ確保を許可。constexpr 関数内で std::vector, std::string を使用可能になる。デストラクタに constexpr を付けることができる。
	https://cpprefjp.github.io/lang/cpp20/more_constexpr_containers.html

・所感
	業務で consteval や constinit を使わなければならないケースはあまり思いつかない。基本的には constexpr だけで十分に感じる。
	何か良い事例が思いついたら教えてほしい。

・参考
	const一族の違い
	https://onihusube.hatenablog.com/entry/2019/08/02/190341
	https://gununu.hatenadiary.jp/entry/2019/10/15/020903

*/

#include <iostream>

//----------------------------------------
//関数
//----------------------------------------
//コンパイル時と実行時のどちらで呼ばれるかは、呼び出し元に依存する
constexpr int sqr_expr(int n) {
	if (std::is_constant_evaluated()) {
		return n * n;
	}
	else {
		return -1;
	}
}

//コンパイル時に呼ばれる
consteval int sqr_eval(int n) {
	return n * n;
}

//関数に constinit を付けることはできない
//constinit int sqr_init(int n) {
//	return n * n;
//}

//実行時に呼ばれる
int sqr(int n) {
	return n * n;
}

#if 0
//----------------------------------------
//グローバル変数
//----------------------------------------

//コンパイル時に初期化
constexpr int i_expr_expr = sqr_expr(100);
constexpr int i_expr_eval = sqr_eval(100);
//constexpr int i_expr = sqr(100);	//NG: sqr関数はコンパイル時に呼び出すことはできない

//変数に consteval を付けることはできない
//consteval int i_eval = sqr_eval(100);

//コンパイル時に初期化
constinit int i_init_expr = sqr_expr(100);
constinit int i_init_eval = sqr_eval(100);
//constinit int i_init = sqr(100);		//NG: sqr関数はコンパイル時に呼び出すことはできない
constinit const int i_initconst_expr = sqr_expr(100);
constinit const int i_initconst_eval = sqr_eval(100);

//コンパイル時に初期化
const int i_const_expr = sqr_expr(100);
const int i_const_eval = sqr_eval(100);
//実行時に初期化
int i_expr = sqr_expr(100);	//おそらく実行時に初期化。sqr_exprはコンパイル時に実行。
int i_eval = sqr_eval(100);	//おそらく実行時に初期化。sqr_evalはコンパイル時に実行。
int i = sqr(100);
const int i_const = sqr(100);

int main() {

	static_assert(i_expr_expr == 10000, "100 * 100 == 10000");
	static_assert(i_expr_eval == 10000, "100 * 100 == 10000");
	//static_assert(i_init_expr == 10000, "100 * 100 == 10000");		//NG: const を付けない constinit は変数になるため、static_assert には使えない
	//static_assert(i_init_eval == 10000, "100 * 100 == 10000");		//NG: 同上
	static_assert(i_initconst_expr == 10000, "100 * 100 == 10000");
	static_assert(i_initconst_eval == 10000, "100 * 100 == 10000");
	static_assert(i_const_expr == 10000, "100 * 100 == 10000");
	static_assert(i_const_eval == 10000, "100 * 100 == 10000");
	//static_assert(i_expr == 10000, "100 * 100 == 10000");				//NG: const を付けていない変数になるため、static_assert には使えない
	//static_assert(i_eval == 10000, "100 * 100 == 10000");				//NG: 同上
	//static_assert(i == 10000, "100 * 100 == 10000");					//NG: コンパイル時に初期化されないため、static_assert には使えない
	//static_assert(i_const == 10000, "100 * 100 == 10000");			//NG: 同上


	//i_expr_expr += 1;			//NG: constexpr を付けた変数は定数になる。暗黙的にconstになる。 
	//i_expr_eval += 1;			//NG: 同上
	i_init_expr += 1;
	i_init_eval += 1;
	//i_initconst_expr += 1;	//NG: const を付けているため。
	//i_initconst_eval += 1;	//NG: 同上
	//i_const_expr += 1;			//NG: 同上
	//i_const_eval += 1;			//NG: 同上
	i_expr += 1;
	i_eval += 1;
	i += 1;
	//i_const += 1;				//NG: const を付けているため。


	std::cout << "i_expr_expr: " << i_expr_expr << std::endl;
	std::cout << "i_expr_eval: " << i_expr_eval << std::endl;
	std::cout << "i_init_expr: " << i_init_expr << std::endl;
	std::cout << "i_init_eval: " << i_init_eval << std::endl;
	std::cout << "i_initconst_expr: " << i_initconst_expr << std::endl;
	std::cout << "i_initconst_eval: " << i_initconst_eval << std::endl;
	std::cout << "i_const_expr: " << i_const_expr << std::endl;
	std::cout << "i_const_eval: " << i_const_eval << std::endl;
	std::cout << "i_expr: " << i_expr << std::endl;
	std::cout << "i_eval: " << i_eval << std::endl;
	std::cout << "i: " << i << std::endl;
	std::cout << "i_const: " << i_const << std::endl;
}
#else

int main() {
	//コンパイル時に初期化
	constexpr int i_expr_expr = sqr_expr(100);
	constexpr int i_expr_eval = sqr_eval(100);
	//constexpr int i_expr = sqr(100);	//NG: sqr関数はコンパイル時に呼び出すことはできない

	//変数に consteval を付けることはできない
	//consteval int i_eval = sqr_eval(100);

	//ローカル変数に constinit を付けることはできない
	//constinit int i_init_eval = sqr_eval(100);

	//static ローカル変数なら constinit を付けることが可能
	//コンパイル時に初期化
	constinit static int i_init_expr = sqr_expr(100);
	constinit static int i_init_eval = sqr_eval(100);
	//constinit static int i_init = sqr(100);		//NG: sqr関数はコンパイル時に呼び出すことはできない
	constinit static const int i_initconst_expr = sqr_expr(100);
	constinit static const int i_initconst_eval = sqr_eval(100);

	//コンパイル時に初期化
	const int i_const_expr = sqr_expr(100);
	const int i_const_eval = sqr_eval(100);
	//実行時に初期化
	int i_expr = sqr_expr(100);	//おそらく実行時に初期化。sqr_exprはランタイム時に実行。
	int i_eval = sqr_eval(100);	//おそらく実行時に初期化。sqr_evalはコンパイル時に実行。
	int i = sqr(100);
	const int i_const = sqr(100);

	//----------------------------------------
	//ローカル変数
	//----------------------------------------

	static_assert(i_expr_expr == 10000, "100 * 100 == 10000");
	static_assert(i_expr_eval == 10000, "100 * 100 == 10000");
	//static_assert(i_init_expr == 10000, "100 * 100 == 10000");		//NG: const を付けない constinit は変数になるため、static_assert には使えない
	//static_assert(i_init_eval == 10000, "100 * 100 == 10000");		//NG: 同上
	static_assert(i_initconst_expr == 10000, "100 * 100 == 10000");
	static_assert(i_initconst_eval == 10000, "100 * 100 == 10000");
	static_assert(i_const_expr == 10000, "100 * 100 == 10000");
	static_assert(i_const_eval == 10000, "100 * 100 == 10000");
	//static_assert(i_expr == 10000, "100 * 100 == 10000");				//NG: const を付けていない変数になるため、static_assert には使えない
	//static_assert(i_eval == 10000, "100 * 100 == 10000");				//NG: 同上
	//static_assert(i == 10000, "100 * 100 == 10000");					//NG: コンパイル時に初期化されないため、static_assert には使えない
	//static_assert(i_const == 10000, "100 * 100 == 10000");			//NG: 同上


	//i_expr_expr += 1;			//NG: constexpr を付けた変数は定数になる。暗黙的にconstになる。 
	//i_expr_eval += 1;			//NG: 同上
	i_init_expr += 1;
	i_init_eval += 1;
	//i_initconst_expr += 1;	//NG: const を付けているため。
	//i_initconst_eval += 1;	//NG: 同上
	//i_const_expr += 1;			//NG: 同上
	//i_const_eval += 1;			//NG: 同上
	i_expr += 1;
	i_eval += 1;
	i += 1;
	//i_const += 1;				//NG: const を付けているため。


	std::cout << "i_expr_expr: " << i_expr_expr << std::endl;
	std::cout << "i_expr_eval: " << i_expr_eval << std::endl;
	std::cout << "i_init_expr: " << i_init_expr << std::endl;
	std::cout << "i_init_eval: " << i_init_eval << std::endl;
	std::cout << "i_initconst_expr: " << i_initconst_expr << std::endl;
	std::cout << "i_initconst_eval: " << i_initconst_eval << std::endl;
	std::cout << "i_const_expr: " << i_const_expr << std::endl;
	std::cout << "i_const_eval: " << i_const_eval << std::endl;
	std::cout << "i_expr: " << i_expr << std::endl;
	std::cout << "i_eval: " << i_eval << std::endl;
	std::cout << "i: " << i << std::endl;
	std::cout << "i_const: " << i_const << std::endl;
}
#endif