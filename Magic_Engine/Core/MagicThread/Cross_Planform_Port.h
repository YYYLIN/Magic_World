#ifndef _CROSS_PLANTFORM_PORT_H_
#define _CROSS_PLANTFORM_PORT_H_

#include <time.h>
#include <stdio.h>

#ifdef WIN32
#include <process.h>
#include <windows.h>
#include <io.h>
#else
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#endif

#ifdef WIN32
#define Magic_Sprintf_s sprintf_s
#define Magic_CLOCK()	clock()

/*
*功能：创建线程
*参数：
*	f = 线程对象指针
*	b = 默认空（平台特性）
*	c = 线程函数指针
*	d = 线程参数
*返回值：如果等于非零值成功
*/
#define Magic_Thread_Create(f,b,c,d)				f = CreateThread(NULL, b, c, d, 0, NULL)
#define Magic_ResumeThread(a)						ResumeThread(a)
#define Magic_CloseHandle(a)						CloseHandle(a)
#define Magic_Thread_Wait(a)						WaitForSingleObject(a, INFINITE)
#define Magic_Thread_Exit(a)						TerminateThread(a, 0)

/*
*功能：创建共享消息
*参数：
*	a = 对象指针
*	b = Win32平台参数
*	c = Win32平台参数
*	d = Win32平台参数最大消息次数
*	e = Win32平台参数共享名字
*	f = Linux平台参数
*	g = Linux平台参数
*/
#define Magic_Thread_SEM_init(a,b,c,d,e,f,g)		a = CreateSemaphore(b,c,d,e)
#define Magic_Thread_SEM_Wait(a)					WaitForSingleObject(a, INFINITE)
#define Magic_Thread_SEM_Wait_Time(a,t)				WaitForSingleObject(a, t);
#define Magic_Thread_SEM_Post(a)					ReleaseSemaphore(a, 1, NULL)
#define Magic_Thread_SEM_destroy(a)					CloseHandle(a)
#define Magic_Thread_Mutex_Lock(a)					EnterCriticalSection(a)
#define Magic_Thread_Mutex_unLock(a)				LeaveCriticalSection(a)
#define Magic_Thread_Mutex_Init(a)					InitializeCriticalSection(a)
#define Magic_Thread_Mutex_Destroy(a)				DeleteCriticalSection(a)
#define arcoss										DWORD WINAPI
#define arcoss_return(a)							((DWORD)(a))
typedef HANDLE										Magic_THREAD;
typedef HANDLE										Magic_SEM;
typedef HANDLE										Magic_SHARED;
typedef CRITICAL_SECTION							Magic_MUTEX;
typedef int											socklen_t;

typedef unsigned int								Magic_SOCKSET;

#define Magic_Sleep(a)								Sleep(a * 1000)
#define Magic_MSleep(a)								Sleep(a)
#define Magic_ProgressID							_getpid
#define Magic_CloseSocket(a)						closesocket(a)
#define Magic_File_Open(a, b, c)					fopen_s(&a, b, c)
#define Magic_File_Read(a, b, c, d)					fread(a, b, c, d)
#define Magic_File_Write(a, b, c, d)				fwrite(a, b, c, d)
#define Magic_File_Close(a)							fclose(a)
#define Magic_File_Access(a, b)						_access(a, b)
#define Magic_File_CreateDirectory(a, b)			_mkdir(a)

#define DLL_OPEN(a)									LoadLibraryA(a)
#define DLL_CLOSE(a)								FreeLibrary(a)
#define DLL_GetFunction(a, b)						GetProcAddress(a, b)
#define DLL_HANDLE									HINSTANCE

#define S_THREAD									__declspec(thread)

#else
#define Magic_Sprintf_s snprintf

#define Magic_Thread_Create(f,b,c,d)				(pthread_create(&f,b,c,d) == 0)
#define Magic_ResumeThread(a)
#define Magic_CloseHandle(a)
#define Magic_Thread_Wait(a)						pthread_join(a, NULL)
#define Magic_Thread_Exit(a)						pthread_cancel(a)
#define Magic_Thread_SEM_init(a,b,c,d,e,f,g)		(sem_init(&a,f,g) == 0)
#define Magic_Thread_SEM_Wait(a)					sem_wait(&a)
#define Magic_Thread_SEM_Post(a)					sem_post(&a)
#define Magic_Thread_SEM_destroy					sem_destroy(&a)
#define Magic_Thread_Mutex_Lock(a)					pthread_mutex_lock(a)
#define Magic_Thread_Mutex_unLock(a)				pthread_mutex_unlock(a)
#define Magic_Thread_Mutex_Init(a)					pthread_mutex_init(a, NULL)
#define Magic_Thread_Mutex_Destroy(a)				pthread_mutex_destroy(a)

#define arcoss										void*
#define arcoss_return								((void*)(a))
typedef pthread_t									Magic_THREAD;
typedef	sem_t										Magic_SEM;
typedef pthread_mutex_t								Magic_MUTEX;
typedef int											Magic_SOCKSET;
typedef int											Magic_SHARED;
#define Magic_Sleep(a)								sleep(a)
#define Magic_MSleep(a)								usleep(a * 1000)
#define Magic_ProgressID							getpid

/*文件操作*/
#define Magic_CloseSocket(a)						close(a)
#define Magic_File_Open(a, b, c)					a = fopen(b, c)
#define Magic_File_Read(a, b, c, d)					fread(a, b, c, d)
#define Magic_File_Write(a, b, c, d)				fwrite(a, b, c, d)
#define Magic_File_Close(a)							fclose(a)
#define Magic_File_Access(a, b)						access(a, b)
#define Magic_File_CreateDirectory(a, b)			mkdir(a, b)

#define DLL_OPEN(a)									dlopen(a, RTLD_LAZY)
#define DLL_CLOSE(a)								dlclose(a)
#define DLL_GetFunction(a, b)						dlsym(a, b)
#define DLL_HANDLE									void*

#define S_THREAD									__thread

#endif


#endif