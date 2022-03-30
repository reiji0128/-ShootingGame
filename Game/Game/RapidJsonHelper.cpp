#include "RapidJsonHelper.h"

//////////////////////////////////////////////////////////////////////////
// jsonファイルのオープン 
// in    : inFileName : オープンしたいjsonファイルパス
// inout : inoutDoc   : rapidjson オブジェクト
// オープンできた時は trueを返し、inoutDocに　rapidjsonDocumentを参照できる
//////////////////////////////////////////////////////////////////////////
bool openJsonFile(rapidjson::Document & inoutDoc, const char * inFileName)
{
	// jsonファイル開けるか？
	std::ifstream file(inFileName);
	if (!file.is_open())
	{
		return false;
	}

	// filestreamにファイル文字列取り込む
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();

	// ファイル文字列をJSon文字列に変換
	rapidjson::StringStream jsonStr(contents.c_str());
	// ファイルストリーム閉じる
	file.close();

	// 解析オブジェクト作成
	inoutDoc.ParseStream(jsonStr);

	// JSonObjectとして読めたか？
	if (!inoutDoc.IsObject())
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Doc内にそのメンバーは存在するか？
// in : inDoc      rapidjson Document
//    : memberName メンバー名
// 戻り値　Document内に memberNameが存在したときのみtrueを返す。
///////////////////////////////////////////////////////////////////////////////
bool IsExistMember(const rapidjson::Document & inDoc, const char * memberName)
{
	rapidjson::Value::ConstMemberIterator iter;
	iter = inDoc.FindMember(memberName);

	if (iter == inDoc.MemberEnd())
	{
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////
// そのメンバーが存在し、かつ値が一致するか？
// in    :  inDoc       rapidjsonドキュメント
//          memberName  調べたいメンバー名
//          matchValue   調べたいメンバー名にマッチする値
// 戻り値 : memberNameに対応する値がmatchValueだったときのみtrueを返す 
/////////////////////////////////////////////////////////////////////////////////
bool IsExistMemberAndValue(const rapidjson::Document & inDoc, const char * memberName, const char * matchValue)
{
	if (!IsExistMember(inDoc, memberName))
	{
		return false;
	}

	std::string findValueString;
	findValueString = std::string(matchValue);

	if (findValueString != inDoc[memberName].GetString())
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////
// Doc中に memberNameという名前の配列はあるか？
//////////////////////////////////////////////////////////////////////////
bool IsExistArrayName(const rapidjson::Document & inDoc, const char * memberName)
{
	rapidjson::Value::ConstMemberIterator iter;
	iter = inDoc.FindMember(memberName);
	if (iter == inDoc.MemberEnd())
	{
		return false;
	}
	return inDoc[memberName].IsArray();
}

// JSON値を内容がintでも強制的にfloat 値として取得
float ForceGetFloat(const rapidjson::Value& val)
{
	std::string tmp;
	tmp = val.GetString();

	return atof(tmp.c_str());
}
