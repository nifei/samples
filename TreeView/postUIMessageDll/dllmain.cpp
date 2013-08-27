// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "MainFrm.h"
#include "UIThreadDll.h"

CAppModule   _Module;

//一个隐藏的后台窗口, 创建的目的是得到操作UI线程事件队列的方法
CMainFrame* _wndMain = 0;

void PostMessageToUIThread( void *userData, MainThreadCallbackFun ptrFun)
{
	if (_wndMain)
	{
		WPARAM wParam = (WPARAM)userData;
		LPARAM	 lParam = (LPARAM)ptrFun;
		_wndMain->PostMessage(WM_MY_MESSAGE, wParam, lParam);
	}
}

//! Run和MyMain是从wtl工程中的WndMain&Run中拷贝过来的, 在DllMain中调用, 启动窗口事件循环
int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop *theLoop = _Module.GetMessageLoop();

	if (!theLoop)
	{
		theLoop = new CMessageLoop();
		_Module.AddMessageLoop(theLoop);
	}
	
	_wndMain = new CMainFrame();

	if(_wndMain->CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	theLoop->AddMessageFilter(_wndMain);
	theLoop->AddIdleHandler(_wndMain);
	_wndMain->ShowWindow(SW_HIDE );

	return 0;
}

BOOL MyMain(HINSTANCE hInstance, LPVOID lpReserved)
{	
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run();

	return TRUE;
}

void GetUIThread(HINSTANCE hInstance, LPVOID lpReserved)
{
	if (!_wndMain)
		MyMain(hInstance, lpReserved);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_ATTACH:
		GetUIThread(HINSTANCE(hModule), lpReserved);
		break;
	case DLL_THREAD_DETACH:
		if (_wndMain)
		{
			delete _wndMain;
			_wndMain = 0;
			_Module.RemoveMessageLoop();
			_Module.Term();
			::CoUninitialize();
		}
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

