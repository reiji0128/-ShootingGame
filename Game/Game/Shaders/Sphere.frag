#version 330 core

layout(location = 0) out vec4 FragBuffer;          // HDRバッファに出力
layout(location = 1) out vec4 BrightBuffer;        // 高輝度バッファに出力

// ライトの色
uniform vec3 color;

// 輝度
uniform float luminance;

out vec4 outColor;

void main()
{
	vec3 result;
	result = color * luminance;

	// ピクセルの輝度を求める
	float brightness = dot(result,vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.8)
	{
		BrightBuffer = vec4(result, 0.0f);
	}
	else
	{
		BrightBuffer = vec4(0.0f,0.0f,0.0f,0.0f);
	}

	FragBuffer = vec4(result,1.0);
}