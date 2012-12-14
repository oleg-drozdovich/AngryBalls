#include <assert.h>
#include <stdlib.h>

#include "Factory.h"
#include "ObjectCircle.h"
#include "Game.h"
#include "Utils\GLUtils.h"

#define CIRCLE_MAX_RADIUS	40
#define CIRCLE_MIN_RADIUS	20
#define CIRCLE_BASE_SPEED	100.0

GLuint	__Colors[] = { 0xFF, 0xFF00, 0xFF0000, 0xFF00FF, 0xFFFFFF, 0xFFFF };

CObject* CFactory::CreateFallingCircle()
{
	int			size, index;
	GLvector	vPosition, vSpeed, vScreenSize;
	GLuint		uRadius, uRGBA;

	size	= sizeof( __Colors ) / sizeof( GLuint );		// размер массива RGB
	index	= rand() % size;								// случайный индекс в массиве RGB
	assert( index < size );

	GLUtils::guGetScreenSize( &vScreenSize );

	uRGBA = __Colors[index];

	uRadius = rand() % ( CIRCLE_MAX_RADIUS - CIRCLE_MIN_RADIUS + 1 ) + CIRCLE_MIN_RADIUS;	// генерируем радиус в интервале от CIRCLE_MIN_RADIUS до CIRCLE_MAX_RADIUS

	vPosition.fX = rand() % (int)( vScreenSize.fX - 2 * uRadius + 1 ) + uRadius;			// генерируем начальное положение круга
	vPosition.fY = uRadius;

	vSpeed.fX = 0;
	vSpeed.fY = CIRCLE_BASE_SPEED * ( CIRCLE_MAX_RADIUS / (float)uRadius );					// генерируем скорость 				

	return new CObjectCircle( vPosition, vSpeed, uRGBA, uRadius );							// создаем новый круг
}
