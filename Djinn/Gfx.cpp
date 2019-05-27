#include "Gfx.h"


using namespace Microsoft::WRL;
using namespace DirectX;
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
                _renderFormat, 0, &modeCount, nullptr);
            auto modeList = new DXGI_MODE_DESC[modeCount];
            current_output->GetDisplayModeList(
                _renderFormat, 0, &modeCount, modeList);

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
    msFeatureData.Format = _renderFormat;
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
        &cmdQueueDesc, IID_PPV_ARGS(&_cmdQueue)));

    ThrowIfFailed(_device->CreateCommandAllocator(
        _cmdListType, IID_PPV_ARGS(_cmdAllocator.GetAddressOf())));

    ThrowIfFailed(_device->CreateCommandList(
        0, _cmdListType, _cmdAllocator.Get(), nullptr, IID_PPV_ARGS(_cmdList.GetAddressOf())));

    _cmdList->Close();
}


Gfx::~Gfx() {}


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
    swapChainDesc.Format = _renderFormat;
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

    // TODO: descriptor heaps, et. al.

    Resize(_renderSize);
}

void Gfx::Resize(uint2 newSize)
{
    // TODO: Flush command queue.

    _renderSize = newSize;

    ThrowIfFailed(_cmdList->Reset(_cmdAllocator.Get(), nullptr));

    for (int i = 0; i < SwapChainBufferCount; ++i) {
        _swapChainBuffer[i].Reset();
    }
    _depthStencilBuffer.Reset();

    _swapChain->ResizeBuffers(
        SwapChainBufferCount,
        _renderSize.x, _renderSize.y,
        _renderFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

    _currentBackBuffer = 0;

   
    ThrowIfFailed(_cmdList->Close());
}


void Gfx::ThrowIfFailed(HRESULT result)
{
    if (!FAILED(result)) return;

    string errorMessage;
#ifdef _DEBUG
    if (_device.Get() != nullptr) {
        ComPtr<ID3D12InfoQueue> infoQueue;
        if (FAILED(_device->QueryInterface(infoQueue.GetAddressOf()))) {
            errorMessage += "Unable to query device interface.";
        } else {
            auto numStoredMessages = infoQueue->GetNumStoredMessages();
            errorMessage += "D3D12 Debug Layer Messages: " + to_string(numStoredMessages);
            for (auto i = 0; i < numStoredMessages; ++i) {
                errorMessage += "\nInfo queue message: ";
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
            errorMessage += "\n\n";
        }
    }
#endif

    _com_error error(result);
    _bstr_t bstr(error.ErrorMessage());
    throw Exception(errorMessage + string(bstr));
}
