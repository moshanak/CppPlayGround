#pragma once
#include <source_location>

//�\�[�X�R�[�h��̈ʒu��\���N���X
class Try_source_location
{
public:

	//���̊֐��̌Ăяo�����̃\�[�X�R�[�h��̈ʒu��Ԃ�
	void current();

	//�Ăяo�����̃\�[�X�R�[�h�̈ʒu���擾������@
	//�f�t�H���g�����ɂ��āA�Ăяo�����ł͈����Ȃ��ŌĂ�ł��A�Ăяo�����̏���\���ł���
	void currentCaller(std::source_location location = std::source_location::current());
};