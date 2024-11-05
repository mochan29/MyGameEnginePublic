#include "Fbx.h"
#include "Camera.h"
#include <filesystem>
#include <DirectXCollision.h>
#include "Define.h"

namespace fs = std::filesystem;

Fbx::Fbx()
{
	vertexCount_ = -1;
	polygonCount_ = -1;
	materialCount_ = -1;

	index_ = nullptr;

	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
}

//���[�h���Ă��낢�돉����
HRESULT Fbx::Load(std::string fileName)
{
	//�}�l�[�W���𐶐�
	FbxManager* pFbxManager = FbxManager::Create();

	//�C���|�[�^�[�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//�V�[���I�u�W�F�N�g��Fbx�t�@�C���̏��𗬂�����
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//���b�V�������擾
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//�e���̌����擾
	vertexCount_ = mesh->GetControlPointsCount();
	polygonCount_ = mesh->GetPolygonCount();
	materialCount_ = pNode->GetMaterialCount();

	//�p�X�����
	fs::path cPath, basePath;
	cPath = fs::current_path();
	basePath = cPath;
	string subDir("Assets\\Models");
	fs::path subPath(cPath.string() + "\\" + subDir);
	assert(fs::exists(subPath));
	fs::current_path(subPath);

	InitVertex(mesh);
	InitIndex(mesh);
	InitConstantBuffer();
	InitMaterial(pNode);

	//�J�����g�p�X��߂�
	fs::current_path(basePath);

	//�}�l�[�W�����
	pFbxManager->Destroy();
	return S_OK;
}

void Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//���_��������z��̏�����
	vertices_ = std::vector<VERTEX>(vertexCount_);

	//�S�|���S��
	for (DWORD poly = 0; poly < (DWORD)polygonCount_; poly++)
	{
		//3���_��
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//���ׂ钸�_�̔ԍ�
			int index = mesh->GetPolygonVertex(poly, vertex);

			//���_�̈ʒu
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices_[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//���_�Ɋ��蓖�Ă�ꂽUV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices_[index].uv = XMVectorSet((float)(1.0 - uv.mData[0]), (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//���_�̖@��
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//i�Ԗڂ̃|���S����,j�Ԗڂ̒��_�̖@�����Q�b�g
			vertices_[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}
	//���_�o�b�t�@�쐬
	HRESULT hr;
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"���_�o�b�t�@�̍쐬�Ɏ��s", L"Fbx.cpp", MB_OK);
	}
}

void Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];
	indexCount_ = std::vector<int>(materialCount_);
	index_ = new std::vector<int>[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		index_[i] = std::vector<int>(polygonCount_ * 3);
		int count = 0;
		//�S�|���S��
		for (DWORD poly = 0; poly < (DWORD)polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);
			if (mtlId == i)
			{
				//3���_��
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					index_[i][count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}
		}
		indexCount_[i] = count;

		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index_[i].data();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr;
		hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s", L"Fbx.cpp", MB_OK);
		}
	}
}

void Fbx::InitConstantBuffer()
{
	//Quad�ƈꏏ
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s", L"Fbx.cpp", MB_OK);
	}
}

void Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = std::vector<MATERIAL>(materialCount_);
	for (int i = 0; i < materialCount_; i++)
	{
		//i�Ԗڂ̃}�e���A�������擾
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		//�e�N�X�`�����
		FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//�e�N�X�`���̐���
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//�e�N�X�`������
		if (fileTextureCount)
		{
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			//�p�X�����
			fs::path texFile(textureFilePath);


			//�����ő��݃`�F�b�N���K�v�i���Ƃł�낤�j
			if (fs::is_regular_file(texFile))
			{
				pMaterialList_[i].pTexture = new Texture;
				pMaterialList_[i].pTexture->Load(texFile.string());
			}
			else
			{
			}
		}
		//�e�N�X�`������
		else
		{
			pMaterialList_[i].pTexture = nullptr;
			//�}�e���A���̐F���Ƃ��Ă���	
			//�g�U���ˌW�����Ƃ��Ă��Ă�����
			FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			FbxDouble3  diffuse = pMaterial->Diffuse;
			pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		}
	}
}

void Fbx::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_3D);
	transform.Calculation();

	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());

	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	for (int i = 0; i < materialCount_; i++)
	{
		cb.diffuseColor = pMaterialList_[i].diffuse;
		if (pMaterialList_[i].pTexture == nullptr)
		{
			cb.isTextured = false;
		}
		else
		{
			cb.isTextured = true;
		}

		D3D11_MAPPED_SUBRESOURCE pdata;
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); //GPU����̃f�[�^�A�N�Z�X���~�߂�
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb)); //�f�[�^��l�𑗂�

		Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//�ĊJ

		//���_�o�b�t�@,�C���f�b�N�X�o�b�t�@,�R���X�^���g�o�b�t�@���p�C�v���C���ɃZ�b�g
		//���_�o�b�t�@
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//�R���X�^���g�o�b�t�@
		Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
		Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

		if (!(pMaterialList_[i].pTexture == nullptr))
		{
			//�T���v���[�ƃV�F�[�_�[���\�[�X�r���[���V�F�[�_�ɃZ�b�g
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

		}
		//�`��
		Direct3D::pContext->DrawIndexed(indexCount_[i], 0, 0);
	}
}

void Fbx::Release()
{
	SAFE_RELEASE(pConstantBuffer_);
	for (int i = 0; i < materialCount_; i++)
	{
		SAFE_RELEASE(pIndexBuffer_[i]); //���g����
	}
	SAFE_DELETE_ARRAY(pIndexBuffer_); //�{�̏���
	SAFE_RELEASE(pVertexBuffer_);
}

void Fbx::RayCast(RayCastData& rayData,Transform& transform)
{
	//���[���h�s��̋t�s����g���ă��C�����f���Ƌt�����ɓ�����->���̂ق����y��
	transform.Calculation();
	XMMATRIX invWorld = XMMatrixInverse(nullptr, transform.GetWorldMatrix());

	XMVECTOR start = XMLoadFloat3(&rayData.start); //���C�̔��ˈʒu
	XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&rayData.dir)); //����(���K���ς�)

	//���[�J�����W�̂܂܂Ȃ̂Ń��[���h�s��������ă��[���h���W��
	//���C�𓮂������Ƃ��̐V���Ȍ��������߂�
	XMVECTOR end = start + dir;
	start = XMVector3TransformCoord(start, invWorld);
	end = XMVector3TransformCoord(end, invWorld);
	dir = end - start; //���f������]�����Ƃ��ɂ�dir���ς��

	//�}�e���A����
	for (int material = 0; material < materialCount_; material++)
	{
		//�|���S���� ���_���̐擪�������Ă���
		for (int poly = 0; poly < indexCount_[material] / 3; poly++)
		{
			XMVECTOR v0 = vertices_[index_[material][poly * 3 + 0]].position; //3���_ poly*3=�|���S�����̐擪���w�� 3��1�Z�b�g
			XMVECTOR v1 = vertices_[index_[material][poly * 3 + 1]].position;
			XMVECTOR v2 = vertices_[index_[material][poly * 3 + 2]].position;

			rayData.hit = TriangleTests::Intersects(start, dir, v0, v1, v2, rayData.dist); //���C�̔��ˈʒu�A�����A3���_�A�����������
			if (rayData.hit)
			{
				return;
			}
		}
	}
}