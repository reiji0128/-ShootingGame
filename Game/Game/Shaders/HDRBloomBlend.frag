#version 330 core

// uv座標
in vec2 fragTexCoord;

// 出力色
out vec4 FragColor;

uniform sampler2D uScene;
uniform sampler2D uBloom1;
uniform sampler2D uBloom2;
uniform sampler2D uBloom3;
uniform sampler2D uBloom4;
uniform sampler2D uBloom5;

// 露出
uniform float uExposure;

void main()
{
	vec3 hdrColor    = texture(uScene,fragTexCoord).rgb;
	vec3 bloomColor1 = texture(uBloom1,fragTexCoord).rgb;
	vec3 bloomColor2 = texture(uBloom2,fragTexCoord).rgb;
	vec3 bloomColor3 = texture(uBloom3,fragTexCoord).rgb;
	vec3 bloomColor4 = texture(uBloom4,fragTexCoord).rgb;
	vec3 bloomColor5 = texture(uBloom5,fragTexCoord).rgb;

	hdrColor += bloomColor1 + bloomColor2 + bloomColor3 + bloomColor4 + bloomColor5;

	// 露出トーンマッピング
	vec3 mapped = vec3(1.0) - exp(-hdrColor * uExposure);

	// ガンマコレクション
	const float gamma = 2.2;
	mapped = pow(mapped,vec3(1.0 / gamma));
	FragColor = vec4(mapped,1.0);
}