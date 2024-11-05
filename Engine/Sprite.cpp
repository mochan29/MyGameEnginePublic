#include "Sprite.h"
#include "Camera.h"
#include <filesystem>
#include "Define.h"

Sprite::Sprite()
{
	vertexNum_ = 0;
	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
	pTexture_ = nullptr;
	indexNum_ = 0;
}

HRESULT Sprite::Load(std::string fileName)
{
	//頂点情報/頂点バッファ作成
	InitVertexData();
	if (FAILED(CreateVertexBuffer()))
	{
		MessageBox(nullptr, L"失敗: 頂点バッファ", L"Sprite.cpp", MB_OK);
		return E_FAIL;
	}

	//インデックス情報/インデックスバッファ作成
	InitIndexData();
	if (FAILED(CreateIndexBuffer()))
	{
		MessageBox(nullptr, L"失敗: インデックスバッファ", L"Sprite.cpp", MB_OK);
		return E_FAIL;
	}

	//コンスタントバッファ作成
	if (FAILED(CreateConstantBuffer()))
	{
		MessageBox(nullptr, L"失敗: コンスタントバッファ", L"Sprite.cpp", MB_OK);
		return E_FAIL;
	}

	//テクスチャのロード
	if (FAILED(LoadTexture(fileName)))
	{
		MessageBox(nullptr, L"失敗: テクスチャのロード", L"Sprite.cpp", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void Sprite::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_TYPE::SHADER_2D);

	//ワールドマトリクスを計算
	transform.Calculation();

	//コンスタントバッファに情報を渡す
	PassDataToCB(transform.GetWorldMatrix());
	//頂点バッファ、インデックスバッファ、コンスタントバッファをパイプラインにわたす
	SetBufferToPipeline();
	//頂点インデックスを送る、描画
	Direct3D::pContext->DrawIndexed(indexNum_, 0, 0);
}

void Sprite::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_DELETE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Sprite::InitVertexData()
{
	//頂点データ
	vertices_ =
	{ //頂点、UV
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) }, //左上
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) }, //右上
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f)}, //右下
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) }, //左下
	};
	vertexNum_ = vertices_.size();
}

HRESULT Sprite::CreateVertexBuffer()
{
	HRESULT hr;

	//頂点データ用バッファ設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_; //バッファのサイズ(byte)
	bd_vertex.Usage = D3D11_USAGE_DEFAULT; //読み込み・書き込みの方法
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER; //バインドに関するやつ
	bd_vertex.CPUAccessFlags = 0; //CPUアクセスに関するやつ
	bd_vertex.MiscFlags = 0; //いろんなフラグ
	bd_vertex.StructureByteStride = 0; //構造体のサイズ(byte)
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data(); //配列のアドレス
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"失敗: 頂点バッファ", L"Sprite.cpp", MB_OK);
		return hr;
	}

	return hr;
}

void Sprite::InitIndexData()
{
	//インデックス情報
	indices_ = { 0,2,3,0,1,2, };
	indexNum_ = indices_.size();
}

HRESULT Sprite::CreateIndexBuffer()
{
	HRESULT hr;
	//インデックスバッファ生成
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices_.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"失敗: インデックスバッファ", L"Sprite.cpp", MB_OK);
		return hr;
	}
	return hr;
}

HRESULT Sprite::CreateConstantBuffer()
{
	HRESULT hr;

	//コンスタントバッファ設定
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	//コンスタントバッファ作成
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"失敗: コンスタントバッファ", L"Sprite.cpp", MB_OK);
		return hr;
	}
	return hr;
}

HRESULT Sprite::LoadTexture(std::string fileName)
{
	HRESULT hr;

	//テクスチャ生成
	namespace fs = std::filesystem;
	pTexture_ = new Texture;
	assert(fs::is_regular_file(fileName));

	hr = pTexture_->Load(fileName);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"失敗: テクスチャ作成", L"Sprite.cpp", MB_OK);
		return hr;
	}
	return hr;
}

void Sprite::PassDataToCB(DirectX::XMMATRIX worldMatrix)
{
	CONSTANT_BUFFER cb;
	cb.matW = XMMatrixTranspose(worldMatrix); //Matrixのかけ算をDirectX仕様に
	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); //GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb)); //データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_, 0); //再開
}

void Sprite::SetBufferToPipeline()
{
	//頂点バッファセット
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//インデックスバッファーセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファセット
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_); //頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_); //ピクセルシェーダー用

	//サンプラーとシェーダーリソースビューをシェーダにセット
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
}
