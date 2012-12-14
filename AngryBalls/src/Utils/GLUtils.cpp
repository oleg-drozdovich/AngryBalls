#include <assert.h>
#include <math.h>

#include "GLUtils.h"
#include "Logger.h"

#define CIRCLE_LINE_NUMBER	32
#define	MATH_KF_2_PI		( 3.14159265 * 2 )

#define GL_TEXT_BUFFER_SIZE	256
#define GL_COUNT_GLYPH		96
#define GL_FIRST_GRLYPH		32

HDC			GLUtils::__hDC			= NULL;						// Private GDI Device Context
HGLRC		GLUtils::__hRC			= NULL;						// Permanent Rendering Context
HWND		GLUtils::__hWnd			= NULL;						// Holds Our Window Handle
HINSTANCE	GLUtils::__hInstance	= NULL;						// Holds The Instance Of The Application
GLint		GLUtils::__iList		= 0;


BOOL GLUtils::guCreateFont( LPCSTR zFontName )	
{
	HFONT	font;												// Windows Font ID
	HFONT	oldfont;											// Used For Good House Keeping

	__iList = glGenLists( GL_COUNT_GLYPH );						// Storage For 96 Characters

	if( !( font = CreateFont(	-24,							// Height Of Font
								0,								// Width Of Font
								0,								// Angle Of Escapement
								0,								// Orientation Angle
								FW_BOLD,						// Font Weight
								FALSE,							// Italic
								FALSE,							// Underline
								FALSE,							// Strikeout
								ANSI_CHARSET,					// Character Set Identifier
								OUT_TT_PRECIS,					// Output Precision
								CLIP_DEFAULT_PRECIS,			// Clipping Precision
								ANTIALIASED_QUALITY,			// Output Quality
								FF_DONTCARE | DEFAULT_PITCH,	// Family And Pitch
								zFontName ) ) )					// Font Name
	{
		LOG_ERROR( "ERROR %d guCreateFont::CreateFont", ::GetLastError() );
		return FALSE;
	}

	oldfont = (HFONT)SelectObject( __hDC, font );							// Selects The Font We Want
	wglUseFontBitmaps( __hDC, GL_FIRST_GRLYPH, GL_COUNT_GLYPH, __iList );	// Builds 96 Characters Starting At Character 32
	SelectObject( __hDC, oldfont );											// Selects The Font We Want
	DeleteObject( font );													// Delete The Font
	return TRUE;
}

void GLUtils::guPrint( LPCSTR fmt, ... )									// Custom GL "Print" Routine
{
	assert( fmt );

	char		zText[GL_TEXT_BUFFER_SIZE];									// Holds Our String
	va_list		ap;															// Pointer To List Of Arguments

	va_start( ap, fmt );													// Parses The String For Variables
	vsprintf( zText, fmt, ap );												// And Converts Symbols To Actual Numbers
	va_end( ap );															// Results Are Stored In Text

	glPushAttrib( GL_LIST_BIT );											// Pushes The Display List Bits
	glListBase( __iList - GL_FIRST_GRLYPH );								// Sets The Base Character to 32
	glCallLists( strlen( zText ), GL_UNSIGNED_BYTE, zText );				// Draws The Display List Text
	glPopAttrib();															// Pops The Display List Bits
}

void GLUtils::guDeleteFont()												// Delete The Font List
{
	glDeleteLists( __iList, GL_COUNT_GLYPH );								// Delete All 96 Characters
}

