#include "Try_source_location.h"
#include <iostream>

void Try_source_location::current()
{
	const std::source_location location = std::source_location::current();
	//�s�ԍ�
	std::cout << location.line() << std::endl;
	//��ԍ�
	std::cout << location.column() << std::endl;
	//�t�@�C�����B�����ƃt�@�C���̃t���p�X���Ԃ��Ă����B
	std::cout << location.file_name() << std::endl;
	//�֐���
	std::cout << location.function_name() << std::endl;
}

void Try_source_location::currentCaller(std::source_location location)
{
	//���ׂČĂяo�����̏��ɂȂ�
	std::cout << location.line() << std::endl;
	std::cout << location.column() << std::endl;
	std::cout << location.file_name() << std::endl;
	std::cout << location.function_name() << std::endl;
}
