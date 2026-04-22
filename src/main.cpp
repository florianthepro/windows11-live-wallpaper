#include <windows.h>
#include <WebView2.h>
#include <wrl.h>

HWND hwnd = nullptr, workerw = nullptr;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM)
{
    if (FindWindowEx(hWnd, nullptr, L"SHELLDLL_DefView", nullptr))
        workerw = FindWindowEx(nullptr, hWnd, L"WorkerW", nullptr);
    return TRUE;
}

void AttachToDesktop(HWND hwnd)
{
    HWND progman = FindWindow(L"Progman", nullptr);
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 100, nullptr);
    EnumWindows(EnumWindowsProc, 0);
    SetParent(hwnd, workerw);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM, LPARAM)
{
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, 0, 0);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, int)
{
    WNDCLASS wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"ClassicLiveWallpaper";
    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        wc.lpszClassName,
        L"",
        WS_POPUP,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, hInst, nullptr
    );

    AttachToDesktop(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    Microsoft::WRL::ComPtr<ICoreWebView2Environment> env;
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Microsoft::WRL::Callback<
            ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&](HRESULT, ICoreWebView2Environment* environment) -> HRESULT
        {
            environment->CreateCoreWebView2Controller(
                hwnd,
                Microsoft::WRL::Callback<
                    ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [&](HRESULT, ICoreWebView2Controller* controller) -> HRESULT
                {
                    RECT r; GetClientRect(hwnd, &r);
                    controller->put_Bounds(r);

                    ICoreWebView2* webview;
                    controller->get_CoreWebView2(&webview);

                    wchar_t url[512];
                    MultiByteToWideChar(CP_UTF8, 0, lpCmdLine, -1, url, 512);
                    webview->Navigate(url);

                    return S_OK;
                }).Get());
            return S_OK;
        }).Get());

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
