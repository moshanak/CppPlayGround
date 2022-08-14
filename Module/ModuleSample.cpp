/*

モジュール

・概要：
　ヘッダファイルに代わる新しいファイル分割機能

・メリット
　おおむねコンパイル時間が短くなる。#include を記述した箇所にヘッダファイルのコードがコピペされなくなるため。
  インクルードガードは不要になる。

・所感
　メリットは良いが、下記のデメリットもあると思う。
	* 既存のソースコードが多いほど、書き換えに工数がかかる。
	* コーディング規約も大幅な変更が必要になる。
	* C++20では標準ライブラリはモジュールになっていない (C++23以降)
	* 分散ビルドを使う場合、逆に遅くなるケースも報告されており、試さないと分からない。
	  https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1441r1.pdf
  もしコンパイル時間が大きな問題になれば、モジュールを使うことを検討しても良いが、直近では不要だろう。
  将来的にはモジュールが主流になるかもしれないため、知っておくことは損ではない。

・cpprefjp
　https://cpprefjp.github.io/lang/cpp20/modules.html

*/

#if 0
//#include "ElementID.h"	//ElementIDクラスにインクルードガードをつけていないとコンパイルエラーになる
#include "Element.h"

int main()
{
	Element element(ElementID(2));
	element.outputLog();
	return 0;
}
#else
import elementIDLib;	//インクルードガードはないがコンパイルエラーにならない
import elementLib;

int main()
{
	Element element(ElementID(2));
	element.outputLog();
	return 0;
}
#endif