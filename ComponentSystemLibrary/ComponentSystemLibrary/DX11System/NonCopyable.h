#pragma once

// �h���N���X���R�s�[�֎~�ɂ���B
// private�p�����Ďg���B
class NonCopyable{
protected:
    NonCopyable(){}
    ~NonCopyable(){}

private:
    NonCopyable(const NonCopyable&);
    const NonCopyable& operator=(const NonCopyable&);
};