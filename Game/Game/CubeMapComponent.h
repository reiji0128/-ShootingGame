#pragma once
#include "Component.h"

class Shader;

class CubeMapComponent : public Component
{
public:
	CubeMapComponent(Actor* owner);
	~CubeMapComponent();

	void CreateCubeMap();

	void Draw(Shader* shader);

// �Z�b�^�[�@//

	/// <summary>
	/// �\���t���O�̃Z�b�g
	/// </summary>
	/// <param name="visible">
	/// true  : �\������
	/// false : �\�����Ȃ�
	/// </param>
	void SetVisible(bool visible) { mVisible = visible;}

// �Q�b�^�[ //

	/// <summary>
	/// �\���t���O�̎擾
	/// </summary>
	/// <returns>
	/// true  : �\������
	/// false : �\�����Ȃ�
	/// </returns>
	bool GetVisible() const { return mVisible; }

private:
	// �\���t���O
	bool mVisible;
};