#version 330 core

// 頂点シェーダーからuv座標を入力
in vec2 fragTexCoord;

// カラーバッファーへの出力
out vec4 outColor;

// 浮動小数点数バッファ
uniform sampler2D uHDRBuffer;

// 露出
uniform float exposure;

void main()
{
	vec3 hdrColor = texture(uHDRBuffer,fragTexCoord).rgb;
	outColor = vec4(hdrColor,1.0);
}