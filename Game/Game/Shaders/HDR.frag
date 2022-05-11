#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D uHDRBuffer;

void main()
{
	const float gammma = 2.2;
	vec3 hdrColor = texture(uDHRBuffer,TexCoords).rgb;

	// reinhard�g�[���}�b�s���O
	vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
	// �K���}�R���N�V����
	mapped = pow(mapped,vec3(1.0 / gamma));

	FragColor = vec4(mapped,1.0);
}
