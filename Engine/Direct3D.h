#pragma once
#include <d3d11.h>

/// <summary>
///�`��S��
/// </summary>

//�V�F�[�_��؂�ւ��邽�߂̂��
enum SHADER_TYPE
{
	SHADER_2D,
	SHADER_3D,
	SHADER_MAX
};

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Direct3D
{
	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;

	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//simple2D.hlsl�̏�����
	HRESULT InitShader2D();
	//simple3d.hlsl�̏�����
	HRESULT InitShader3D();

	//�V�F�[�_�[����
	HRESULT InitShader();
	void SetShader(SHADER_TYPE type);

	void BeginDraw();
	void EndDraw();
	void Release();
};

