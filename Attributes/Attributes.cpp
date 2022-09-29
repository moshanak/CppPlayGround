/*

属性に関する変更

・概要
	* 新しい属性が追加
	* nodiscard 属性の機能拡張

・詳細（★は下部にサンプルコードあり）
	8.1 空オブジェクトに対する最適化を支援する属性[[no_unique_address]]
	https://cpprefjp.github.io/lang/cpp20/language_support_for_empty_objects.html
	8.2 確率が高い分岐と低い分岐を伝える属性 [[likely]], [[unlikely]]
	https://cpprefjp.github.io/lang/cpp20/likely_and_unlikely_attributes.html
	★8.3 [[nodiscard]]属性に理由となる文字列を付加できるようにする
	https://cpprefjp.github.io/lang/cpp20/nodiscard_should_have_a_reason.html
	8.4 [[nodiscard]]をコンストラクタのオーバーロードごとに付加できるようにする
	https://cpprefjp.github.io/lang/cpp20/nodiscard_for_constructors.html

・所感
	8.3 は使うと良いと思う。
	8.1 は MSVC では固有の書き方　[[msvc::no_unique_address]]　をしないと機能しない。現状の開発では使う必要性はないだろう。
	8.2 は試したが効果は分からなかった。コンパイラ依存で必ず処理が最適化されるわけでもないため、使わなくてよい。
	8.4 はコンストラクタに nodiscard を付けても良いが、そこまで必要とは思わない。
*/


#include <iostream>
#include <fstream>
#include <filesystem>

class FileReader {
public:
	[[nodiscard("Need to check read error.")]]
	bool read(const std::filesystem::path& filePath) {
		std::ifstream ifs(filePath);
		if (ifs.fail()) {
			return false;
		}

		ifs >> readData_;
		return true;
	}

	const std::string& getReadData() const { return readData_; }

private:
	std::string readData_;
};

int main() {
	FileReader fileReader;
	fileReader.read("aaa.txt");	// ビルド時に warning を出力するときに、nodiscard で指定したメッセージも出してくれる。
	std::cout << fileReader.getReadData() << std::endl;
}
