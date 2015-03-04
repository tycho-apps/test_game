//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 30 March 2008 8:07:01 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "engine/engine.h"
#include "core/console.h"
#include "core/core.h"
#include "core/clock.h"
#include "math/utilities.h"
#include "../test_game.h"
#include "core/pc/safe_windows.h"

using namespace tycho::core;

//////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

int  platform_run();
void platform_cleanup();
void platform_show_window();
void animate_window(HWND hwnd, const RECT &rect, float target_width, float target_height, uint8 start_alpha, uint8 end_alpha);
void enable_window_layering(HWND hwnd, bool);
void set_window_alpha(HWND hwnd, BYTE alpha);
void close_window();

//////////////////////////////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////////////////////////////

static HINSTANCE g_instance;
HWND			 g_hwnd;
test_game::game  g_game;
static bool		 g_quitting = false;


//////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void dock_console_window()
{
	if(g_hwnd)
	{
		RECT win_rect, console_rect;
		::GetWindowRect(g_hwnd, &win_rect);				
		::GetWindowRect(::GetConsoleWindow(), &console_rect);
		
		int x = win_rect.left;
		int y = win_rect.bottom;
		int w = win_rect.right - win_rect.left;
		int h = console_rect.bottom - console_rect.top;
		if(!::MoveWindow(::GetConsoleWindow(), x, y, w, h, TRUE))
		{
			DWORD err = ::GetLastError();
			TYCHO_ASSERT(err);
		}
	}
}

/// Main window procedure
LRESULT WINAPI MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch(msg) 
    {	
		case WM_CLOSE : {
			close_window();		
            PostQuitMessage(0);
		} break;
		        
        case WM_PAINT : {
			static bool first = true;
			if(first)
			{
				dock_console_window();
				first = false;
			}
        } break;

        case WM_ACTIVATE: {
        } break;

        case WM_ERASEBKGND : {
            return FALSE;
        } break;

		case WM_MOVE :  {
			dock_console_window();
		} break;
        
        case WM_SIZE : {
			dock_console_window();
        } break;
        
        case WM_MOUSEMOVE : {
            static int ox, oy;
            static bool first = true;
            int nx, ny;

            if(first) {
                ox = GET_X_LPARAM(lParam);
                oy = GET_Y_LPARAM(lParam);
                first = false;
            }
            nx = GET_X_LPARAM(lParam);
            ny = GET_Y_LPARAM(lParam);
            //g_viewer.MouseHandler(nx - ox, ny - oy); 
            ox = nx;
            oy = ny;

            return 0L;
        } break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam );
} 

void close_window()
{
    // animate the window closed
    RECT r;
    ::GetWindowRect(g_hwnd, &r);
    float aspect_ratio = float(r.bottom - r.top) / float(r.right - r.left);
    float target_width = 80.0f;
    float target_height = target_width * aspect_ratio;
    enable_window_layering(g_hwnd, true);
    animate_window(g_hwnd, r, target_width, target_height, 255, 0);
    ::ShowWindow(g_hwnd, SW_HIDE);
    enable_window_layering(g_hwnd, false);
}

int WINAPI WinMain(__in HINSTANCE hInst, __in_opt HINSTANCE /*hPrevInstance*/, __in_opt LPSTR /*lpCmdLine*/, __in int /*nShowCmd*/)
{
	if(!g_game.initialise())
	{
		g_game.shutdown();
		return 0;
	}

	g_instance = hInst;
	
	// enumerate graphics drivers available
   // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_OWNDC | CS_BYTEALIGNWINDOW, MsgProc, 0L, 0L,
                      hInst, NULL, NULL, NULL, "MAIN_MENU",
                      "TestGame", NULL };

