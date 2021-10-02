#include "Try_source_location.h"
#include <iostream>

void Try_source_location::current()
{
	const std::source_location location = std::source_location::current();
	//行番号
	std::cout << location.line() << std::endl;
	//列番号
	std::cout << location.column() << std::endl;
	//ファイル名。試すとファイルのフルパスが返ってきた。
	std::cout << location.file_name() << std::endl;
	//関数名
	std::cout << location.function_name() << std::endl;
}

void Try_source_location::currentCaller(std::source_location location)
{
	//すべて呼び出し元の情報になる
	std::cout << location.line() << std::endl;
	std::cout << location.column() << std::endl;
	std::cout << location.file_name() << std::endl;
	std::cout << location.function_name() << std::endl;
}
