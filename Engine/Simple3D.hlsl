//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    //変換行列、視点、光源の情報などをまとめて送る
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW; //法線をワールド座標に
    float4 diffuseColor; //マテリアルの色(拡散反射係数)
    int isTextured; //テクスチャが貼られているか
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置 :から始まるやつはセマンティクス
    float2 uv : TEXCOORD; //UV座標 
    float4 cos_alpha : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;
    
    float4 light = float4(1, 1, -1, 0);
    light = normalize(light);
    
    //法線を回転
    normal = mul(normal, matW);
    normal = normalize(normal);
    normal.w = 0;
    light.w = 0;
   
    outData.cos_alpha = clamp(dot(normal, light), 0, 1);

	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 Id = { 1.0, 1.0, 1.0, 0.0 }; //光の強さ デフォルト1.0
    float4 Kd = g_texture.Sample(g_sampler, inData.uv); //RGBα->真っ白 テクスチャの色をサンプリング/テクスチャがあればマテリアルの色
    float cos_alpha = inData.cos_alpha; //拡散反射係数の計算(光源ベクトルと面の法線のcos)
    float4 ambientSource = { 0.3, 0.3, 0.3, 0.0 }; //環境光の強さ
    if (isTextured == false)
    {
        return Id * diffuseColor * cos_alpha + Id * diffuseColor * ambientSource;
    }
    else
    {
        return Id * Kd * cos_alpha + Id * Kd * ambientSource;
    }
}

