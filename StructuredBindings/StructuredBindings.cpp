/*

構造化束縛に関する変更

・概要
	* 構造化束縛を使える範囲が拡がる

・詳細（★は下部にサンプルコードあり）
	★6.1 static/thread_local の許可
	https://cpprefjp.github.io/lang/cpp20/extending_structured_bindings_to_be_more_like_variable_declarations.html
	6.2 非公開メンバへのアクセス
	https://cpprefjp.github.io/lang/cpp20/allow_structured_bindings_to_accessible_members.html
	6.3 呼び出す get メンバ関数の考慮の変更
	https://cpprefjp.github.io/lang/cpp20/relaxing_the_structured_bindings_customization_point_finding_rules.html

・所感
	現状の開発では、どれも使う機会はあまりないだろう。
*/

#include <iostream>
#include <chrono>
#include <string>


auto getCurrentTimeInfo()
{
	std::chrono::utc_clock::time_point tp = std::chrono::utc_clock::now();
	return std::make_tuple(
		std::format("{:%Y}", tp),
		std::format("{:%m}", tp),
		std::format("{:%d}", tp),
		std::format("{:%H}", tp),
		std::format("{:%M}", tp),
		std::format("{:%S}", tp));
}


void displayTimeInfo()
{
#if 0
	auto [year, month, day, hour, minute, second] = getCurrentTimeInfo();	//C++17 OK
#else
	static auto [year, month, day, hour, minute, second] = getCurrentTimeInfo();	//C++20 OK
#endif
	std::cout << "year:" << year << std::endl;
	std::cout << "month:" << month << std::endl;
	std::cout << "day:" << day << std::endl;
	std::cout << "hour:" << hour << std::endl;
	std::cout << "minute:" << minute << std::endl;
	std::cout << "second:" << second << std::endl;
}

int main() {
	displayTimeInfo();
	std::cout << "-------------------" << std::endl;
	displayTimeInfo();
}