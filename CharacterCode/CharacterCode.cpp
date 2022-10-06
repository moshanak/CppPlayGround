/*

文字コードに関する変更

・概要
	* UTF-8 を扱う char8_t 型が新たに追加される

・詳細（★は下部にサンプルコードあり）
	★7.1 UTF-8エンコーディングされた文字の型としてchar8_tを追加
	https://cpprefjp.github.io/lang/cpp20/char8_t.html
		関連して非推奨になる機能がある
		https://cpprefjp.github.io/reference/filesystem/u8path.html
	7.2 char16_tとchar32_tの文字・文字列リテラルを、文字コードUTF-16/32に規定
	https://cpprefjp.github.io/lang/cpp20/make_char16t_char32t_string_literals_be_utf16_32.html

・所感
	現状の開発で大きな影響はないと思うが、char は char8_t に変えると明確に文字コードが分かるため、変えると良いだろう。
		* char,string ・・・ utf-8 (visual studio の設定 [/source-charset:utf-8 /execution-charset:utf-8])
		* char8_t ・・・ utf-8
		* QString ・・・ utf-16
	また、意図的に QString から string への変換などで、u8path などの C++ の API を呼んでいる箇所は影響が出るため、注意。
*/

#include <iostream>
#include <fstream>
#include <filesystem>

int main()
{
#if 0
	// C++20 から非推奨
	std::filesystem::path filepathShiftJIS = std::filesystem::u8path("てすとShiftJIS.txt");
	std::filesystem::path filepathUTF8 = std::filesystem::u8path("てすとutf8.txt");
#else
	//C++20 からは char8_t 型を std::filesystem::path のコンストラクタへ渡すことができる
	std::filesystem::path filepathShiftJIS(u8"てすとShiftJIS.txt");
	std::filesystem::path filepathUTF8(u8"てすとutf8.txt");
#endif
	std::ifstream ifsShiftJIS(filepathShiftJIS);
	std::ifstream ifsUTF8(filepathUTF8);
	if (!ifsShiftJIS.is_open() || !ifsUTF8.is_open()) {
		return 0;
	}

	std::string readtxtShiftJIS;
	ifsShiftJIS >> readtxtShiftJIS;

	std::string readtxtUTF8;
	ifsUTF8 >> readtxtUTF8;

	std::cout << readtxtShiftJIS << std::endl;	// ShiftJIS （てすとShiftJIS.txt は ShiftJIS の文字コードで保存）
	std::cout << readtxtUTF8 << std::endl;		// UTF-8　	（てすとutf8.txt は UTF-8 の文字コードで保存）

	const char* test = "てすと";
	std::string strtest = "てすと";
	std::cout << test << std::endl;			// UTF-8　(Visual Studio の設定依存)
	std::cout << strtest << std::endl;		// UTF-8　(Visual Studio の設定依存)

	const char8_t* testu8 = u8"てすと";
	std::u8string strtestu8 = u8"てすと";
	std::cout << reinterpret_cast<const char*>(testu8) << std::endl;					// UTF-8
	std::cout << reinterpret_cast<const char*>(strtestu8.c_str()) << std::endl;			// UTF-8
}
