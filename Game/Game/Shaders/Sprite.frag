#version 330

// 頂点シェーダーからテクスチャ座標を入力
in vec2 fragTexCoord;

// カラーバッファへの出力色
out vec4 outColor;

// テクスチャサンプリングに使用
uniform sampler2D uTexture;
uniform float alpha;

void main()
{
	// テクスチャからのサンプルカラー
	vec4 color = texture(uTexture,fragTexCoord);
	color.a = color.a * alpha;
	outColor = color;
}