BOOL GLUtils::guCreateWindow( LPCSTR zTitle, WNDPROC WndProc, DWORD uWidth, DWORD uHeight )
{
	GLuint		iPixelFormat;												// Holds The Results After Searching For A Match
	WNDCLASS	wc;															// Windows Class Structure
	DWORD		dwExStyle;													// Window Extended Style
	DWORD		dwStyle;													// Window Style
	RECT		WindowRect;													// Grabs Rectangle Upper Left / Lower Right Values
	LONG		lResult;

	WindowRect.left		= (long)0;											// Set Left Value To 0
	WindowRect.right	= (long)uWidth;										// Set Right Value To Requested Width
	WindowRect.top		= (long)0;											// Set Top Value To 0
	WindowRect.bottom	= (long)uHeight;									// Set Bottom Value To Requested Height

	__hInstance			= ::GetModuleHandle( NULL );						// Grab An Instance For Our Window

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;								// WndProc Handles Messages
	wc.cbClsExtra		= 0;												// No Extra Window Data
	wc.cbWndExtra		= 0;												// No Extra Window Data
	wc.hInstance		= __hInstance;										// Set The Instance
	wc.hIcon			= ::LoadIcon( NULL, IDI_WINLOGO );					// Load The Default Icon
	wc.hCursor			= ::LoadCursor( NULL, IDC_ARROW );					// Load The Arrow Pointer
	wc.hbrBackground	= NULL;												// No Background Required For GL
	wc.lpszMenuName		= NULL;												// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";											// Set The Class Name

	if ( !::RegisterClass( &wc ) )							
	{
		LOG_ERROR( "ERROR %d guCreateWindow::RegisterClass", ::GetLastError() );
		return FALSE;										
	}
	
	dwExStyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Window Extended Style
	dwStyle		= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;				// Windows Style
	

	::AdjustWindowRectEx( &WindowRect, dwStyle, FALSE, dwExStyle );			// Adjust Window To True Requested Size

	if (!( __hWnd = ::CreateWindowEx(	dwExStyle,							// Extended Style For The Window
										"OpenGL",							// Class Name
										zTitle,								// Window Title
										dwStyle |							// Defined Window Style
										WS_CLIPSIBLINGS |					// Required Window Style
										WS_CLIPCHILDREN,					// Required Window Style
										0, 0,								// Window Position
										WindowRect.right - WindowRect.left,	// Calculate Window Width
										WindowRect.bottom - WindowRect.top,	// Calculate Window Height
										NULL,								// No Parent Window
										NULL,								// No Menu
										__hInstance,						// Instance
										NULL ) ) )							// Dont Pass Anything To WM_CREATE
	{
		LOG_ERROR( "ERROR %d guCreateWindow::CreateWindowEx", ::GetLastError() );
		guDeleteWindow();
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if( !( __hDC = ::GetDC( __hWnd ) ) )						
	{
		LOG_ERROR( "ERROR %d guCreateWindow::GetDC", ::GetLastError() );
		guDeleteWindow();	
		return FALSE;								
	}

	if( !( iPixelFormat = ::ChoosePixelFormat( __hDC, &pfd ) ) )
	{
		LOG_ERROR( "ERROR %d guCreateWindow::ChoosePixelFormat", ::GetLastError() );
		guDeleteWindow();									
		return FALSE;										
	}

	if( !::SetPixelFormat( __hDC, iPixelFormat, &pfd ) )	
	{
		LOG_ERROR( "ERROR %d guCreateWindow::SetPixelFormat", ::GetLastError() );
		guDeleteWindow();							
		return FALSE;						
	}

	if ( !( __hRC = wglCreateContext( __hDC ) ) ) 
	{
		LOG_ERROR( "ERROR %d guCreateWindow::wglCreateContext", ::GetLastError() );
		guDeleteWindow();	
		return FALSE;									
	}
 
	if( !wglMakeCurrent( __hDC, __hRC ) )		
	{
		LOG_ERROR( "ERROR %d guCreateWindow::wglMakeCurrent", ::GetLastError() );
		guDeleteWindow();	
		return FALSE;							
	}

	::ShowWindow( __hWnd, SW_SHOW );							// Show The Window
	::SetForegroundWindow( __hWnd );							// Slightly Higher Priority
	::SetFocus( __hWnd );										// Sets Keyboard Focus To The Window

	return TRUE;												// Success
}

void GLUtils::guDeleteWindow()			
{
	if( __hRC )									
	{
		if( !wglMakeCurrent( NULL, NULL ) )				
		{
			LOG_ERROR( "ERROR %d guDeleteWindow::wglMakeCurrent", ::GetLastError() );
		}
		if( !wglDeleteContext( __hRC ) )			
		{
			LOG_ERROR( "ERROR %d guDeleteWindow::wglDeleteContext", ::GetLastError() );
		}
		__hRC = NULL;							
	}

	if( __hDC && !::ReleaseDC( __hWnd, __hDC ) )			
	{
		LOG_ERROR( "ERROR %d guDeleteWindow::ReleaseDC", ::GetLastError() );
		__hDC = NULL;							
	}

	if( __hWnd && !::DestroyWindow( __hWnd ) )				
	{
		LOG_ERROR( "ERROR %d guDeleteWindow::DestroyWindow", ::GetLastError() );
		__hWnd = NULL;								
	}

	if( !::UnregisterClass( "OpenGL", __hInstance ) )		
	{
		LOG_ERROR( "ERROR %d guDeleteWindow::UnregisterClass", ::GetLastError() );
		__hInstance = NULL;								
	}

	GLUtils::guDeleteFont();
}


bool GLUtils::guGetScreenSize( GLvector *pvSize )
{
	assert( pvSize );

	RECT rect;

	if( GetClientRect( __hWnd, &rect ) )
	{
		pvSize->fX = rect.right - rect.left;
		pvSize->fY = rect.bottom - rect.top;
		return true;
	}
	else
	{
		LOG_ERROR( "ERROR %d guGetScreenSize::GetClientRect", ::GetLastError() );
		return false;
	}
}

bool GLUtils::guGetCursorPos( GLvector *pvPos )
{
	POINT	cp;

	if( GetCursorPos( &cp ) )
	{	
		pvPos->fX = cp.x;
		pvPos->fY = cp.y;
		return true;
	}
	else
	{
		LOG_ERROR( "ERROR %d guGetScreenSize::GetCursorPos", ::GetLastError() );
		return false;
	}
}

void GLUtils::guDrawCircle( const GLvector& vPosition, GLuint uRadius, GLuint uRGBA )
{
	GLfloat		fAngle;
	GLuint		i;
	GLvertex	astVertexList[CIRCLE_LINE_NUMBER];

	glVertexPointer( 2, GL_FLOAT, sizeof( GLvertex ), &( astVertexList[0].fX ) );
	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( GLvertex ), &( astVertexList[0].uRGBA ) );

	for( i = 0, fAngle = 0.0; i < CIRCLE_LINE_NUMBER; i++, fAngle += MATH_KF_2_PI / CIRCLE_LINE_NUMBER )
	{
		astVertexList[i].fX =  uRadius * cos( fAngle ) + vPosition.fX;
		astVertexList[i].fY =  uRadius * sin( fAngle ) + vPosition.fY;
		astVertexList[i].uRGBA = uRGBA;
	}
	glDrawArrays( GL_TRIANGLE_FAN, 0, CIRCLE_LINE_NUMBER );
}

	
