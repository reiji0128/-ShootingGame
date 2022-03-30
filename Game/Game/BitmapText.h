#pragma once
#include <string>

class BitmapText
{
public:
	BitmapText();
	~BitmapText();

	void SetFontImage(int xNum, int yNum, const std::string& fontimgfilename);	// フォント画像よりフォントをセットする
	void SetFontColor(int r, int g, int b);										// フォントカラーセット

	void TextDraw(int x, int y, const char* str);								// テキストを描画

	void ReMapText(const char* remapText);										// ビットマップに並んでいる文字列を入れて、テキストから文字列を作れるようにする。（初期化で必要）

private:
	int mFontMap[256];			// 文字マップ char型文字→フォントインデックスのマッピング

	unsigned int mSplitXnum;	//文字マップ横文字数
	unsigned int mSplitYnum;	//文字マップ縦文字数
	unsigned int   mSizeX;      // 1文字分サイズx
	unsigned int   mSizeY;      // 1文字分サイズy
	bool           mUseflag;    // フォント使用可能フラグ

	int            GetIndex(char c); // 文字をイメージ配列の添え字に変換
	class Texture* mTexture;
};