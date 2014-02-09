// TSWhatToDo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TSWhatToDo.h"
#include "LuaFunctions.h"


// Global Variables:
HWND g_hDlg = NULL;
HHOOK g_hKeyHook = NULL;
HHOOK g_hMouseHook = NULL;
BOOL g_bUpdate = FALSE;
HDC g_hDc = NULL;


// Forward declarations of functions included in this code module:
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);

VOID UpdateDlgWinTxt(DWORD dwId, HWND hWnd);
VOID UpdateDlgColor();


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);

 	// TODO: Place code here.

    TCHAR szFile[MAX_PATH] = {};
    int len = lstrlen(lpCmdLine);
    if (len > 2)
    {
        lstrcpy(szFile, lpCmdLine + 1);
        szFile[len - 2] = 0;
    }

    if (_access(szFile, 0) == 0)
    {
        CLuaSe oLua;
        oLua.Init();
        ::RegisterFunctions(oLua);
        CStr err(128);
        bool res = oLua.RunFile(szFile, &err);
        if (res == false)
        {
            MessageBox(HWND_DESKTOP, err.GetBuffer(), TEXT("TSWhatToDo"), MB_ICONERROR);
        }
        
        return 0;
    }
#if 1
    HHOOK g_hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyHookProc, hInstance, 0);
    if (g_hKeyHook == NULL)
    {
        return 0;
    }

    HHOOK g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInstance, 0);
    if (g_hMouseHook == NULL)
    {
        return 0;
    }
#endif
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_HELPER), NULL, DlgProc);

#if 0
    MSG msg;
    HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TSWHATTODO));

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
#else
    return 0;
#endif
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        
        g_hDlg = hDlg;

        SetDlgItemText(hDlg, IDC_KEY_INFO, TEXT("<VKCode, ScanCode> (Press [PauseBreak] to stop/start)"));
        SetDlgItemText(hDlg, IDC_MOUSE_INFO, TEXT("<CursorPosition>"));
        SetDlgItemText(hDlg, IDC_COLOR_INFO, TEXT("<CursorColor>"));
        SetDlgItemText(hDlg, IDC_WIN_TXT, TEXT("<WindowText>"));
        SetDlgItemText(hDlg, IDC_TOP_WIN_TXT, TEXT("<TopLevelWindowText>"));

        g_hDc = GetDC(HWND_DESKTOP);

        g_bUpdate = TRUE;

        return (INT_PTR)TRUE;

    case WM_COMMAND:

        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
    {
        PKBDLLHOOKSTRUCT pKeyInfo = (PKBDLLHOOKSTRUCT)lParam;

        if (pKeyInfo->vkCode == VK_PAUSE)
        {
            TCHAR szColor[256] = {};
            if (g_bUpdate)
            {
                UpdateDlgColor();
            }

            g_bUpdate = !g_bUpdate;
            SetWindowText(g_hDlg, g_bUpdate ? TEXT("TSWhatToDo") : TEXT("TSWhatToDo - Pause"));
            
        }

        if (g_bUpdate)
        {
            TCHAR szKeyInfo[128] = {};
            wsprintf(szKeyInfo, "%u, %u%s", pKeyInfo->vkCode, pKeyInfo->scanCode, (pKeyInfo->flags & LLKHF_EXTENDED) ? "x" : "");
            SetDlgItemText(g_hDlg, IDC_KEY_INFO, szKeyInfo);
        }
    }

    return CallNextHookEx(g_hKeyHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && g_bUpdate)
    {
        if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_MBUTTONDOWN)
        {
            UpdateDlgColor();
        }

        TCHAR szMouseInfo[128] = {};
        
        PMSLLHOOKSTRUCT pMouseInfo = (PMSLLHOOKSTRUCT)lParam;
        wsprintf(szMouseInfo, "%ld, %ld", pMouseInfo->pt.x, pMouseInfo->pt.y);
        
        SetDlgItemText(g_hDlg, IDC_MOUSE_INFO, szMouseInfo);

        TCHAR szWinTxt[256] = {};
        
        HWND hWnd = WindowFromPoint(pMouseInfo->pt);
        UpdateDlgWinTxt(IDC_WIN_TXT, hWnd);

        HWND hTopWnd = NULL;
        while (hWnd)
        {
            hTopWnd = hWnd;
            hWnd = GetParent(hWnd);
        }

        UpdateDlgWinTxt(IDC_TOP_WIN_TXT, hTopWnd);
        
    }

    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

VOID UpdateDlgWinTxt(DWORD dwId, HWND hWnd)
{
    TCHAR szWinTxt[256] = {};
    if (hWnd)
    {
        GetWindowText(hWnd, szWinTxt, sizeof(szWinTxt));
    }

    TCHAR szWinTxtOld[256] = {};
    GetDlgItemText(g_hDlg, dwId, szWinTxtOld, sizeof(szWinTxtOld));
    if (lstrcmp(szWinTxtOld, szWinTxt) != 0)
    {
        SetDlgItemText(g_hDlg, dwId, szWinTxt);
    }
}

VOID UpdateDlgColor()
{
    HDC dc = GetDC(HWND_DESKTOP);
    POINT pt = {};
    GetCursorPos(&pt);
    COLORREF color = GetPixel(dc, pt.x, pt.y);
    ReleaseDC(HWND_DESKTOP, dc);
    TCHAR szColor[256] = {};
    wsprintf(szColor, TEXT("%08X, RGB(%u, %u, %u)"), color, (UINT)GetRValue(color), (UINT)GetGValue(color), (UINT)GetBValue(color));
    SetDlgItemText(g_hDlg, IDC_COLOR_INFO, szColor);
}

DWORD GetPidByName(PCTSTR pName){    DWORD pid = 0;    PROCESSENTRY32 pe = {sizeof(PROCESSENTRY32)};    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);        for (bool res = Process32First(hSnapshot, &pe); res; res = Process32Next(hSnapshot, &pe))    {        if(lstrcmpi(pe.szExeFile, pName) == 0)        {            pid = pe.th32ProcessID;            break;        }    }    CloseHandle(hSnapshot);    return pid;}
