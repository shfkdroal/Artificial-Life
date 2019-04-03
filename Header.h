#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "WAVFileReader.h" //음악 설정 헤더 파일
#include "DDSTextureLoader.h" //MS에서 텍스쳐 쉽게 하라고 만들어놓음
#include "resource.h"
#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;
using namespace DirectX;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
int flip_Value = 1;
enum action {
	r = 0,
	l = 1,
	g = 2,
	atk = 3,
	s = 4
};


struct SimpleVertex		//정점 노말벡터
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT4 Color;
	XMFLOAT2 TextrureCoord;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT3 vLightPos;
	XMFLOAT4 vLightDir;
	XMFLOAT4 vLightColor;
	XMFLOAT4 vOutputColor;
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz; //여기선 노멀벡터로 지정
	float cx, cy, cz; //여기선 칼라벡터로 지정
}; //모델 타입

struct CollisionBox
{
	BoundingOrientedBox obox;
	ContainmentType collision;
};

struct CollisionRay
{
	XMVECTOR origin;
	XMVECTOR direction;
};

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = nullptr;
ID3D11Device1*          g_pd3dDevice1 = nullptr;
ID3D11DeviceContext*    g_pImmediateContext = nullptr;
ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
IDXGISwapChain*         g_pSwapChain = nullptr;
IDXGISwapChain1*        g_pSwapChain1 = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
ID3D11Texture2D*        g_pDepthStencil = nullptr;
ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
ID3D11VertexShader*     g_pVertexShader = nullptr;
ID3D11PixelShader*      g_pPixelShader = nullptr;
ID3D11PixelShader*      g_pPixelShaderSolid = nullptr;
ID3D11InputLayout*      g_pVertexLayout = nullptr;
ID3D11Buffer*           g_pVertexBuffer[11] = { nullptr, };
ID3D11Buffer*           g_pIndexBuffer[11] = { nullptr, };
ID3D11Buffer*           g_pConstantBuffer[11] = { nullptr, };
XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

ID3D11ShaderResourceView*           g_pTextureRV = nullptr;//텍스쳐
ID3D11ShaderResourceView*           g_pTextureRV2 = nullptr;//텍스쳐
ID3D11SamplerState*                 g_pSamplerLinear = nullptr;//텍스쳐


float mouse_X;
float mouse_Y;
bool keyList[12];
int map[300][300];
int pickBodyIndex;
BOOL isEffectRunning = FALSE;// 이펙트 소리 전역변수화


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();
HRESULT bind(int num, SimpleVertex *_model, WORD *_index, int count);
void initEnemy();
void startStage(int num);
int PickBody();

void attack(int);
void initFood();
void InitbodyInform();

