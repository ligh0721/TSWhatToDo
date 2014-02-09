#include "stdafx.h"
#include "LuaFunctions.h"
#include "TSWhatToDo.h"


struct GLOBAL_CONFIG
{
    GLOBAL_CONFIG()
        : dwOpDelay(100)
    {
    }

    DWORD dwOpDelay;
};

inline UINT WINAPI DelaySendInput(DWORD dwDelayMSec, UINT cInputs, LPINPUT pInputs, int cbSize)
{
    Sleep(dwDelayMSec);
    return SendInput(cInputs, pInputs, cbSize);
}

inline BOOL IsExtenedKey(WORD vkCode)
{
    switch(vkCode)
    {
    case VK_RSHIFT:
    case VK_RCONTROL:
    case VK_RMENU:

    case VK_LWIN:
    case VK_RWIN:
    case VK_APPS:

    case VK_SNAPSHOT:
    case VK_INSERT:
    case VK_DELETE:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_LEFT:
    case VK_UP:
    case VK_RIGHT:
    case VK_DOWN:

    case VK_NUMLOCK:
    case VK_DIVIDE:
    //case VK_RETURN: NUMPAD's
        return TRUE;

    default:
        break;
    }

    return FALSE;
}

GLOBAL_CONFIG g_cfg;

DEF_LUACFUNC(Delay)
{
    DEF_LUAOBJ(CLuaSe, l);

    DWORD msec = l.ToUnsigned(1);
    Sleep(msec);

    return 0;
}

DEF_LUACFUNC(KeyPress)
{
    DEF_LUAOBJ(CLuaSe, l);

    WORD vkCode = l.ToUnsigned(1);

    INPUT stIn = {};
    
    stIn.type = INPUT_KEYBOARD;
    stIn.ki.wVk = vkCode;
    stIn.ki.wScan = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
    stIn.ki.dwFlags = IsExtenedKey(vkCode) ? KEYEVENTF_EXTENDEDKEY : 0;
    stIn.ki.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));
    
    stIn.ki.dwFlags |= KEYEVENTF_KEYUP;
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(KeyDown)
{
    DEF_LUAOBJ(CLuaSe, l);

    WORD vkCode = l.ToUnsigned(1);
    
    INPUT stIn = {};

    stIn.type = INPUT_KEYBOARD;
    stIn.ki.wVk = vkCode;
    stIn.ki.wScan = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
    stIn.ki.dwFlags = IsExtenedKey(vkCode) ? KEYEVENTF_EXTENDEDKEY : 0;
    stIn.ki.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(KeyUp)
{
    DEF_LUAOBJ(CLuaSe, l);

    WORD vkCode = l.ToUnsigned(1);

    INPUT stIn = {};

    stIn.type = INPUT_KEYBOARD;
    stIn.ki.wVk = vkCode;
    stIn.ki.wScan = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
    stIn.ki.dwFlags = (IsExtenedKey(vkCode) ? KEYEVENTF_EXTENDEDKEY : 0) | KEYEVENTF_KEYUP;
    stIn.ki.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(MoveTo)
{
    DEF_LUAOBJ(CLuaSe, l);

    double x = l.ToNumber(1);
    double y = l.ToNumber(2);

    const double CONST_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
    const double CONST_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);

    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dx = (LONG)(0xFFFF / (CONST_CXSCREEN - 1) * x);
    stIn.mi.dy = (LONG)(0xFFFF / (CONST_CYSCREEN - 1) * y);
    stIn.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE/* | g_dwLFlags*/;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(LClick)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));
    
    stIn.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(LDClick)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));
    
    stIn.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    stIn.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    stIn.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));


    return 0;
}

DEF_LUACFUNC(LDown)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(LUp)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(RClick)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    stIn.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(RDown)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(RUp)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(MClick)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    stIn.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(MDown)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(MUp)
{
    INPUT stIn = {};

    stIn.type = INPUT_MOUSE;
    stIn.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    stIn.mi.dwExtraInfo = GetMessageExtraInfo();
    DelaySendInput(g_cfg.dwOpDelay, 1, &stIn, sizeof(INPUT));

    return 0;
}

DEF_LUACFUNC(FindProcess)
{
    DEF_LUAOBJ(CLuaSe, l);
    
    const char* pName = l.ToString(1);
    DWORD pid = GetPidByName(pName);
    if (pid != 0)
    {
        l.PushUnsigned(pid);
    }
    else
    {
        l.PushNil();
    }

    return 1;
}

