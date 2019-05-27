#pragma once

#include <string>
#include <vector>

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "Djinn.h"

namespace Djn
{
    typedef struct OutputInfo {
        DXGI_OUTPUT_DESC desc;
        std::vector<DXGI_MODE_DESC> modeDescs;
    } OutputInfo;

    typedef struct AdapterInfo {
        DXGI_ADAPTER_DESC desc;
        std::vector<OutputInfo> outputs;
    } AdapterInfo;

    class Gfx
    {
    public:
        static Gfx& Instance() {
            static Gfx instance;
            return instance;
        }
    public:
        void Init(HWND hWnd, uint2 windowSize);
        void Resize(uint2 newSize);
    private:
        Gfx();
        ~Gfx();
        // Settings.
        D3D12_COMMAND_LIST_TYPE _cmdListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
        DXGI_FORMAT _renderFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_SAMPLE_DESC _defaultSampleDesc;
        static const UINT SwapChainBufferCount = 2;
        UINT _currentBackBuffer = 0;
        HWND _hWnd;
        uint2 _renderSize;
        // Base objects.
        Microsoft::WRL::ComPtr<IDXGIFactory4> _factory;
        Microsoft::WRL::ComPtr<ID3D12Device> _device;
        // Command objects.
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _cmdAllocator;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _cmdList;
        // Graphics resources.
        Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
        Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
        Microsoft::WRL::ComPtr<ID3D12Resource> _swapChainBuffer[SwapChainBufferCount];
        Microsoft::WRL::ComPtr<ID3D12Resource> _depthStencilBuffer;
        std::vector<AdapterInfo> _adapterInfo;
        DXGI_MODE_DESC _preferredOutputMode;
        void ThrowIfFailed(HRESULT result);
    };
}