//--------------------------------------------------------------------------------------
// Create buffer bind
//--------------------------------------------------------------------------------------
HRESULT bind(int num, SimpleVertex *_model, WORD *_index, int count) {
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	// Create vertex buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * count;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = _model;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer[num]);
	if (FAILED(hr))
		return hr;

	// Create index buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * count;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = _index;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer[num]);
	if (FAILED(hr))
		return hr;

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[num]);
	if (FAILED(hr))
		return hr;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"Team 6 Game";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"Team 6 Game", L"Team 6 Game",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return hr;

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1));
		if (SUCCEEDED(hr))
		{
			(void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, g_hWnd, &sd, nullptr, nullptr, &g_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = g_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(L"VSPS.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);


	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"VSPS.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Compile the pixel shader
	pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"VSPS.fx", "PSSolid", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShaderSolid);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	
	// 텍스처 추가부분
	//--------------------------------------------------------------------------------------
    // Load the Texture
	// 메모리로부터 텍스쳐를 읽어오는 부분 resource폴더의 seafloor.dds 이미지를 가져온다
	// C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Utilities\bin\x86 이경로를 통해서 다른 이미지를 dds파일로 변환해주는게 있다
	// DxTex.exe 파일이다.
	// g_pTextureRV : 텍스쳐 자료에대한 포인터
    hr = CreateDDSTextureFromFile( g_pd3dDevice, L"seafloor.dds", nullptr, &g_pTextureRV );
    if( FAILED( hr ) )
        return hr;

	hr = CreateDDSTextureFromFile( g_pd3dDevice, L"white.dds", nullptr, &g_pTextureRV2 );
    if( FAILED( hr ) )
       return hr;

    // Create the sample state
	// sampler state : 텍스쳐를 쉐이더에서 어떻게 사용할 것인지에 대한 description
	// g_pSamplerLinear : sampler state에 대한 포인터
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //선형 보간
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerLinear );
    if( FAILED( hr ) )
        return hr;

	//--------------------------------------------------------------------------------------
	// bind function original position
	//--------------------------------------------------------------------------------------

	// Initialize the world matrices
	g_World = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 300.0f);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	for (int i = 0; i < 11; i++) {
		if (g_pConstantBuffer) g_pConstantBuffer[i]->Release();
		if (g_pVertexBuffer) g_pVertexBuffer[i]->Release();
		if (g_pIndexBuffer) g_pIndexBuffer[i]->Release();
	}
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShaderSolid) g_pPixelShaderSolid->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pDepthStencil) g_pDepthStencil->Release();
	if (g_pDepthStencilView) g_pDepthStencilView->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain1) g_pSwapChain1->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext1) g_pImmediateContext1->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice1) g_pd3dDevice1->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// 음악 설정
namespace
{
    
struct handle_closer { void operator()(HANDLE h) { if (h) CloseHandle(h); } };

typedef public std::unique_ptr<void, handle_closer> ScopedHandle;

inline HANDLE safe_handle( HANDLE h ) { return (h == INVALID_HANDLE_VALUE) ? 0 : h; }


//--------------------------------------------------------------------------------------
// .WAV files
//--------------------------------------------------------------------------------------
const uint32_t FOURCC_RIFF_TAG      = 'FFIR';
const uint32_t FOURCC_FORMAT_TAG    = ' tmf';
const uint32_t FOURCC_DATA_TAG      = 'atad';
const uint32_t FOURCC_WAVE_FILE_TAG = 'EVAW';
const uint32_t FOURCC_XWMA_FILE_TAG = 'AMWX';
const uint32_t FOURCC_DLS_SAMPLE    = 'pmsw';
const uint32_t FOURCC_MIDI_SAMPLE   = 'lpms';
const uint32_t FOURCC_XWMA_DPDS     = 'sdpd';
const uint32_t FOURCC_XMA_SEEK      = 'kees';

#pragma pack(push,1)
struct RIFFChunk
{
    uint32_t tag;
    uint32_t size;
};

struct RIFFChunkHeader
{
    uint32_t tag;
    uint32_t size;
    uint32_t riff;
};

struct DLSLoop
{
    static const uint32_t LOOP_TYPE_FORWARD = 0x00000000;
    static const uint32_t LOOP_TYPE_RELEASE = 0x00000001;

    uint32_t size;
    uint32_t loopType;
    uint32_t loopStart;
    uint32_t loopLength;
};

struct RIFFDLSSample
{
    static const uint32_t OPTIONS_NOTRUNCATION = 0x00000001;
    static const uint32_t OPTIONS_NOCOMPRESSION = 0x00000002;

    uint32_t    size;
    uint16_t    unityNote;
    int16_t     fineTune;
    int32_t     gain;
    uint32_t    options;
    uint32_t    loopCount;
};

struct MIDILoop
{
    static const uint32_t LOOP_TYPE_FORWARD     = 0x00000000;
    static const uint32_t LOOP_TYPE_ALTERNATING = 0x00000001;
    static const uint32_t LOOP_TYPE_BACKWARD    = 0x00000002;

