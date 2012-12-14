#ifndef LOCKER_H_
#define LOCKER_H_

#if defined _WINDOWS

#include <windows.h>

class auto_lock
{
public:
    auto_lock()	{ InitializeCriticalSection( &__lock ); }
    ~auto_lock() { DeleteCriticalSection( &__lock ); }
    operator LPCRITICAL_SECTION() { return &__lock; }
private:
    CRITICAL_SECTION __lock;
};

class auto_locker
{
public:
    auto_locker( LPCRITICAL_SECTION lock ) : __lock( lock ) { EnterCriticalSection( __lock ); }
    ~auto_locker() { LeaveCriticalSection( __lock ); }
private:
    LPCRITICAL_SECTION __lock;
};
#else
#if defined __linux__ || defined ANDROID

#include <pthread.h>

class auto_lock
{
public:
    auto_lock() { __lock = PTHREAD_MUTEX_INITIALIZER; }
	~auto_lock() {}
    operator pthread_mutex_t&() { return __lock; }
private:
    pthread_mutex_t __lock;
};

class auto_locker
{
public:
    auto_locker( pthread_mutex_t &lock ) : __lock( lock ) { pthread_mutex_lock( &__lock ); }
    ~auto_locker() { pthread_mutex_unlock( &__lock ); }
private:
    pthread_mutex_t __lock;
};
#endif // LINUX || ANDROID
#endif // WINDOWS

#endif
