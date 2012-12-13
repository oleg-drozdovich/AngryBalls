// AngryBalls.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>	
#include <math.h>	
#include <exception>	

#include "Utils\GLUtils.h"
#include "Game.h"

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

LRESULT	CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );	

int WINAPI WinMain(	HINSTANCE	hInstance,		
					HINSTANCE	hPrevInstance,		
					LPSTR		lpCmdLine,			
					int			nCmdShow )			
{
	LOG_SETFILENAME( "AngryBalls.log" );

	MSG		msg;			
	bool	isQuit = false;	
	CGame	&game = CGame::Instance();					// ������� ������ Game

	// ������� ����
	try
	{
		if( !GLUtils::guCreateWindow( "Angry Balls", WndProc, SCREEN_WIDTH, SCREEN_HEIGHT ) )		
		{
			throw std::exception();							
		}
		// ������� �����
		if( !GLUtils::guCreateFont( "Courier New" ) )		
		{
			throw std::exception();	
		}

		glEnableClientState( GL_VERTEX_ARRAY );				// �������� ����� ������� ��� ������
		glEnableClientState( GL_COLOR_ARRAY );				// � ������

		glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );	// ����� ������� ������� ������
		glMatrixMode( GL_PROJECTION );						// ����� ������� ��������
		glLoadIdentity();									// ����� ������� ��������
		glOrtho( 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 2.0f, -2.0f );	// ������������� ������������� �������� ������
		glMatrixMode( GL_MODELVIEW );						// ����� ������� ���� ������
		glLoadIdentity();									// ����� ������� ���� ������

		while( !isQuit )								
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) // ����� ��������� �� �������, ���� ����
			{
				if( msg.message == WM_QUIT )					
				{
					isQuit = true;							
				}
				else										
				{
					TranslateMessage( &msg );				// ����������� ������		
					DispatchMessage( &msg );				// � ���������
				}
			}
			else									
			{
				game.DoUpdate();
				game.DoDraw();

				SwapBuffers( GLUtils::__hDC );				// ������ ������
			}
		}
		GLUtils::guDeleteWindow();
	}
	catch( std::bad_alloc& e )
	{
		LOG_ERROR( "ERROR Not enough memory. Terminating..." );
		return -1;
	}
	catch( ... )
	{
		LOG_ERROR( "ERROR Terminating..." );
		return -1;
	}
								
	return ( msg.wParam );									
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			
							UINT	uMsg,		
							WPARAM	wParam,			
							LPARAM	lParam)			
{
	switch ( uMsg )		// ������������ ���������							
	{
		case WM_CLOSE:							
		{
			PostQuitMessage(0);					
			return 0;						
		}

		case WM_LBUTTONDOWN:							
		{
			GLvector vMouse;

			vMouse.fX = LOWORD(lParam);
			vMouse.fY = HIWORD(lParam);
			CGame::Instance().DoMouseDown( &vMouse );	
			return 0;						
		}
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
