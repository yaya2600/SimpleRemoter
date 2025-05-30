// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__46CA6496_AAD6_4658_B6E9_D7AEB26CDCD5__INCLUDED_)
#define AFX_STDAFX_H__46CA6496_AAD6_4658_B6E9_D7AEB26CDCD5__INCLUDED_

// 是否使用ZLIB
#define USING_ZLIB 0

#if !USING_ZLIB
// 是否使用LZ4
#define USING_LZ4 0

#if !USING_LZ4
#define USING_ZSTD 1
#define USING_CTX 1
#endif

#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _DEBUG
#define Mprintf(format, ...) printf(format, ##__VA_ARGS__)
#else
#define Mprintf(format, ...) 
#endif

// 检测内存泄漏，需安装VLD；否则请注释此行
#include "vld.h"

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__46CA6496_AAD6_4658_B6E9_D7AEB26CDCD5__INCLUDED_)

#include <assert.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

// 高精度的睡眠函数
#define Sleep_m(ms) { timeBeginPeriod(1); Sleep(ms); timeEndPeriod(1); }

// 以步长n毫秒在条件C下等待T秒(n是步长，必须能整除1000)
#define WAIT_n(C, T, n) {assert(!(1000%(n)));int s=(1000*(T))/(n);do{Sleep(n);}while((C)&&(--s));}

// 在条件C成立时等待T秒(步长10ms)
#define WAIT(C, T) { timeBeginPeriod(1); WAIT_n(C, T, 10); timeEndPeriod(1); }

// 在条件C成立时等待T秒(步长1ms)
#define WAIT_1(C, T) { timeBeginPeriod(1); WAIT_n(C, T, 1); timeEndPeriod(1); }

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// 智能计时器，计算函数的耗时
class auto_tick
{
private:
	const char *func;
	int span;
	clock_t tick;
	__inline clock_t now() const { return clock(); }
	__inline int time() const { return now() - tick; }

public:
	auto_tick(const char *func_name, int th = 5) : func(func_name), span(th), tick(now()) { }
	~auto_tick() { stop(); }

	__inline void stop() {
		if (span != 0) { int s(this->time()); if (s > span)Mprintf("[%s]执行时间: [%d]ms.\n", func, s); span = 0; }
	}
};

#ifdef _DEBUG
// 智能计算当前函数的耗时，超时会打印
#define AUTO_TICK(thresh) auto_tick TICK(__FUNCTION__, thresh)
#define STOP_TICK TICK.stop()
#else
#define AUTO_TICK(thresh) 
#define STOP_TICK
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if(NULL !=(p)){ delete (p);(p) = NULL;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if(NULL !=(p)){ delete[] (p);(p) = NULL;}
#endif

#ifndef SAFE_DELETE_AR
#define SAFE_DELETE_AR(p) if(NULL !=(p)){ delete[] (p);(p) = NULL;}
#endif

class CLock
{
private:
	CRITICAL_SECTION m_cs;
public:
	CLock()
	{
		InitializeCriticalSection(&m_cs);
	}
	~CLock()
	{
		DeleteCriticalSection(&m_cs);
	}

	void Unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}
};

class CAutoLock
{
private:
	CRITICAL_SECTION &m_cs;
public:
	CAutoLock(CRITICAL_SECTION& cs) : m_cs(cs)
	{
		EnterCriticalSection(&m_cs);
	}
	~CAutoLock()
	{
		LeaveCriticalSection(&m_cs);
	}
};
