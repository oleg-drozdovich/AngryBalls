#ifndef COBJECT_H_
#define COBJECT_H_

#include "Utils\GLUtils.h"

class CObject
{
protected:
	GLvector	__vPosition;
	GLvector	__vSpeed;
	GLuint		__uRGBA;
public:
	virtual void OnDraw() = 0;											// ��������� �������
	virtual void OnUpdate() = 0;										// ��������� ��������� �������
	virtual bool IsContaines( const GLvector &vPosition ) const = 0;	// ����� ��������� - ����� �� ����� ������ �������
	virtual bool IsContained( const GLvector &vBoxSize ) const = 0;		// ����� ��������� - ����� �� ������ ������ ��������������
	virtual int	GetHitPoints() const = 0;								// ��������� ���-�� ����� �� ���������
	virtual ~CObject(){};
};

#endif /* COBJECT_H_ */
