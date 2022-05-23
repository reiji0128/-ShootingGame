#version 330 core

// uv座標
in vec2 fragTexCoord;

// 出力色
out vec4 FragColor;

struct GaussBlurParam
{
	int SampleCount;
	vec3 Offset[16];
};

uniform GaussBlurParam uParam;

uniform sampler2D uBlursource;



void main()
{
	vec4 result = vec4(0.0f);
	for(int i = 0; i < uParam.SampleCount;i++)
	{
	   // ガウスの重み(uParam.Offset[i])に基づいてテクスチャカラーを合成		
       result += uParam.Offset[i].z * texture(uBlursource,fragTexCoord + uParam.Offset[i].xy);  
	}
	result.w = 1.0f;
	FragColor = result;
}