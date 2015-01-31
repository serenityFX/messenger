// messanger.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "messanger.h"
#include "../../../sciter/include/sciter-x.h"

int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow )
{
	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_MESSANGER ) );

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



