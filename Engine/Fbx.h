#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Texture.h"
#include <vector>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

/// <summary>
/// Fbx�Ɋւ���N���X
/// </summary>

struct RayCastData
{
	XMFLOAT3 start; //���C�̔��ˈʒu
	XMFLOAT3 dir; //����
	bool hit; //�����������ǂ���
	float dist; //�����������
};

class Fbx
{
	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	//�R���X�^���g�o�b�t�@�[ �A�v��������V�F�[�_�[�ɖ��t���[���n���������
	struct CONSTANT_BUFFER
	{
		XMMATRIX matWVP; //���[���h�r���[�v���W�F�N�V����
		XMMATRIX matNormal; //�@���x�N�g���̕ϊ��p
		XMFLOAT4 diffuseColor; //�}�e���A���̐F(�g�U���ˌW��)
		int isTextured; //�e�N�X�`�����\���Ă��邩
	};

	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
	};

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_; //�}�e���A���̌�
	std::vector<MATERIAL> pMaterialList_; //�}�e���A���̃��X�g
	std::vector<int> indexCount_; //�C���f�b�N�X�̌�

	std::vector<VERTEX> vertices_; //���_��������z��
	ID3D11Buffer* pVertexBuffer_; //���_�o�b�t�@
	std::vector<int>* index_; //�C���f�b�N�X��������z��
	ID3D11Buffer** pIndexBuffer_; //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_; //�R���X�^���g�o�b�t�@
	void InitVertex(fbxsdk::FbxMesh* mesh); //���_�o�b�t�@����
	void InitIndex(fbxsdk::FbxMesh* mesh); //�C���f�b�N�X�o�b�t�@����
	void InitConstantBuffer(); //�R���X�^���g�o�b�t�@����
	void InitMaterial(fbxsdk::FbxNode* pNode); //�}�e���A������
public:
	Fbx();
	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void Release();
	void RayCast(RayCastData& rayData, Transform& transform); //���C�L���X�g�p
};

