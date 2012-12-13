#include <algorithm>
#include <assert.h>

#include "Game.h"
#include "Factory.h"

CGame::CGame() :	__fPeriod( 0.4 ),			// время между созданием объектов
					__clockLastCreated( 0 ),	// тики в момент последнего создания объекта
					__clockLastUpdated( 0 ),	// тики в момент последнего обновления объектов
					__Factory( new CFactory() )	// создаем фабрику
{
	srand ( time( NULL ) );
}

void CGame::DoMouseDown( GLvector *pMouse )		// вызываем при нажатии кнопкой мышки в окне
{
	assert( pMouse );

	for( TObjects::iterator i = __Objects.begin(); i != __Objects.end(); i++ )
	{
		if( (*i)->IsContaines( *pMouse ) )		// попали в объект
		{
			__iHitCounter += (*i)->GetHitPoints();
			RemoveObject( *i );					
			break;
		}
	}
}

void CGame::DoDraw()							// отрисовка сцена
{
	GLvector vScreenSize;

	GLUtils::guGetScreenSize( &vScreenSize );

	glClear( GL_COLOR_BUFFER_BIT );				// очищаем экран
	glColor3f( 1.0, 1.0, 1.0 );					// устанавливаем белый цвет для шрифта
	glRasterPos2f( 0, vScreenSize.fY - 5 );		// устанавливаем положение надписи

	GLUtils::guPrint( "%d", __iHitCounter );	// выводим значение счетчика
	// Вызываем метод OnDraw() для каждого объекта в списке
	std::for_each( __Objects.begin(), __Objects.end(), std::mem_fun( &CObject::OnDraw ) );		
}

void CGame::DoUpdate()						// 
{
	clock_t		clockCurrent;
	GLvector	vScreenSize;

	GLUtils::guGetScreenSize( &vScreenSize );

	clockCurrent = clock();

	if( GetCreateClock() / (float)CLOCKS_PER_SEC > __fPeriod )		// пришло время создавать новый объект?	
	{
		__Objects.push_back( __Factory->CreateFallingCircle() );	// создаем новый объект и помещаем в контейнер	
		__clockLastCreated = clockCurrent;							// запоминаем время создания объекта					
	}
	// Вызываем метод OnUpdate() для каждого объекта в списке
	std::for_each( __Objects.begin(), __Objects.end(), std::mem_fun( &CObject::OnUpdate ) );	

	TObjects::iterator i = __Objects.begin();
	while( i != __Objects.end() )
	{
		// находится внутри внтури прямоугольника окна
		if( !(*i)->IsContained( vScreenSize ) )		
		{	
			// удаляем объект
			i = RemoveObject( *i );					
		}
		else
		{
			i++;
		}
	}
	// запоминаем время обновления объектов
	__clockLastUpdated = clockCurrent;				
}

CGame::TObjects::iterator CGame::RemoveObject( CObject *pObject )
{
	TObjects::iterator	i;

	if( ( i = std::find( __Objects.begin(), __Objects.end(), pObject ) ) != __Objects.end() )
	{
		i = __Objects.erase( i );
	}
	delete pObject;
	return i;
}


clock_t CGame::GetUpdateClock() 					// кол-во тиков с момента последнего обновления объектов
{
	return clock() - __clockLastUpdated;	
}

clock_t CGame::GetCreateClock() 					// кол-во тиков с момента последнего создания объекта	
{
	return clock() - __clockLastCreated;	
}