    uint32_t cuePointId;
    uint32_t type;
    uint32_t start;
    uint32_t end;
    uint32_t fraction;
    uint32_t playCount;
};

struct RIFFMIDISample
{
    uint32_t        manufacturerId;
    uint32_t        productId;
    uint32_t        samplePeriod;
    uint32_t        unityNode;
    uint32_t        pitchFraction;
    uint32_t        SMPTEFormat;
    uint32_t        SMPTEOffset;
    uint32_t        loopCount;
    uint32_t        samplerData;
};
#pragma pack(pop)

static_assert( sizeof(RIFFChunk) == 8, "structure size mismatch");
static_assert( sizeof(RIFFChunkHeader) == 12, "structure size mismatch");
static_assert( sizeof(DLSLoop) == 16, "structure size mismatch");
static_assert( sizeof(RIFFDLSSample) == 20, "structure size mismatch");
static_assert( sizeof(MIDILoop) == 24, "structure size mismatch");
static_assert( sizeof(RIFFMIDISample) == 36, "structure size mismatch");

};


//--------------------------------------------------------------------------------------
static const RIFFChunk* FindChunk( _In_reads_bytes_(sizeBytes) const uint8_t* data, _In_ size_t sizeBytes, _In_ uint32_t tag )
{
    if ( !data )
        return nullptr;

    const uint8_t* ptr = data;
    const uint8_t* end = data + sizeBytes;

    while ( end > ( ptr + sizeof(RIFFChunk) ) )
    {
        auto header = reinterpret_cast<const RIFFChunk*>( ptr );
        if ( header->tag == tag )
            return header;

        ptrdiff_t offset = header->size + sizeof(RIFFChunk);
        ptr += offset;
    }

    return nullptr;
}


