#include <math.h>
#include <stdlib.h>

#include "ObjectCircle.h"
#include "Game.h"
#include "Utils\GLUtils.h"


CObjectCircle::CObjectCircle( const GLvector &vPosition, const GLvector &vSpeed, GLuint uRGBA, GLuint uRadius )
{
	__vPosition = vPosition;
	__vSpeed = vSpeed;
	__uRGBA = uRGBA;
	__uRadius = uRadius;
}

CObjectCircle::~CObjectCircle()
{
}

void CObjectCircle::OnDraw()
{
	// рисуем круг
	GLUtils::guDrawCircle( __vPosition, __uRadius, __uRGBA );
}

void CObjectCircle::OnUpdate()
{
	clock_t clock = CGame::Instance().GetUpdateClock();		
	// сдвигаем круг на расстояние t * V
	if( __vSpeed.fX )
	{
		__vPosition.fX += ( clock / 1000.0 ) * __vSpeed.fX;
	}
	if(  __vSpeed.fY )
	{
		__vPosition.fY += ( clock / 1000.0 ) * __vSpeed.fY;
	}
}

bool CObjectCircle::IsContaines( const GLvector &vPosition ) const
{
	// точка внутри круга?
	if( __uRadius >= sqrt( pow( vPosition.fX - __vPosition.fX, 2 ) + pow( vPosition.fY - __vPosition.fY, 2 ) ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CObjectCircle::IsContained( const GLvector &vBoxSize ) const
{
	// окружность целиком в прямоугольной области?
	if(	( __vPosition.fX - __uRadius ) > 0 &&
		( __vPosition.fY - __uRadius ) > 0 &&
		( __vPosition.fX + __uRadius ) < vBoxSize.fX &&
		( __vPosition.fY + __uRadius ) < vBoxSize.fY )
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CObjectCircle::GetHitPoints() const
{
	return ( 1.0 / ( __uRadius * __uRadius * __uRadius ) ) * 1000000; 
}
