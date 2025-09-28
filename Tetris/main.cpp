#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "functions.h"
#include <iostream>
#include <string>
#include <array>

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImFont* LoadFontWithFallback(const char* primaryPath, const char* fallbackPath, float size) {
    ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(primaryPath, size);
    if (!font) {
        std::cout << "Warning: Could not load font from " << primaryPath << std::endl;
        std::cout << "Please place Tetris.ttf in the current directory or download it from: https://www.dafont.com/es/tetris.font" << std::endl;
        std::cout << "Using fallback font..." << std::endl;
        font = ImGui::GetIO().Fonts->AddFontFromFileTTF(fallbackPath, size);
        if (!font) {
            std::cout << "Error: Could not load fallback font either. Using default font." << std::endl;
            font = ImGui::GetIO().Fonts->AddFontDefault();
        }
    }
    return font;
}


int main(int, char**) {
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    RegisterClassExW(&wc);
    HWND hwnd = CreateWindowW(wc.lpszClassName, L"Tetris", WS_OVERLAPPEDWINDOW, 0, 0,
        gl::columns * gl::PieceSize + gl::PieceSize * 8,
        gl::rows * gl::PieceSize + gl::PieceSize, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    gl::TetrisFont = LoadFontWithFallback("Tetris.ttf", "C:/Windows/Fonts/arial.ttf", 40.0f);
    ImFont* tetrisFontBig = LoadFontWithFallback("Tetris.ttf", "C:/Windows/Fonts/arial.ttf", 55.0f);

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    std::random_device rd;
    std::mt19937 gen(rd());
    gl::gen = gen;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    std::thread ApplyGravity(Tetris::ApplyGravity);
    std::thread GeneratePiece(Tetris::GeneratePiece);
    ApplyGravity.detach();
    GeneratePiece.detach();

    bool done = false;
    while (!done) {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0) {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        gl::drawList = ImGui::GetForegroundDrawList();

        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), io.DisplaySize, Colors::Black);
        gl::drawList->AddLine(ImVec2(gl::columns * gl::PieceSize + 10.f, 0),
            ImVec2(gl::columns * gl::PieceSize + 10.f, io.DisplaySize.y),
            Colors::Grey, 18.f);

        int keyPressed = 0;
        if (GetAsyncKeyState(VK_LEFT) & 1)
            keyPressed = VK_LEFT;
        else if (GetAsyncKeyState(VK_RIGHT) & 1)
            keyPressed = VK_RIGHT;
        else if (GetAsyncKeyState(VK_UP) & 1)
            keyPressed = VK_UP;
        else if (GetAsyncKeyState(VK_DOWN) & 1)
            keyPressed = VK_DOWN;
        else if (GetAsyncKeyState(VK_SPACE) & 1)
            keyPressed = VK_SPACE;

        if (keyPressed != 0)
            Tetris::HandleInput(keyPressed);

        Tetris::CheckRowComplete();
        Tetris::Render();
        Tetris::RenderPredict();

        if (gl::GameFinished) {
            ImGui::PushFont(tetrisFontBig);
            gl::drawList->AddText(ImVec2(50.f, io.DisplaySize.y / 2), Colors::Red, "YOU LOST");
            ImGui::PopFont();
        }

        std::string ScoreString = "SCORE: " + std::to_string(gl::score);

        ImGui::PushFont(gl::TetrisFont);
        gl::drawList->AddText(ImVec2(gl::columns * gl::PieceSize + 10.f + 55.f, 0), Colors::White, ScoreString.c_str());
        ImGui::PopFont();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w,
                                                 clear_color.y * clear_color.w,
                                                 clear_color.z * clear_color.w,
                                                 clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
        &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) {
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
            featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
            &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    }
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}