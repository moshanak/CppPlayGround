#include "concept_demo.h"
#include <concepts>
#include <iostream>

#if 1
// ���l�^(����or���������_��)�𔻒肷��R���Z�v�g
template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

// #1 �v���C�}���֐��e���v���[�g(�ȗ��\��)
void twice(auto) {
	std::cout << "*" << std::endl;
}

// #2 ���l�^�ɑ΂��ėD�悳���֐��e���v���[�g(�ȗ��\��)
void twice(Number auto x) {
	std::cout << (x * 2) << std::endl;
}

// #3 �����Ȃ������^�ɑ΂��čŗD�悳���֐��e���v���[�g(�ȗ��\��)
void twice(std::unsigned_integral auto x) {
	std::cout << (x * 2) << "u" << std::endl;
}
#elif 0
//C++17�ȑO�̏�����1

// �֐��I�[�o�[���[�h
void twice(...) {
	std::cout << "*" << std::endl;
}

void twice(int x) {
	std::cout << (x * 2) << std::endl;
}

void twice(long long x) {
	std::cout << (x * 2) << std::endl;
}

void twice(float x) {
	std::cout << (x * 2) << std::endl;
}

void twice(double x) {
	std::cout << (x * 2) << std::endl;
}

void twice(unsigned char x) {
	std::cout << (x * 2) << "u" << std::endl;
}

void twice(unsigned int x) {
	std::cout << (x * 2) << "u" << std::endl;
}

void twice(unsigned long long x) {
	std::cout << (x * 2) << "u" << std::endl;
}

#else
//C++17�ȑO�̏�����2
//�֐��e���v���[�g

// ���l�^(����or���������_��)�𔻒肷�郁�^�֐�
template <typename T>
struct is_number : std::integral_constant<bool, std::is_integral<T>::value || std::is_floating_point<T>::value>
{};

// #1 ���l�^�ȊO�ɑ΂��ėL���ƂȂ�֐��e���v���[�g
template <typename T>
typename std::enable_if<!is_number<T>::value>::type
twice(T) {
	std::cout << "*" << std::endl;
}

// #2 ���l�^�������Ȃ������^�ȊO�ɑ΂��ėL���ƂȂ�֐��e���v���[�g
template <typename T>
typename std::enable_if<is_number<T>::value && !std::is_unsigned<T>::value>::type
twice(T x) {
	std::cout << (x * 2) << std::endl;
}

// #3 ���l�^�������Ȃ������^�ɑ΂��ėL���ƂȂ�֐��e���v���[�g
template <typename T>
typename std::enable_if<is_number<T>::value&& std::is_unsigned<T>::value>::type
twice(T x) {
	std::cout << (x * 2) << "u" << std::endl;
}
#endif

void concept_demo::demo()
{
	//���l��2�{
	twice(1);
	twice(1.0);
	//�����Ȃ������͖�����u��t����
	twice(1u);
	//����ȊO�� * ���o��
	twice("aaa");
}
