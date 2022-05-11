#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D uHDRBuffer;

void main()
{
	const float gammma = 2.2;
	vec3 hdrColor = texture(uDHRBuffer,TexCoords).rgb;

	// reinhardトーンマッピング
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
	// ガンマコレクション
	mapped = pow(mapped,vec3(1.0 / gamma));

	FragColor = vec4(mapped,1.0);
}
