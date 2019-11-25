#include "stdafx.h"
#include "minidump.h"

#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib") 


#ifdef _UNICODE
#define TSTRNCPY_S wcsncpy_s
#else
#define TSTRNCPY_S strncpy_s
#endif

#ifdef _UNICODE
#define TSTRRCHR wcsrchr
#else
#define TSTRRCHR strrchr
#endif

#ifdef _UNICODE
#define TSTRNCAT_S wcsncat_s
#else
#define TSTRNCAT_S strncat_s
#endif

#ifdef _UNICODE
#define TSPRINTF_S swprintf_s
#else
#define TSPRINTF_S sprintf_s
#endif

#ifdef _UNICODE
#define TSTRNLEN_S wcsnlen_s
#else
#define TSTRNLEN_S strnlen_s
#endif

#define DUMP_FILE_DIR "\\log\\" // minidump文件存放目录

const int MAX_DUMP_FILE_NUMBER = 5;

static TCHAR  g_DumpDir[MAX_PATH] /*= _T("F:\\Test\\C++_Else\\C++_Else\\Debug\\log\\")*/;

static LONG WINAPI TopLevelFilter(__in struct _EXCEPTION_POINTERS* pExceptionInfo);

// 获取当前.exe文件所在目录
DWORD GetAppDir(TCHAR* DirBuff, DWORD nSize)
{
	DWORD dwLen = 0;
	HMODULE hModule = NULL;
	TCHAR* pChar;

	if (nSize < MAX_PATH){
		return 0;
	}

	ZeroMemory(DirBuff, sizeof(TCHAR)* nSize);
	hModule = GetModuleHandle(NULL);
	dwLen = ::GetModuleFileName(hModule, DirBuff, nSize);

	//::PathRemoveFileSpec(appDir);
	pChar = TSTRRCHR(DirBuff, _T('\\'));
	if (NULL != pChar){
		*pChar = _T('\0');
	}

	return pChar - DirBuff;
}


static DWORD InitDumpDir(TCHAR* DirBuff, DWORD nSize)
{
	DWORD dwLen = 0;
	errno_t err = 0;
	
	dwLen = GetAppDir(DirBuff, nSize); // 获取当前.exe文件所在目录
	if (0 == dwLen){
		return dwLen;
	}

	err = TSTRNCAT_S(DirBuff, nSize, _T(DUMP_FILE_DIR), _TRUNCATE);	// 在.exe文件所在目录后追加 "//log"
	if (STRUNCATE == err){
		DirBuff[nSize - 1] = 0;
	}

	return dwLen + sizeof(DUMP_FILE_DIR);
}

static void GetDumpDir(TCHAR* DirBuff, DWORD nSize)
{
	TSTRNCPY_S(DirBuff, nSize, g_DumpDir, _TRUNCATE);
}

static void GetDumpFilePath(TCHAR* DumpFilePath, DWORD nSize)
{
	size_t PathLen = 0;
	SYSTEMTIME SystemTime;

	// 取dump文件目录
	ZeroMemory(DumpFilePath, sizeof(TCHAR)* nSize);
	GetDumpDir(DumpFilePath, nSize);
	PathLen = TSTRNLEN_S(DumpFilePath, nSize);

	// 生成dump文件名 "MagicBox-年-月-日-时-分-秒-毫秒.dmp"
	::GetLocalTime(&SystemTime);
	TSPRINTF_S(DumpFilePath + PathLen, nSize - PathLen, _T("MagicBox-%4d-%02d-%02d-%02d-%02d-%02d-%03d.dmp"),
		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
}

static BOOL WriteDumpExceptionInfo(TCHAR* pDumpFilePath, PEXCEPTION_POINTERS pExceptionInfo)
{
	HANDLE hFile = ::CreateFile(pDumpFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION einfo;
		einfo.ThreadId = ::GetCurrentThreadId();
		einfo.ExceptionPointers = pExceptionInfo;
		einfo.ClientPointers = FALSE;

		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &einfo, NULL, NULL);
		::CloseHandle(hFile);

		return TRUE;
	}

	return FALSE;
}

static LONG WINAPI TopLevelFilter(__in struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	TCHAR DumpFilePath[MAX_PATH];

	// 取dump文件路径
	GetDumpFilePath( DumpFilePath, sizeof(DumpFilePath) / sizeof(TCHAR) );

	if (WriteDumpExceptionInfo(DumpFilePath, pExceptionInfo)){
		return EXCEPTION_EXECUTE_HANDLER;
	} else {
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

void InitExceptionHandler()
{
	InitDumpDir(g_DumpDir, sizeof(g_DumpDir) / sizeof(TCHAR));

    ::SetUnhandledExceptionFilter(TopLevelFilter);  //设置异常捕获函数.当异常没有处理的时候, 系统就会调用SetUnhandledExceptionFilter所设置异常处理函数.
}
