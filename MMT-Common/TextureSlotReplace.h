#pragma once

#include <string>
#include <unordered_map>

class TextureSlotReplace {
public:
	std::unordered_map<std::wstring, std::wstring> In_SlotName_ResourceName_Map;
	std::unordered_map<std::wstring, std::wstring> In_SlotName_ResourceFileName_Map;

	//这个方法根据上面两个在贴图探测时生成的内容，自动拼接出用于持久化的SlotName_Equal_ResourceFileName_List
	void SetSerializedList();

	std::vector<std::string> Save_SlotName_Equal_ResourceFileName_List;

	std::vector<std::wstring> Out_SlotReplaceStrList;
	std::unordered_map<std::wstring,std::wstring> Out_ResourceName_FileName_Map;

	TextureSlotReplace();

	//根据持久化读取到的内容，自动拼接成输出时使用的类型
	TextureSlotReplace(std::vector<std::string> Input_SlotName_Equal_ResourceFileName_List);
};