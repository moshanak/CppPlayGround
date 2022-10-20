/*

その他の変更

・概要
	* どれも機能がバラバラなので一言でまとめにくい。概要は省略。

・詳細（★は下部にサンプルコードあり）
	11.1 ほとんどのvolatileを非推奨化
	https://cpprefjp.github.io/lang/cpp20/deprecating_volatile.html
		volatile は最適化をやめてくれる機能。詳細は下記のサイトが参考になるが、現在の開発で使うケースは無いと思う。
		https://onihusube.hatenablog.com/entry/2019/10/09/184906
	11.2 ビットフィールドのデフォルトメンバ初期化
	https://cpprefjp.github.io/lang/cpp20/default_member_initializers_for_bit_fields.html
	★11.3 using enum
	https://cpprefjp.github.io/lang/cpp20/using_enum.html
	11.4 入れ子 inline 名前空間定義の簡易化
	https://cpprefjp.github.io/lang/cpp20/nested_inline_mamespaces.html
	11.5 __VA_OPT__
	https://cpprefjp.github.io/lang/cpp20/va_opt.html
		msvc でも対応しているとは書かれているが、__VA_OPT__ はコンパイルエラーになるため、使えない。
	11.6 添字演算子内カンマの非推奨化
	https://cpprefjp.github.io/lang/cpp20/deprecate_uses_of_the_comma_operator_in_subscripting_expressions.html
	11.7 トリビアルな型のオブジェクトを暗黙的に構築する

・所感
	どれも、そこまで使うケースは無い印象。
	11.3は使ってもよいと思うが、エイリアス宣言でも良いと思う。
*/

#include <iostream>

class DisplayCommandBar
{
public:
	// 現在の開発では、QML とプロパティで繋ぐために class 内に enum を定義するケースがある
	enum class DisplayType : int
	{
		wireframe,
		shading,
		transparent
	};
	//Q_ENUM(DisplayType)


	DisplayCommandBar(): currentType_(DisplayType::wireframe) {}
	const DisplayType& getCurrentType() const { return currentType_; }

private:
	DisplayType currentType_;
};


int main()
{
	DisplayCommandBar bar;
	const DisplayCommandBar::DisplayType type = bar.getCurrentType();

	// 何もしない場合
	{
		// 長い。
		if (type == DisplayCommandBar::DisplayType::wireframe) {
			std::cout << "Current display type is wireframe." << std::endl;
		}
	}

	// using (エイリアス宣言) を使う  C++11
	{
		// 短くできる
		using DisplayType = DisplayCommandBar::DisplayType;
		if (type == DisplayType::wireframe) {
			std::cout << "Current display type is wireframe." << std::endl;
		}
	}

	// using enum を使う C++20
	{
		// さらに短くできる
		using enum DisplayCommandBar::DisplayType;
		if (type == wireframe) {
			std::cout << "Current display type is wireframe." << std::endl;
		}
	}
}
