#pragma once

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


//JSONファイルのオープン
bool openJsonFile(rapidjson::Document &inoutDoc, const char* inFileName);

// Doc内にそのメンバーは存在するか？
bool IsExistMember(const rapidjson::Document& inDoc, const char* memberName);

// そのメンバーが存在し、かつ値が一致するか？
bool IsExistMemberAndValue(const rapidjson::Document& inDoc, const char* memberName, const char* matchValue);

// Doc中に memberNameという名前の配列はあるか？
bool IsExistArrayName(const rapidjson::Document &inDoc, const char* memberName);

// Value値を強制的にFloat値として取得
float ForceGetFloat(const rapidjson::Value& val);

