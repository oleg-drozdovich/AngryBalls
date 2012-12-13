#ifndef GAME_H_
#define GAME_H_

#include "Object.h"
#include "Utils\Logger.h"

#include <list>
#include <time.h>
#include <memory>

class CFactory;

class CGame
{
public:
	typedef std::list<CObject*>	TObjects;

	static CGame& Instance()
	{
		static CGame game;
		return game;
	}

	void DoDraw();								// отрисовка сцены
	void DoUpdate();							// обновление состояния объектов
	void DoMouseDown( GLvector *pMouse );		// вызываем при нажатии кнопкой мышм в окне

	TObjects::iterator RemoveObject( CObject *pObject );

	clock_t GetCreateClock();					// возвращает кол-во тиков с момента последнего создания объекта
	clock_t GetUpdateClock();					// возвращает  кол-во тиков с момента последнего обновления объектов
private:
	CGame();

	float		__fPeriod;						// интервал создания объектов в секундах 
	clock_t		__clockLastCreated;				// тики в момент последнего создания объекта
	clock_t		__clockLastUpdated;				// тики в момент последнего обновления объектов
	TObjects	__Objects;						// контейнер в котором хранятся все объекты
	int			__iHitCounter;					// счетчик очков
	std::auto_ptr<CFactory>	__Factory;			
};


#endif /* GAME_H_ */
