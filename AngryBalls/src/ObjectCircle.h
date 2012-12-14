#ifndef OBJECTCIRCLE_H_
#define OBJECTCIRCLE_H_


#include "Object.h"
#include "Utils\GLUtils.h"

class CObjectCircle : public CObject
{
	GLuint	__uRadius;
public:
	CObjectCircle( const GLvector &vPosition, const GLvector &vSpeed, GLuint uRGBA, GLuint uRadius );

	virtual void OnDraw();											// отрисовка объекта
	virtual void OnUpdate();										// обновляет состояние объекта
	virtual bool IsContaines( const GLvector &vPosition ) const;	// метод проверяет - лежит ли точка внутри объекта
	virtual bool IsContained( const GLvector &vBoxSize ) const;		// метод проверяет - лежит ли объект внутри прямоугольника
	virtual	int GetHitPoints() const;								// возращает кол-во очков за попадание
	virtual ~CObjectCircle();
};

#endif /* OBJECTCIRCLE_H_ */
