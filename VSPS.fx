
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float3 vLightPos;
	float4 vLightDir;
	float4 vLightColor;
	float4 vOutputColor;
}


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD0; //텍스쳐 위치 받아옴
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD1;//텍스쳐적용
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Norm = mul( float4( input.Norm, 1 ), World ) - vLightPos;	// 노말벡터를 world좌표로 변환한것
	output.Color = input.Color;
	output.Tex = input.Tex; //텍스쳐적용
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 finalColor = 1.0f;
    
	// 빛의 방향과 정점의 노말벡터를 dot product를해서 빛의 세기를 구함
    finalColor += saturate( 0.2f * dot( input.Norm,(float3)vLightDir) * vLightColor);
   
    finalColor.a = 1;
    return txDiffuse.Sample( samLinear, input.Tex )*input.Color * finalColor;
}


//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid( PS_INPUT input) : SV_Target
{
    return vOutputColor;
}
