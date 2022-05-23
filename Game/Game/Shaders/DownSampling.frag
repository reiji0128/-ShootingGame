#version 330 core

// uvç¿ïW
in vec2 fragTexCoord;

// èoóÕêF
out vec4 FragColor;

uniform sampler2D uScreenTexture;

void main()
{
	ivec2 size = textureSize(uScreenTexture,0);
	vec2 texOffset = vec2(1.0f / size);

	vec3 c1 = vec3(texture(uScreenTexture, fragTexCoord + texOffset * vec2(1.0f,1.0f)));
	vec3 c2 = vec3(texture(uScreenTexture, fragTexCoord + texOffset * vec2(-1.0f,1.0f)));
	vec3 c3 = vec3(texture(uScreenTexture, fragTexCoord + texOffset * vec2(1.0f,-1.0f)));
	vec3 c4 = vec3(texture(uScreenTexture, fragTexCoord + texOffset * vec2(-1.0f,-1.0f)));
	vec3 c5 = vec3(texture(uScreenTexture, fragTexCoord + texOffset * vec2(0.0f,0.0f)));

	FragColor = vec4((c1 + c2 + c3 + c4 + c5) * 0.2f,1.0f);
}