//--------------------------------------------------------------------------------------
static HRESULT WaveFindFormatAndData( _In_reads_bytes_(wavDataSize) const uint8_t* wavData, _In_ size_t wavDataSize,
                                      _Outptr_ const WAVEFORMATEX** pwfx, _Outptr_ const uint8_t** pdata, _Out_ uint32_t* dataSize,
                                      _Out_ bool& dpds, _Out_ bool& seek )
{
    if ( !wavData || !pwfx )
        return E_POINTER;

    dpds = seek = false;

    if (wavDataSize < (sizeof(RIFFChunk)*2 + sizeof(uint32_t) + sizeof(WAVEFORMAT) ) )
    {
        return E_FAIL;
    }

    const uint8_t* wavEnd = wavData + wavDataSize;

    // Locate RIFF 'WAVE'
    auto riffChunk = FindChunk( wavData, wavDataSize, FOURCC_RIFF_TAG );
    if ( !riffChunk || riffChunk->size < 4 )
    {
        return E_FAIL;
    }

    auto riffHeader = reinterpret_cast<const RIFFChunkHeader*>( riffChunk );
    if ( riffHeader->riff != FOURCC_WAVE_FILE_TAG && riffHeader->riff != FOURCC_XWMA_FILE_TAG )
    {
        return E_FAIL;
    }

    // Locate 'fmt '
    auto ptr = reinterpret_cast<const uint8_t*>( riffHeader ) + sizeof(RIFFChunkHeader);
    if ( ( ptr + sizeof(RIFFChunk) ) > wavEnd )
    {
        return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
    }

    auto fmtChunk = FindChunk( ptr, riffHeader->size, FOURCC_FORMAT_TAG );
    if ( !fmtChunk || fmtChunk->size < sizeof(PCMWAVEFORMAT) )
    {
        return E_FAIL;
    }

    ptr = reinterpret_cast<const uint8_t*>( fmtChunk ) + sizeof( RIFFChunk );
    if ( ptr + fmtChunk->size > wavEnd )
    {
        return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
    }

    auto wf = reinterpret_cast<const WAVEFORMAT*>( ptr );

    // Validate WAVEFORMAT (focused on chunk size and format tag, not other data that XAUDIO2 will validate)
    switch( wf->wFormatTag )
    {
    case WAVE_FORMAT_PCM:
    case WAVE_FORMAT_IEEE_FLOAT:
        // Can be a PCMWAVEFORMAT (8 bytes) or WAVEFORMATEX (10 bytes)
        // We validiated chunk as at least sizeof(PCMWAVEFORMAT) above
        break;

    default:
        {
            if ( fmtChunk->size < sizeof(WAVEFORMATEX) )
            {
                return E_FAIL;
            }

            auto wfx = reinterpret_cast<const WAVEFORMATEX*>( ptr );

            if ( fmtChunk->size < ( sizeof(WAVEFORMATEX) + wfx->cbSize ) )
            {
                return E_FAIL;
            }

            switch( wfx->wFormatTag )
            {
            case WAVE_FORMAT_WMAUDIO2:
            case WAVE_FORMAT_WMAUDIO3: // xWMA is supported by XAudio 2.7 and by Xbox One
                dpds = true;
                break;

            case  0x166 /*WAVE_FORMAT_XMA2*/: // XMA2 is supported by Xbox One
                if ( ( fmtChunk->size < 52 /*sizeof(XMA2WAVEFORMATEX)*/ ) || ( wfx->cbSize < 34 /*( sizeof(XMA2WAVEFORMATEX) - sizeof(WAVEFORMATEX) )*/ ) )
                {
                    return E_FAIL;
                }
                seek = true;
                break;

            case WAVE_FORMAT_ADPCM:
                if ( ( fmtChunk->size < ( sizeof(WAVEFORMATEX) + 32 ) ) || ( wfx->cbSize < 32 /*MSADPCM_FORMAT_EXTRA_BYTES*/ ) )
                {
                    return E_FAIL;
                }
                break;

            case WAVE_FORMAT_EXTENSIBLE:
                if ( ( fmtChunk->size < sizeof(WAVEFORMATEXTENSIBLE) ) || ( wfx->cbSize < ( sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) ) ) )
                {
                    return E_FAIL;
                }
                else
                {
                     static const GUID s_wfexBase = {0x00000000, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};

                     auto wfex = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>( ptr );

                    if ( memcmp( reinterpret_cast<const BYTE*>(&wfex->SubFormat) + sizeof(DWORD),
                                 reinterpret_cast<const BYTE*>(&s_wfexBase) + sizeof(DWORD), sizeof(GUID) - sizeof(DWORD) ) != 0 )
                    {
                        return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );
                    }

                    switch( wfex->SubFormat.Data1 )
                    {
                    case WAVE_FORMAT_PCM:
                    case WAVE_FORMAT_IEEE_FLOAT:
                        break;

                    // MS-ADPCM and XMA2 are not supported as WAVEFORMATEXTENSIBLE

                    case WAVE_FORMAT_WMAUDIO2:
                    case WAVE_FORMAT_WMAUDIO3:
                        dpds = true;
                        break;

                    default:
                        return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );
                    }

                }
                break;

            default:
                return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );
            }
        }
    }

    // Locate 'data'
    ptr = reinterpret_cast<const uint8_t*>( riffHeader ) + sizeof(RIFFChunkHeader);
    if ( ( ptr + sizeof(RIFFChunk) ) > wavEnd )
    {
        return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
    }

    auto dataChunk = FindChunk( ptr, riffChunk->size, FOURCC_DATA_TAG );
    if ( !dataChunk || !dataChunk->size )
    {
        return HRESULT_FROM_WIN32( ERROR_INVALID_DATA );
    }

    ptr = reinterpret_cast<const uint8_t*>( dataChunk ) + sizeof( RIFFChunk );
    if ( ptr + dataChunk->size > wavEnd )
    {
        return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
    }

    *pwfx = reinterpret_cast<const WAVEFORMATEX*>( wf );
    *pdata = ptr;
    *dataSize = dataChunk->size;
    return S_OK;
}


