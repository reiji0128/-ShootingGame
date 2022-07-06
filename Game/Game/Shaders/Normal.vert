#version 330 core

layout (location = 0) in vec3 aPos;       // 頂点座標
layout (location = 1) in vec3 aNormal;    // 法線
layout (location = 2) in vec2 aTexCoords; // uv座標
layout (location = 3) in vec3 aTangent;   // 従法線

// ポイントライト
struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 dpecular;
};

out VS_OUT
{
    PointLight Light;
    vec2 TexCoords;
    vec3 FragPos;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
}vx_out;

uniform PointLight in_light;

uniform mat4 model;      // モデル行列
uniform mat4 view;       // ビュー行列
uniform mat4 projection; // プロジェクション行列
uniform vec3 viewPos;    // カメラの座標

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos,1.0));
    vs_out.TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = normalize(normalMatrix * aTangent);
    T = normalize(T - dot(T,N) * N);
    vec3 B = cross(N,T);

    mat3 TBN = transpose(mat3(T,B,N));

    // ライトのタンジェント空間での位置
    vs_out.Light = in_light;
    vs_out.TangentLightPos = TBN * in_light.position;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TagngetFragPos = TBN * vs_out.FragPos;

    gl_Position = projection * view * model * vec4(aPos,1.0);
}