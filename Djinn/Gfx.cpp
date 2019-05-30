#include "Gfx.h"


using namespace Microsoft::WRL;
using namespace Djn;
using namespace std;


Gfx::Gfx()
{
#ifdef _DEBUG
    ComPtr<ID3D12Debug> debugController;
    ThrowIfFailed(D3D12GetDebugInterface(
        IID_PPV_ARGS(&debugController)));
    debugController->EnableDebugLayer();
#endif

    // Create factory object.
    ThrowIfFailed(CreateDXGIFactory1(
        IID_PPV_ARGS(&_factory)));

    // Enumerate adapters and outputs.
    UINT i = 0;
    IDXGIAdapter *current_adapter = nullptr;
    while (_factory->EnumAdapters(i++, &current_adapter) != DXGI_ERROR_NOT_FOUND) {
        AdapterInfo newAdapter;
        current_adapter->GetDesc(&newAdapter.desc);

        UINT j = 0;
        IDXGIOutput* current_output = nullptr;
        while (current_adapter->EnumOutputs(j++, &current_output) != DXGI_ERROR_NOT_FOUND) {
            UINT modeCount = 0;
            current_output->GetDisplayModeList(
                _renderTextureFormat, 0, &modeCount, nullptr);
            auto modeList = new DXGI_MODE_DESC[modeCount];
            current_output->GetDisplayModeList(
                _renderTextureFormat, 0, &modeCount, modeList);

            OutputInfo newOutput;
            current_output->GetDesc(&newOutput.desc);
            for (UINT k = 0U; k < modeCount; ++k) {
                if (modeList[k].Width == 1920U && modeList[k].Height == 1080U) {
                    if (_preferredOutputMode.RefreshRate.Denominator == 0) {
                        _preferredOutputMode = modeList[k];
                    } else {
                        auto& newRefresh = modeList[k].RefreshRate;
                        double newRate = newRefresh.Numerator / newRefresh.Denominator;
                        auto& oldRefresh = _preferredOutputMode.RefreshRate;
                        double oldRate = oldRefresh.Numerator / oldRefresh.Denominator;
                        if (newRate > oldRate) {
                            _preferredOutputMode = modeList[k];
                        }
                    }
                }
                newOutput.modeDescs.push_back(modeList[k]);
            }
            newAdapter.outputs.push_back(newOutput);
            current_output->Release();
            current_output = nullptr;
        }
        _adapterInfo.push_back(newAdapter);
        current_adapter->Release();
        current_adapter = nullptr;
    }

#ifdef _DEBUG
    wstring log = L"Enumerating Graphics Hardware Results...";
    for (auto& adapter : _adapterInfo) {
        log += wstring(L"\nAdapter: ") + adapter.desc.Description;
        for (auto& output : adapter.outputs) {
            log += wstring(L"\n- Output: ") + output.desc.DeviceName;
            /*for (auto& mode : output.modeDescs) {
            log += wstring(L"\n   - ");
            log += to_wstring(mode.Width) + wstring(L"x") + to_wstring(mode.Height);
            auto& num = mode.RefreshRate.Numerator;
            auto& den = mode.RefreshRate.Denominator;
            log += wstring(L" @ ") + to_wstring(num / den) + wstring(L"hz");
            }*/
        }
    }
    Log::Write(log);
#endif

    // Create device object.
    ThrowIfFailed(D3D12CreateDevice(
        nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)));

    // Enumerate supported MSAA quality levels.
    UINT sampleCount = 1;
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msFeatureData = {};
    msFeatureData.Format = _renderTextureFormat;
    msFeatureData.SampleCount = sampleCount;
    msFeatureData.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    auto result = _device->CheckFeatureSupport(
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &msFeatureData, sizeof(msFeatureData));
    if (result == E_FAIL) {
        throw Exception(
            string("Unable to retrieve NumQualityLevels for sample count ") +
            to_string(msFeatureData.SampleCount));
    } else {
        ThrowIfFailed(result);
    }
    _defaultSampleDesc.Count = sampleCount;
    _defaultSampleDesc.Quality = msFeatureData.NumQualityLevels - 1;

    // Create command objects.
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
    cmdQueueDesc.Type = _cmdListType;
    cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    ThrowIfFailed(_device->CreateCommandQueue(
        &cmdQueueDesc,
        IID_PPV_ARGS(&_cmdQueue)));
    ThrowIfFailed(_device->CreateCommandAllocator(
        _cmdListType,
        IID_PPV_ARGS(_cmdAllocator.GetAddressOf())));
    ThrowIfFailed(_device->CreateCommandList(
        0, _cmdListType, _cmdAllocator.Get(), nullptr,
        IID_PPV_ARGS(_cmdList.GetAddressOf())));
    _cmdList->Close();

    // Create desc heaps.
    auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    _rtvDescSize = _device->GetDescriptorHandleIncrementSize(heapType);
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = heapType;
    heapDesc.NumDescriptors = SwapChainBufferCount;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    heapDesc.NodeMask = 0;
    ThrowIfFailed(_device->CreateDescriptorHeap(
        &heapDesc, IID_PPV_ARGS(_rtvDescHeap.GetAddressOf())
    ));
    heapType = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    _dsvDescSize = _device->GetDescriptorHandleIncrementSize(heapType);
    heapDesc.Type = heapType;
    heapDesc.NumDescriptors = 1;
    ThrowIfFailed(_device->CreateDescriptorHeap(
        &heapDesc, IID_PPV_ARGS(_dsvDescHeap.GetAddressOf())
    ));
    heapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    _cbvSrvUavDescSize = _device->GetDescriptorHandleIncrementSize(heapType);
}


