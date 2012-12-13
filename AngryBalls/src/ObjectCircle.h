#ifndef OBJECTCIRCLE_H_
#define OBJECTCIRCLE_H_


#include "Object.h"
#include "Utils\GLUtils.h"

class CObjectCircle : public CObject
{
	GLuint	__uRadius;
public:
	CObjectCircle( const GLvector &vPosition, const GLvector &vSpeed, GLuint uRGBA, GLuint uRadius );

	virtual void OnDraw();											// ��������� �������
	virtual void OnUpdate();										// ��������� ��������� �������
	virtual bool IsContaines( const GLvector &vPosition ) const;	// ����� ��������� - ����� �� ����� ������ �������
	virtual bool IsContained( const GLvector &vBoxSize ) const;		// ����� ��������� - ����� �� ������ ������ ��������������
	virtual	int GetHitPoints() const;								// ��������� ���-�� ����� �� ���������
	virtual ~CObjectCircle();
};

#endif /* OBJECTCIRCLE_H_ */
