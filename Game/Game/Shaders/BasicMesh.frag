#version 330

// 頂点シェーダーからテクスチャ座標を入力
in vec2 TexCoords;

// カラーバッファへの出力色
out vec4 outColor;

// テクスチャサンプリングに使用
uniform sampler2D uTexture;

void main()
{
	// テクスチャからのサンプルカラー
    	outColor = texture(uTexture, TexCoords);
}
