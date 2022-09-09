#version 330 core

uniform vec3 lightColor;

// カラーバッファへの出力色
out vec4 outColor;

void main()
{           
    outColor = vec4(lightColor, 1.0);
}