DEF_LUACFUNC(KillProcess)
{
    DEF_LUAOBJ(CLuaSe, l);

    DWORD pid = l.ToUnsigned(1);
    HANDLE proc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (proc != NULL)
    {
        TerminateProcess(proc, 0);
        CloseHandle(proc);
    }
    
    return 0;
}

DEF_LUACFUNC(PixelColor)
{
    DEF_LUAOBJ(CLuaSe, l);
    
    POINT pt = {};
    if (l.GetTopPos() == 2)
    {
        pt.x = (LONG)l.ToNumber(1);
        pt.y = (LONG)l.ToNumber(2);
    }
    else
    {
        GetCursorPos(&pt);
    }
    
    HDC dc = GetDC(HWND_DESKTOP);
    COLORREF color = GetPixel(dc, pt.x, pt.y);
    ReleaseDC(HWND_DESKTOP, dc);

    l.PushUnsigned(color);

    return 1;
}

DEF_LUACFUNC(RGB_)
{
    DEF_LUAOBJ(CLuaSe, l);

    BYTE r = l.ToUnsigned(1);
    BYTE g = l.ToUnsigned(2);
    BYTE b = l.ToUnsigned(3);

    COLORREF color = RGB(r, g, b);

    l.PushUnsigned(color);

    return 1;
}

DEF_LUACFUNC(UnRGB)
{
    DEF_LUAOBJ(CLuaSe, l);

    COLORREF color = l.ToUnsigned(1);
    
    l.PushUnsigned(GetRValue(color));
    l.PushUnsigned(GetGValue(color));
    l.PushUnsigned(GetBValue(color));

    return 3;
}

DEF_LUACFUNC(MsgBox)
{
    DEF_LUAOBJ(CLuaSe, l);

    int c = l.GetTopPos();

    const char* pText = l.ToString(1);
    const char* pTitle = (c >= 2 ? l.ToString(2) : "TSWhatToDo");
    UINT uType = (c == 3 ? l.ToUnsigned(3) : MB_OK);

    MessageBox(HWND_DESKTOP, pText, pTitle, uType);

    return 0;
}

DEF_LUACFUNC(FindWindow_)
{
    DEF_LUAOBJ(CLuaSe, l);

    HWND hWnd = FindWindow(NULL, l.ToString(1));

    if (hWnd != NULL)
    {
        l.PushUnsigned((lua_Unsigned)hWnd);
    }
    else
    {
        l.PushNil();
    }

    return 1;
}

DEF_LUACFUNC(MoveWindow_)
{
    DEF_LUAOBJ(CLuaSe, l);

    HWND hWnd = (HWND)l.ToUnsigned(1);
    int x = (int)l.ToInteger(2);
    int y = (int)l.ToInteger(3);

    SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
    
    return 0;
}

DEF_LUACFUNC(WindowPos)
{
    DEF_LUAOBJ(CLuaSe, l);

    HWND hWnd = (HWND)l.ToUnsigned(1);
    RECT rect = {};
    GetWindowRect(hWnd, &rect);

    l.PushInteger(rect.left);
    l.PushInteger(rect.top);

    return 2;
}

DEF_LUACFUNC(Log)
{
    DEF_LUAOBJ(CLuaSe, l);

    OutputDebugString(l.ToString(1));

    return 0;
}

int RegisterFunctions(CLuaSe& l)
{
#define REG_FUNC(func) l.RegisterCFunction(#func, func);
    
    REG_FUNC(Delay);

    REG_FUNC(KeyPress);
    REG_FUNC(KeyDown);
    REG_FUNC(KeyUp);

    REG_FUNC(MoveTo);
    REG_FUNC(LClick);
    REG_FUNC(LDClick);
    REG_FUNC(LDown);
    REG_FUNC(LUp);
    REG_FUNC(RClick);
    REG_FUNC(RDown);
    REG_FUNC(RUp);
    REG_FUNC(MClick);
    REG_FUNC(MDown);
    REG_FUNC(MUp);

    REG_FUNC(FindProcess);
    REG_FUNC(KillProcess)
    REG_FUNC(PixelColor);
    l.RegisterCFunction("RGB", RGB_);
    REG_FUNC(UnRGB);
    REG_FUNC(MsgBox);
    l.RegisterCFunction("FindWindow", FindWindow_);
    l.RegisterCFunction("MoveWindow", MoveWindow_);
    REG_FUNC(WindowPos);
    REG_FUNC(Log);

    return 0;
}