Gfx::~Gfx()
{
    FlushCommandQueue();
}


void Gfx::Init(HWND hWnd, uint2 windowSize)
{
    _hWnd = hWnd;
    _renderSize = windowSize;

    ThrowIfFailed(_device->CreateFence(
        0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));

    // Create swap chain.
    _swapChain.Reset();
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    swapChainDesc.Width = _renderSize.x;
    swapChainDesc.Height = _renderSize.y;
    swapChainDesc.Format = _renderTextureFormat;
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc = _defaultSampleDesc;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = SwapChainBufferCount;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    /*DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc;
    swapChainFullscreenDesc.RefreshRate;
    swapChainFullscreenDesc.ScanlineOrdering;
    swapChainFullscreenDesc.Scaling;
    swapChainFullscreenDesc.Windowed;*/
    ThrowIfFailed(_factory->CreateSwapChainForHwnd(
        _cmdQueue.Get(),
        _hWnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        _swapChain.GetAddressOf()));

    Resize(_renderSize);
}


void Gfx::Resize(uint2 newSize)
{
    FlushCommandQueue();

    _renderSize = newSize;

    ThrowIfFailed(_cmdList->Reset(_cmdAllocator.Get(), nullptr));

    for (int i = 0; i < SwapChainBufferCount; ++i) {
        _swapChainBuffer[i].Reset();
    }
    _depthStencilBuffer.Reset();

    _swapChain->ResizeBuffers(
        SwapChainBufferCount,
        _renderSize.x, _renderSize.y,
        _renderTextureFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

    _currentBackBufferIdx = 0;

    auto rtvHeapHandle = _rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
    for (auto i = 0; i < SwapChainBufferCount; ++i) {
        ThrowIfFailed(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_swapChainBuffer[i])));
        _device->CreateRenderTargetView(_swapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.ptr += _rtvDescSize;
    }

    // Create depth stencil view.
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = _renderSize.x;
    depthStencilDesc.Height = _renderSize.y;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthStencilDesc.SampleDesc = _defaultSampleDesc;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = _depthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;

    D3D12_HEAP_PROPERTIES dsvHeapProperties;
    dsvHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    dsvHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    dsvHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    dsvHeapProperties.CreationNodeMask = 1;
    dsvHeapProperties.VisibleNodeMask = 1;

    ThrowIfFailed(_device->CreateCommittedResource(
        &dsvHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(_depthStencilBuffer.GetAddressOf())
    ));

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Format = _depthStencilFormat;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.Texture2D.MipSlice = 0;
    _device->CreateDepthStencilView(
        _depthStencilBuffer.Get(), &dsvDesc,
        _dsvDescHeap->GetCPUDescriptorHandleForHeapStart()
    );

    D3D12_RESOURCE_BARRIER resourceBarrier;
    resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    resourceBarrier.Transition.pResource = _depthStencilBuffer.Get();
    resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
    resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    _cmdList->ResourceBarrier(1, &resourceBarrier);

    ThrowIfFailed(_cmdList->Close());
    ID3D12CommandList* cmdLists[] = { _cmdList.Get() };
    _cmdQueue->ExecuteCommandLists(1, cmdLists);

    FlushCommandQueue();

    _screenViewport.TopLeftX = 0;
    _screenViewport.TopLeftY = 0;
    _screenViewport.Width = static_cast<float>(_renderSize.x);
    _screenViewport.Height = static_cast<float>(_renderSize.y);
    _screenViewport.MinDepth = 0.0f;
    _screenViewport.MaxDepth = 1.0f;

    _scissorRect.left = 0;
    _scissorRect.top = 0;
    _scissorRect.right = _renderSize.x;
    _scissorRect.bottom = _renderSize.y;
}


