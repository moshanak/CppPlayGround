#include "OtherFile.h"
#include <iostream>

int main()
{
	const std::source_location location = std::source_location::current();
	// 行番号
	std::cout << location.line() << std::endl;
	// 列番号
	std::cout << location.column() << std::endl;
	// ファイルのフルパス
	std::cout << location.file_name() << std::endl;
	// 関数名
	std::cout << location.function_name() << std::endl;

	std::cout << std::endl;

	OtherFile().current();
	OtherFile().currentCaller();

	return 0;
}