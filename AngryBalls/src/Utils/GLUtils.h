#ifndef GLUTILS_H_
#define GLUTILS_H_

#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>	

struct GLvector
{
	GLfloat	fX;
	GLfloat	fY;
};

struct GLvertex
{
	GLfloat	fX, fY;
	GLfloat	fU, fV;
	GLuint	uRGBA;
};

namespace GLUtils
{
	extern HDC			__hDC;			// Private GDI Device Context
	extern HGLRC		__hRC;			// Permanent Rendering Context
	extern HWND			__hWnd;			// Holds Our Window Handle
	extern HINSTANCE	__hInstance;	// Holds The Instance Of The Application
	extern GLint		__iList;		// Display list

	BOOL guCreateWindow( LPCSTR zTitle, WNDPROC WndProc, DWORD uWidth, DWORD uHeight );
	BOOL guCreateFont( LPCSTR zFontName );
	void guDeleteWindow();
	void guDeleteFont();
	void guPrint( LPCSTR fmt, ... );
	bool guGetScreenSize( GLvector *pvSize );
	bool guGetCursorPos( GLvector *pvPos );
		
	void guDrawCircle( const GLvector& vPosition, GLuint uRadius, GLuint uRGBA );

};

#endif /* GLUTILS_H_ */