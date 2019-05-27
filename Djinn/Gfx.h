#pragma once

#include <string>
#include <vector>

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
//#include <DirectXMath.h>
//#include <DirectXColors.h>

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
    private:
        Gfx();
        ~Gfx();
        // Settings.
        D3D12_COMMAND_LIST_TYPE _cmdListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
        DXGI_FORMAT _renderTextureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        DXGI_FORMAT _depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        DXGI_SAMPLE_DESC _defaultSampleDesc;
        static const UINT SwapChainBufferCount = 2;
        UINT _currentBackBuffer = 0;
        UINT64 _currentFence = 0;
        HWND _hWnd;
        uint2 _renderSize;
        D3D12_VIEWPORT _screenViewport;
        D3D12_RECT _scissorRect;
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
        UINT _rtvDescSize;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvDescHeap;
        UINT _dsvDescSize;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _dsvDescHeap;
        UINT _cbvSrvUavDescSize;
        std::vector<AdapterInfo> _adapterInfo;
        DXGI_MODE_DESC _preferredOutputMode;
    public:
        void Init(HWND hWnd, uint2 windowSize);
        void Resize(uint2 newSize);
        void Draw();
    private:
        void FlushCommandQueue();
        void ThrowIfFailed(HRESULT result);
    };
}
