#pragma once

#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	//指定された名前の頂点/フラグメントシェーダーをロード
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	//これをアクティブなシェーダープログラムとして設定
	void SetActive();
	//行列ユニフォームを設定
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);
	void SetVectorUniform(const char* name, const Vector3& vector);
	// ベクトル値をシェーダー変数nameにセットする
	void SetVector2Uniform(const char* name, const Vector2& vector);
	//floatユニフォームを設定
	void SetFloatUniform(const char* name, float value);
	void SetIntUniform(const char* name, int value);
private:
	//指定されたシェーダーをコンパイル
	bool CompileShader(const std::string& fileName,                               // 読み込まれたシェーダーのコンパイル
		               GLenum shaderType,
		               GLuint& outShader);

	//シェーダーが正常にコンパイルされたかどうかをテスト
	bool IsCompiled(GLuint shader);
	//頂点/フラグメントプログラムがリンクするかどうかをテスト
	bool IsValidProgram();
private:
	//シェーダーオブジェクトIDを保存
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};