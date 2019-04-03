
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
	float2 Tex : TEXCOORD0; //�ؽ��� ��ġ �޾ƿ�
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD1;//�ؽ�������
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
    output.Norm = mul( float4( input.Norm, 1 ), World ) - vLightPos;	// �븻���͸� world��ǥ�� ��ȯ�Ѱ�
	output.Color = input.Color;
	output.Tex = input.Tex; //�ؽ�������
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 finalColor = 1.0f;
    
	// ���� ����� ������ �븻���͸� dot product���ؼ� ���� ���⸦ ����
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
