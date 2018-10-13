#include <windows.h>
#include "stdafx.h"
#include "resource.h"


// LOWORD  OBJETOS     y     HIWORD  EVENTOS

BOOL CALLBACK miFuncion(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK miOtraFuncion(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE miID;
HWND hVentanaPrincipal;
HWND hVentanaSecundaria;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev,
	PSTR cmdLine, int showWin) {
	miID = hInst;
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	hVentanaPrincipal = CreateDialog( // DIALOGOS NO MODALES
		hInst,
		MAKEINTRESOURCE(IDD_DIALOG1),
		0,
		miFuncion
	);
	ShowWindow(hVentanaPrincipal, SW_SHOW);

	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}


BOOL CALLBACK miFuncion(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:

		break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == ID_LOGIN_LOG && HIWORD(wParam) == BN_CLICKED) {

			//hWnd: Handler el cual contiene el control que se desea obtener.
			// ID_LOGIN_LOG: Identificador del control que se desea obtener. 
			HWND hLoginLogBtn = GetDlgItem(hWnd, ID_LOGIN_LOG);

			SetWindowText(hLoginLogBtn, "Presionado");

		}
		else if (LOWORD(wParam) == ID_LOGIN_CLEAN && HIWORD(wParam) == BN_CLICKED) {
			//MessageBox(NULL, "Alert", "Click a CANCEL", MB_OK);

			 DialogBox( // DIALOGOS MODALES
				miID,
				MAKEINTRESOURCE(IDD_DIALOG2),
				0,
				miOtraFuncion
			);
		//	ShowWindow(hVentanaSecundaria, SW_SHOW);

		}
	}
					 break;
	case WM_CLOSE:
		DestroyWindow(hWnd); //DESTRUYE DIALOGOS O VENTANAS NO MODALES
		break;
	case WM_DESTROY:
		PostQuitMessage(897);
		break;
	}
	return FALSE;
}

BOOL CALLBACK miOtraFuncion(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
		break;
	case WM_CLOSE:
		EndDialog(hWnd, 1);
		break;
	}
	return FALSE;
}