void Gfx::Draw()
{
    try {
        ThrowIfFailed(_cmdAllocator->Reset());

        ThrowIfFailed(_cmdList->Reset(_cmdAllocator.Get(), nullptr));

        auto backBuffer = _swapChainBuffer[_currentBackBufferIdx].Get();
        auto hBackBufferDesc =_rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
        hBackBufferDesc.ptr += _currentBackBufferIdx * _rtvDescSize;
        auto hDepthStencilDesc = _dsvDescHeap->GetCPUDescriptorHandleForHeapStart();
    
        D3D12_RESOURCE_BARRIER renderTextureSwapBarrier;
        renderTextureSwapBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        renderTextureSwapBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        renderTextureSwapBarrier.Transition.pResource = backBuffer;
        renderTextureSwapBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        renderTextureSwapBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        renderTextureSwapBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        _cmdList->ResourceBarrier(1, &renderTextureSwapBarrier);

        _cmdList->RSSetViewports(1, &_screenViewport);
        _cmdList->RSSetScissorRects(1, &_scissorRect);

        _cmdList->ClearRenderTargetView(
            hBackBufferDesc, DirectX::Colors::LightSteelBlue, 0, nullptr);
        _cmdList->ClearDepthStencilView(
            hDepthStencilDesc,
            D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
        _cmdList->OMSetRenderTargets(1, &hBackBufferDesc, true, &hDepthStencilDesc);

        renderTextureSwapBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        renderTextureSwapBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        _cmdList->ResourceBarrier(1, &renderTextureSwapBarrier);

        _cmdList->Close();

        ID3D12CommandList* cmdLists[] = { _cmdList.Get() };
        _cmdQueue->ExecuteCommandLists(1, cmdLists);

        ThrowIfFailed(_swapChain->Present(0, 0));
        _currentBackBufferIdx = (_currentBackBufferIdx + 1) % SwapChainBufferCount;

        FlushCommandQueue();
    } catch (...) {
        throw Exception(GetDebugLayerMessages());
    }
}


void Gfx::FlushCommandQueue()
{
    _currentFence++;
    ThrowIfFailed(_cmdQueue->Signal(_fence.Get(), _currentFence));

    if (_fence->GetCompletedValue() < _currentFence) {
        HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

        ThrowIfFailed(_fence->SetEventOnCompletion(_currentFence, eventHandle));

        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}


void Gfx::ThrowIfFailed(HRESULT result)
{
    if (!FAILED(result)) return;

    _com_error error(result);
    _bstr_t bstr(error.ErrorMessage());
    string debugLayerMessages = GetDebugLayerMessages();
    throw Exception(debugLayerMessages + string(bstr));
}

string Gfx::GetDebugLayerMessages()
{
    string errorMessage = "";
#ifdef _DEBUG
    if (_device.Get() != nullptr) {
        ComPtr<ID3D12InfoQueue> infoQueue;
        if (FAILED(_device->QueryInterface(infoQueue.GetAddressOf()))) {
            errorMessage += "Unable to query device interface.";
        } else {
            auto numStoredMessages = infoQueue->GetNumStoredMessages();
            errorMessage += "D3D12 Debug Layer Messages: " + to_string(numStoredMessages);
            for (auto i = 0; i < numStoredMessages; ++i) {
                errorMessage += "\n - ";
                size_t msgSize;
                if (FAILED(infoQueue->GetMessage(i, NULL, &msgSize))) {
                    errorMessage += "Unable to get message size from info queue.";
                    continue;
                }
                D3D12_MESSAGE* msg = (D3D12_MESSAGE*)malloc(msgSize);
                if (FAILED(infoQueue->GetMessage(i, msg, &msgSize))) {
                    errorMessage += "Unable to get message from info queue.";
                    continue;
                }
                errorMessage += string(msg->pDescription);
                free(msg);
            }
        }
    }
#endif
    return errorMessage;
}
