#pragma once
#include <d3d11.h>

/// <summary>
///描画全般
/// </summary>

//シェーダを切り替えるためのやつ
enum SHADER_TYPE
{
	SHADER_2D,
	SHADER_3D,
	SHADER_MAX
};

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Direct3D
{
	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;

	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//simple2D.hlslの初期化
	HRESULT InitShader2D();
	//simple3d.hlslの初期化
	HRESULT InitShader3D();

	//シェーダー準備
	HRESULT InitShader();
	void SetShader(SHADER_TYPE type);

	void BeginDraw();
	void EndDraw();
	void Release();
};

