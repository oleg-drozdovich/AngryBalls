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

	void DoDraw();								// ��������� �����
	void DoUpdate();							// ���������� ��������� ��������
	void DoMouseDown( GLvector *pMouse );		// �������� ��� ������� ������� ���� � ����

	TObjects::iterator RemoveObject( CObject *pObject );

	clock_t GetCreateClock();					// ���������� ���-�� ����� � ������� ���������� �������� �������
	clock_t GetUpdateClock();					// ����������  ���-�� ����� � ������� ���������� ���������� ��������
private:
	CGame();

	float		__fPeriod;						// �������� �������� �������� � �������� 
	clock_t		__clockLastCreated;				// ���� � ������ ���������� �������� �������
	clock_t		__clockLastUpdated;				// ���� � ������ ���������� ���������� ��������
	TObjects	__Objects;						// ��������� � ������� �������� ��� �������
	int			__iHitCounter;					// ������� �����
	std::auto_ptr<CFactory>	__Factory;			
};


#endif /* GAME_H_ */
