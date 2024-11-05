//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    //�ϊ��s��A���_�A�����̏��Ȃǂ��܂Ƃ߂đ���
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW; //�@�������[���h���W��
    float4 diffuseColor; //�}�e���A���̐F(�g�U���ˌW��)
    int isTextured; //�e�N�X�`�����\���Ă��邩
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu :����n�܂��̓Z�}���e�B�N�X
    float2 uv : TEXCOORD; //UV���W 
    float4 cos_alpha : COLOR; //�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;
    
    float4 light = float4(1, 1, -1, 0);
    light = normalize(light);
    
    //�@������]
    normal = mul(normal, matW);
    normal = normalize(normal);
    normal.w = 0;
    light.w = 0;
   
    outData.cos_alpha = clamp(dot(normal, light), 0, 1);

	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 Id = { 1.0, 1.0, 1.0, 0.0 }; //���̋��� �f�t�H���g1.0
    float4 Kd = g_texture.Sample(g_sampler, inData.uv); //RGB��->�^���� �e�N�X�`���̐F���T���v�����O/�e�N�X�`��������΃}�e���A���̐F
    float cos_alpha = inData.cos_alpha; //�g�U���ˌW���̌v�Z(�����x�N�g���Ɩʂ̖@����cos)
    float4 ambientSource = { 0.3, 0.3, 0.3, 0.0 }; //�����̋���
    if (isTextured == false)
    {
        return Id * diffuseColor * cos_alpha + Id * diffuseColor * ambientSource;
    }
    else
    {
        return Id * Kd * cos_alpha + Id * Kd * ambientSource;
    }
}

