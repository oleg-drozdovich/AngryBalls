#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Locker.h"

#ifdef ANDROID
	#include <android/log.h>
#endif

#define LL_NONE     0
#define LL_FATAL    1
#define LL_ERROR    2
#define LL_WARNING  3
#define LL_MESSAGE  4
#define LL_INFO     5
#define LL_TRACE    6
#define LL_DEBUG    7

#ifndef LOG_LEVEL
#define LOG_LEVEL LL_DEBUG
#endif

#define BUFFER_SIZE		0x200
#define MAX_NAME		0xFF
#define ANDROID_TAG		""

class Logger
{
public:
	static Logger& Instance()
	{
		static Logger logger;
		return logger;
	}
	const char* GetFileName() const { return __zFileName; }
	int GetLevel() const { return __LogLevel; }
	void SetLevel( int LogLevel ) { __LogLevel = LogLevel; }
	void SetFileName( const char* zFileName ){ strcpy( __zFileName, zFileName ); }
	void Write( const char* zFormat, ... )
	{
		auto_locker	locker( __lock );

		va_list	args;

		va_start( args, zFormat );
		WriteFormat( zFormat, args );
		va_end( args );
	}

private:
	Logger()
	{
		SetFileName( "default.log" );
		__LogLevel = LOG_LEVEL;
	}

	void WriteFormat( const char* zFormat, va_list args )
	{
		time_t	stTime;
		tm		stLocalTime;
		char	zBuffer[BUFFER_SIZE];
		int		length;
		FILE*	pstFile;

		stTime = time( NULL );
		stLocalTime = *localtime( &stTime );
		length = sprintf( zBuffer, "%02d.%02d.%04d %02d:%02d:%02d\t",	stLocalTime.tm_mday,
																		stLocalTime.tm_mon,
																		stLocalTime.tm_year,
																		stLocalTime.tm_hour,
																		stLocalTime.tm_min,
																		stLocalTime.tm_sec );
		length += vsprintf( zBuffer + length, zFormat, args );
		strcat( zBuffer, "\r\n" );
		length += 2;
		pstFile = fopen( __zFileName, "a" );
		if( pstFile )
		{
			fwrite( zBuffer, 1, length, pstFile );
			fclose( pstFile );
		}

		#ifdef ANDROID
			__android_log_print( ANDROID_LOG_INFO, ANDROID_TAG, zBuffer, args );
		#else
			vprintf( zBuffer, args );
		#endif
	}
private:
	char		__zFileName[MAX_NAME];	// имя файла лога
    int			__LogLevel;				// динамичеcки управляемый уровень логов
    auto_lock	__lock;					// синхронизирующий объект
};

#if LOG_LEVEL > LL_NONE
	#define LOG_SETFILENAME(x) Logger::Instance().SetFileName( x )
	#define LOG_GETFILENAME() Logger::Instance().GetFileName()
	#define LOG_SETLEVEL(x) Logger::Instance().SetLevel( x )
#else
	#define LOG_SETFILENAME(x)
	#define LOG_GETFILENAME() ""
	#define LOG_SETLEVEL(x)
#endif

#if LOG_LEVEL >= LL_NONE
	#define LOG_ALWAYS( x, ... ) if( Logger::Instance().GetLevel() >= LL_NONE ) Logger::Instance().Write( x, ##__VA_ARGS__ )
#else
	#define LOG_ALWAYS( x, ... )
#endif

#if LOG_LEVEL >= LL_FATAL
	#define LOG_FATAL( x, ... ) if( Logger::Instance().GetLevel() >= LL_FATAL ) Logger::Instance().Write( x, ##__VA_ARGS__ )
#else
	#define LOG_FATAL( x, ... )
#endif

#if LOG_LEVEL >= LL_ERROR
	#define LOG_ERROR( x, ... ) if( Logger::Instance().GetLevel() >= LL_ERROR ) Logger::Instance().Write( x, ##__VA_ARGS__ )
#else
	#define LOG_ERROR( x, ... )
#endif

#if LOG_LEVEL >= LL_WARNING
	#define LOG_WARNING( x, ... ) if( Logger::Instance().GetLevel() >= LL_WARNING ) Logger::Instance().Write( x, ##__VA_ARGS__ )
#else
	#define LOG_WARNING( x, ... )
#endif

#if LOG_LEVEL >= LL_MESSAGE
	#define LOG_MESSAGE( x, ... ) if( Logger::Instance().GetLevel() >= LL_MESSAGE ) Logger::Instance().Write( x, ##__VA_ARGS__ )
#else
    #define LOG_MESSAGE( x, ... )
#endif


#if LOG_LEVEL >= LL_INFO
	#define LOG_INFO( x, ... ) if( Logger::Instance().GetLevel() >= LL_INFO ) Logger::Instance().Write( x, ##__VA_ARGS__ )
#else
	#define LOG_INFO( x, ... )
#endif


#if LOG_LEVEL >= LL_TRACE
	#define LOG_TRACE( x, ... ) if( Logger::Instance().GetLevel() >= LL_TRACE ) Logger::Instance().Write( x, ##__VA_ARGS__ )
#else
	#define LOG_TRACE( x, ... )
#endif


#if LOG_LEVEL >= LL_DEBUG
	#define LOG_DEBUG( x, ... ) if( Logger::Instance().GetLevel() >= LL_DEBUG ) Logger::Instance().Write( x, ##__VA_ARGS__ )
#else
	#define LOG_DEBUG( x, ... )
#endif


#endif /* LOGGER_H_ */
