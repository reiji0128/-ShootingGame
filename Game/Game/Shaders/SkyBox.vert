#version 330 core

layout (location = 0) in vec3 aPos;

// 出力するテクスチャ座標
out vec3 TexCoords;

// プロジェクション行列
uniform mat4 uProjection;

// ビュー行列
uniform mat4 uView;

uniform mat4 uOffset;


void main()
{
	vec4 pos =  vec4(aPos,1.0f)*uOffset* uView*uProjection ;

	// 深度バッファはz座標を深度値として受け取るためスカイボックスの深度値を常に1にする
	gl_Position = vec4(pos.x,pos.y,pos.w,pos.w);

	// 座標系が違うためz軸を反転
	TexCoords = vec3(aPos.x,aPos.y,-aPos.z);
}