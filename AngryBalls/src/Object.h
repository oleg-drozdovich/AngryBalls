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
	virtual void OnDraw() = 0;											// отрисовка объекта
	virtual void OnUpdate() = 0;										// обновляет состояние объекта
	virtual bool IsContaines( const GLvector &vPosition ) const = 0;	// метод проверяет - лежит ли точка внутри объекта
	virtual bool IsContained( const GLvector &vBoxSize ) const = 0;		// метод проверяет - лежит ли объект внутри прямоугольника
	virtual int	GetHitPoints() const = 0;								// возращает кол-во очков за попадание
	virtual ~CObject(){};
};

#endif /* COBJECT_H_ */