#if defined(HIMEMDEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    RegisterClassEx(&wc);
	g_instance = hInst;
	

    // Create the application's window 
    const int S_WIDTH  = 800;
    const int S_HEIGHT = 600;

	// calculate target size
    RECT r = { 0, 0, S_WIDTH-1, S_HEIGHT-1 };
    AdjustWindowRect(&r, WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX , FALSE);

	
	// calculate initial size;
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	int start_width = 0;
	int start_height = 0;
	int start_x = screen_width / 2 - start_width / 2;
	int start_y = screen_height / 2 - start_height / 2;
	RECT start_rect = { start_x, start_y, start_x + start_width, start_y + start_height };
    AdjustWindowRect(&start_rect, WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX , FALSE);
    g_hwnd = CreateWindow("TestGame", "Test Game",
                         WS_OVERLAPPEDWINDOW, start_rect.left, start_rect.top, start_rect.right - start_rect.left, start_rect.bottom - start_rect.top, 
                         GetDesktopWindow(), NULL, wc.hInstance, NULL);

    
    enable_window_layering(g_hwnd, true);
    set_window_alpha(g_hwnd, 0);
    InvalidateRect(g_hwnd, NULL, TRUE);
    ShowWindow(g_hwnd, SW_SHOWNORMAL);
    UpdateWindow(g_hwnd);
    animate_window(g_hwnd, start_rect, float(r.right - r.left), float(r.bottom - r.top), 0, 255);
    enable_window_layering(g_hwnd, false);
    
    // enter main game initialisation, this will not exit until game quit time
    g_game.run();

	close_window();    
	   
    // all done
    return g_game.shutdown();
}

math::vector2i get_window_size(HWND hwnd)
{
	RECT r;
	::GetWindowRect(hwnd, &r);
	return math::vector2i(r.right - r.left, r.bottom - r.top);
}

void clear_window(HWND hwnd)
{
	HDC hdc = ::GetDC(hwnd);
	math::vector2i size = get_window_size(hwnd);
	::PatBlt(hdc, 0, 0, size.x(), size.y(), BLACKNESS);
	::ReleaseDC(hwnd, hdc);
}

void enable_window_layering(HWND hwnd, bool s)
{
	if(s)
	 	::SetWindowLong(hwnd, GWL_EXSTYLE , ::GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	else
	 	::SetWindowLong(hwnd, GWL_EXSTYLE , ::GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
}

void set_window_alpha(HWND hwnd, BYTE alpha)
{
	::SetLayeredWindowAttributes(hwnd, RGB(0,0,0), alpha, LWA_ALPHA);
}

void animate_window(HWND hwnd, const RECT &rect, float target_width, float target_height, uint8 start_alpha, uint8 end_alpha)
{
    float anim_time = 0.7f;
    float64 start_time = core::clock::get_time();
    float window_x = float(rect.left);
    float window_y = float(rect.top);
    float window_width = float(rect.right - rect.left);
    float window_height = float(rect.bottom - rect.top);
    
    // shrink the window in towards the center
    float target_x = (window_x + window_width / 2) - target_width / 2;
    float target_y = (window_y + window_height / 2) - target_height / 2;
    
	for(;;)
    {
		platform_run();
		float64 cur_time = core::clock::get_time();
		float n = (float)((cur_time - start_time) / anim_time);
		if(n > 1.0f || n < 0.0f)
			break;
		float new_w = math::lerp<float>(n, window_width, target_width);
		float new_h = math::lerp<float>(n, window_height, target_height);
		float new_x = math::lerp<float>(n, window_x, target_x);
		float new_y = math::lerp<float>(n, window_y, target_y);
		BYTE alpha = (BYTE)math::lerp<float>(n, start_alpha, end_alpha);
		::MoveWindow(hwnd, (int)new_x, (int)new_y, (int)new_w, (int)new_h, FALSE);
		clear_window(hwnd);
		set_window_alpha(hwnd, alpha);
		Sleep(0);
    }
} 


/// Process all window messages currently in the message queue.
int platform_run()
{    
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));           
	while(::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) 
	{
		if(msg.message == WM_QUIT)
		{
			return 0;
		}
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	} 
	
    // All done
    return 1; 	
}

void platform_cleanup()
{
    // Clean up everything and exit the app
    UnregisterClass("TestGame", g_instance);
}

void platform_show_window()
{


}
