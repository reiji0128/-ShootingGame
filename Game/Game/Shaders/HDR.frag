#version 330 core

// uv座標
in vec2 fragTexCoord;

// 出力カラー
out vec4 outColor;

uniform sampler2D uHDRBuffer;
// 露出
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(uHDRBuffer,fragTexCoord).rgb;

	// 露出トーンマッピング
	vec3 mapped = vec3(1.0) -exp(-hdrColor * exposure);
	// ガンマコレクション
	mapped = pow(mapped,vec3(1.0 / gamma));

	outColor = vec4(mapped,1.0);
}