//--------------------------------------------------------------------------------------
static HRESULT WaveFindLoopInfo( _In_reads_bytes_(wavDataSize) const uint8_t* wavData, _In_ size_t wavDataSize,
                                 _Out_ uint32_t* pLoopStart, _Out_ uint32_t* pLoopLength )
{
    if ( !wavData || !pLoopStart || !pLoopLength )
        return E_POINTER;

    if (wavDataSize < ( sizeof(RIFFChunk) + sizeof(uint32_t) ) )
    {
        return E_FAIL;
    }

    *pLoopStart = 0;
    *pLoopLength = 0;

    const uint8_t* wavEnd = wavData + wavDataSize;

    // Locate RIFF 'WAVE'
    auto riffChunk = FindChunk( wavData, wavDataSize, FOURCC_RIFF_TAG );
    if ( !riffChunk || riffChunk->size < 4 )
    {
        return E_FAIL;
    }

    auto riffHeader = reinterpret_cast<const RIFFChunkHeader*>( riffChunk );
    if ( riffHeader->riff == FOURCC_XWMA_FILE_TAG )
    {
        // xWMA files do not contain loop information
        return S_OK;
    }

    if ( riffHeader->riff != FOURCC_WAVE_FILE_TAG )
    {
        return E_FAIL;
    }

    // Locate 'wsmp' (DLS Chunk)
    auto ptr = reinterpret_cast<const uint8_t*>( riffHeader ) + sizeof(RIFFChunkHeader);
    if ( ( ptr + sizeof(RIFFChunk) ) > wavEnd )
    {
        return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
    }

    auto dlsChunk = FindChunk( ptr, riffChunk->size, FOURCC_DLS_SAMPLE );
    if ( dlsChunk )
    {
        ptr = reinterpret_cast<const uint8_t*>( dlsChunk ) + sizeof( RIFFChunk );
        if ( ptr + dlsChunk->size > wavEnd )
        {
            return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
        }

        if ( dlsChunk->size >= sizeof(RIFFDLSSample) )
        {
            auto dlsSample = reinterpret_cast<const RIFFDLSSample*>( ptr );

            if ( dlsChunk->size >= ( dlsSample->size + dlsSample->loopCount * sizeof(DLSLoop) ) )
            {
                auto loops = reinterpret_cast<const DLSLoop*>( ptr + dlsSample->size );
                for( uint32_t j = 0; j < dlsSample->loopCount; ++j )
                {
                    if ( ( loops[j].loopType == DLSLoop::LOOP_TYPE_FORWARD || loops[j].loopType == DLSLoop::LOOP_TYPE_RELEASE ) )
                    {
                        // Return 'forward' loop
                        *pLoopStart = loops[j].loopStart;
                        *pLoopLength = loops[j].loopLength;
                        return S_OK;
                    }
                }
            }
        }
    }

    // Locate 'smpl' (Sample Chunk)
    auto midiChunk = FindChunk( ptr, riffChunk->size, FOURCC_MIDI_SAMPLE );
    if ( midiChunk )
    {
        ptr = reinterpret_cast<const uint8_t*>( midiChunk ) + sizeof( RIFFChunk );
        if ( ptr + midiChunk->size > wavEnd )
        {
            return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
        }

        if ( midiChunk->size >= sizeof(RIFFMIDISample) )
        {
            auto midiSample = reinterpret_cast<const RIFFMIDISample*>( ptr );

            if ( midiChunk->size >= ( sizeof(RIFFMIDISample) + midiSample->loopCount * sizeof(MIDILoop) ) )
            {
                auto loops = reinterpret_cast<const MIDILoop*>( ptr + sizeof(RIFFMIDISample) );
                for( uint32_t j = 0; j < midiSample->loopCount; ++j )
                {
                    if ( loops[j].type == MIDILoop::LOOP_TYPE_FORWARD )
                    {
                        // Return 'forward' loop
                        *pLoopStart = loops[j].start;
                        *pLoopLength = loops[j].end + loops[j].start + 1;
                        return S_OK;
                    }
                }
            }
        }
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
static HRESULT WaveFindTable( _In_reads_bytes_(wavDataSize) const uint8_t* wavData, _In_ size_t wavDataSize, _In_ uint32_t tag,
                              _Outptr_result_maybenull_ const uint32_t** pData, _Out_ uint32_t* dataCount )
{
    if ( !wavData || !pData || !dataCount )
        return E_POINTER;

    if (wavDataSize < ( sizeof(RIFFChunk) + sizeof(uint32_t) ) )
    {
        return E_FAIL;
    }

    *pData = nullptr;
    *dataCount = 0;

    const uint8_t* wavEnd = wavData + wavDataSize;

    // Locate RIFF 'WAVE'
    auto riffChunk = FindChunk( wavData, wavDataSize, FOURCC_RIFF_TAG );
    if ( !riffChunk || riffChunk->size < 4 )
    {
        return E_FAIL;
    }

    auto riffHeader = reinterpret_cast<const RIFFChunkHeader*>( riffChunk );
    if ( riffHeader->riff != FOURCC_WAVE_FILE_TAG && riffHeader->riff != FOURCC_XWMA_FILE_TAG )
    {
        return E_FAIL;
    }

    // Locate tag
    auto ptr = reinterpret_cast<const uint8_t*>( riffHeader ) + sizeof(RIFFChunkHeader);
    if ( ( ptr + sizeof(RIFFChunk) ) > wavEnd )
    {
        return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
    }

    auto tableChunk = FindChunk( ptr, riffChunk->size, tag );
    if ( tableChunk )
    {
        ptr = reinterpret_cast<const uint8_t*>( tableChunk ) + sizeof( RIFFChunk );
        if ( ptr + tableChunk->size > wavEnd )
        {
            return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );
        }

        if ( ( tableChunk->size % sizeof(uint32_t) ) != 0 )
        {
            return E_FAIL;
        }

        *pData = reinterpret_cast<const uint32_t*>( ptr );
        *dataCount = tableChunk->size / 4;
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
static HRESULT LoadAudioFromFile( _In_z_ const wchar_t* szFileName, _Inout_ std::unique_ptr<uint8_t[]>& wavData, _Out_ DWORD* bytesRead )
{
    if ( !szFileName )
        return E_INVALIDARG;

    // open the file
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
    ScopedHandle hFile( safe_handle( CreateFile2( szFileName,
                                                  GENERIC_READ,
                                                  FILE_SHARE_READ,
                                                  OPEN_EXISTING,
                                                  nullptr ) ) );
#else
    ScopedHandle hFile( safe_handle( CreateFileW( szFileName,
                                                  GENERIC_READ,
                                                  FILE_SHARE_READ,
                                                  nullptr,
                                                  OPEN_EXISTING,
                                                  FILE_ATTRIBUTE_NORMAL,
                                                  nullptr ) ) );
#endif

    if ( !hFile )
    {
        return HRESULT_FROM_WIN32( GetLastError() );
    }

    // Get the file size
    LARGE_INTEGER FileSize = { 0 };

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
    FILE_STANDARD_INFO fileInfo;
    if ( !GetFileInformationByHandleEx( hFile.get(), FileStandardInfo, &fileInfo, sizeof(fileInfo) ) )
    {
        return HRESULT_FROM_WIN32( GetLastError() );
    }
    FileSize = fileInfo.EndOfFile;
#else
    GetFileSizeEx( hFile.get(), &FileSize );
#endif

    // File is too big for 32-bit allocation, so reject read
    if (FileSize.HighPart > 0)
    {
        return E_FAIL;
    }

    // Need at least enough data to have a valid minimal WAV file
    if (FileSize.LowPart < ( sizeof(RIFFChunk)*2 + sizeof(DWORD) + sizeof(WAVEFORMAT) ) )
    {
        return E_FAIL;
    }

    // create enough space for the file data
    wavData.reset( new (std::nothrow) uint8_t[ FileSize.LowPart ] );
    if (!wavData)
    {
        return E_OUTOFMEMORY;
    }

    // read the data in
    if (!ReadFile( hFile.get(),
                   wavData.get(),
                   FileSize.LowPart,
                   bytesRead,
                   nullptr
                   ))
    {
        return HRESULT_FROM_WIN32( GetLastError() );
    }

    return (*bytesRead < FileSize.LowPart) ? E_FAIL : S_OK;
}


//--------------------------------------------------------------------------------------
_Use_decl_annotations_
HRESULT DirectX::LoadWAVAudioInMemory( const uint8_t* wavData,
                                       size_t wavDataSize,
                                       const WAVEFORMATEX** wfx,
                                       const uint8_t** startAudio,
                                       uint32_t* audioBytes )
{
    if ( !wavData || !wfx || !startAudio || !audioBytes )
        return E_INVALIDARG;

    *wfx = nullptr;
    *startAudio = nullptr;
    *audioBytes = 0;

    // Need at least enough data to have a valid minimal WAV file
    if (wavDataSize < (sizeof(RIFFChunk)*2 + sizeof(DWORD) + sizeof(WAVEFORMAT) ) )
    {
        return E_FAIL;
    }

    bool dpds, seek;
    HRESULT hr = WaveFindFormatAndData( wavData, wavDataSize, wfx, startAudio, audioBytes, dpds, seek );
    if ( FAILED(hr) )
        return hr;

    return (dpds || seek) ? E_FAIL : S_OK;
}


//--------------------------------------------------------------------------------------
_Use_decl_annotations_
HRESULT DirectX::LoadWAVAudioFromFile( const wchar_t* szFileName, 
                                       std::unique_ptr<uint8_t[]>& wavData,
                                       const WAVEFORMATEX** wfx,
                                       const uint8_t** startAudio,
                                       uint32_t* audioBytes )
{
    if ( !szFileName || !wfx || !startAudio || !audioBytes )
        return E_INVALIDARG;

    *wfx = nullptr;
    *startAudio = nullptr;
    *audioBytes = 0;

    DWORD bytesRead = 0;
    HRESULT hr = LoadAudioFromFile( szFileName, wavData, &bytesRead );
    if ( FAILED(hr) )
    {
        return hr;
    }

    bool dpds, seek;
    hr = WaveFindFormatAndData( wavData.get(), bytesRead, wfx, startAudio, audioBytes, dpds, seek );
    if ( FAILED(hr) )
        return hr;

    return (dpds || seek) ? E_FAIL : S_OK;
}


//--------------------------------------------------------------------------------------
_Use_decl_annotations_
HRESULT DirectX::LoadWAVAudioInMemoryEx( const uint8_t* wavData, size_t wavDataSize, DirectX::WAVData& result )
{
    if ( !wavData )
        return E_INVALIDARG;

    memset( &result, 0, sizeof(result) );

    // Need at least enough data to have a valid minimal WAV file
    if (wavDataSize < (sizeof(RIFFChunk)*2 + sizeof(DWORD) + sizeof(WAVEFORMAT) ) )
    {
        return E_FAIL;
    }

    bool dpds, seek;
    HRESULT hr = WaveFindFormatAndData( wavData, wavDataSize, &result.wfx, &result.startAudio, &result.audioBytes, dpds, seek );
    if ( FAILED(hr) )
        return hr;

    hr = WaveFindLoopInfo( wavData, wavDataSize, &result.loopStart, &result.loopLength );
    if ( FAILED(hr) )
        return hr;

    if ( dpds )
    {
        hr = WaveFindTable( wavData, wavDataSize, FOURCC_XWMA_DPDS, &result.seek, &result.seekCount );
        if ( FAILED(hr) )
            return hr;
    }
    else if ( seek )
    {
        hr = WaveFindTable( wavData, wavDataSize, FOURCC_XMA_SEEK, &result.seek, &result.seekCount );
        if ( FAILED(hr) )
            return hr;
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
_Use_decl_annotations_
HRESULT DirectX::LoadWAVAudioFromFileEx( const wchar_t* szFileName, std::unique_ptr<uint8_t[]>& wavData, DirectX::WAVData& result )
{
    if ( !szFileName )
        return E_INVALIDARG;

    memset( &result, 0, sizeof(result) );

    DWORD bytesRead = 0;
    HRESULT hr = LoadAudioFromFile( szFileName, wavData, &bytesRead );
    if ( FAILED(hr) )
    {
        return hr;
    }

    bool dpds, seek;
    hr = WaveFindFormatAndData( wavData.get(), bytesRead, &result.wfx, &result.startAudio, &result.audioBytes, dpds, seek );
    if ( FAILED(hr) )
        return hr;

    hr = WaveFindLoopInfo( wavData.get(), bytesRead, &result.loopStart, &result.loopLength );
    if ( FAILED(hr) )
        return hr;

    if ( dpds )
    {
        hr = WaveFindTable( wavData.get(), bytesRead, FOURCC_XWMA_DPDS, &result.seek, &result.seekCount );
        if ( FAILED(hr) )
            return hr;
    }
    else if ( seek )
    {
        hr = WaveFindTable( wavData.get(), bytesRead, FOURCC_XMA_SEEK, &result.seek, &result.seekCount );
        if ( FAILED(hr) )
            return hr;
    }

    return S_OK;
}

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#else
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\comdecl.h>
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h>
#endif

//--------------------------------------------------------------------------------------
// Helper macros
//--------------------------------------------------------------------------------------
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=nullptr; } }
#endif


//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
_Use_decl_annotations_
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
    bool bFound = false;

    if( !strFilename || strFilename[0] == 0 || !strDestPath || cchDest < 10 )
        return E_INVALIDARG;

    // Get the exe name, and exe path
    WCHAR strExePath[MAX_PATH] = {0};
    WCHAR strExeName[MAX_PATH] = {0};
    WCHAR* strLastSlash = nullptr;
    GetModuleFileName( nullptr, strExePath, MAX_PATH );
    strExePath[MAX_PATH - 1] = 0;
    strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
    if( strLastSlash )
    {
        wcscpy_s( strExeName, MAX_PATH, &strLastSlash[1] );

        // Chop the exe name from the exe path
        *strLastSlash = 0;

        // Chop the .exe from the exe name
        strLastSlash = wcsrchr( strExeName, TEXT( '.' ) );
        if( strLastSlash )
            *strLastSlash = 0;
    }

    wcscpy_s( strDestPath, cchDest, strFilename );
    if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
        return S_OK;

    // Search all parent directories starting at .\ and using strFilename as the leaf name
    WCHAR strLeafName[MAX_PATH] = {0};
    wcscpy_s( strLeafName, MAX_PATH, strFilename );

    WCHAR strFullPath[MAX_PATH] = {0};
    WCHAR strFullFileName[MAX_PATH] = {0};
    WCHAR strSearch[MAX_PATH] = {0};
    WCHAR* strFilePart = nullptr;

    GetFullPathName( L".", MAX_PATH, strFullPath, &strFilePart );
    if( !strFilePart )
        return E_FAIL;

    while( strFilePart && *strFilePart != '\0' )
    {
        swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            wcscpy_s( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            wcscpy_s( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        swprintf_s( strSearch, MAX_PATH, L"%s\\..", strFullPath );
        GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
    }
    if( bFound )
        return S_OK;

    // On failure, return the file as the path but also return an error code
    wcscpy_s( strDestPath, cchDest, strFilename );

    return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}
//--------------------------------------------------------------------------------------

