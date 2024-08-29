#include "TextureSlotReplace.h"

#include "MMTStringUtils.h"

TextureSlotReplace::TextureSlotReplace() {

}


TextureSlotReplace::TextureSlotReplace(std::vector<std::string> SlotName_Equal_ResourceFileName_List) {
	this->Save_SlotName_Equal_ResourceFileName_List = SlotName_Equal_ResourceFileName_List;

    std::unordered_map<std::wstring, std::wstring> ResourceName_FileName_Map;
    std::vector<std::wstring> slotReplaceStrList;
    for (std::string textureResourceReplace : SlotName_Equal_ResourceFileName_List) {
        std::vector<std::wstring> splitsStrList = MMTString::SplitString(MMTString::ToWideString(textureResourceReplace), L"=");
        std::wstring ReplaceSlot = boost::algorithm::trim_copy(splitsStrList[0]);
        std::wstring ReplaceFileName = boost::algorithm::trim_copy(splitsStrList[1]);
        std::wstring ResourceName = L"Resource_" + MMTString::GetFileNameWithOutSuffix(ReplaceFileName);
        slotReplaceStrList.push_back(ReplaceSlot + L" = " + ResourceName);
        ResourceName_FileName_Map[ResourceName] = ReplaceFileName;
    }

    this->Out_SlotReplaceStrList = slotReplaceStrList;
    this->Out_ResourceName_FileName_Map = ResourceName_FileName_Map;
}


void TextureSlotReplace::SetSerializedList() {
    std::vector<std::string> TmpList;
    for (const auto& pair: this->In_SlotName_ResourceFileName_Map) {
        std::wstring SlotName = pair.first;
        std::wstring ResourceFileName = pair.second;
        std::wstring ReplaceSlotStr = SlotName + L" = " + ResourceFileName;

        TmpList.push_back(MMTString::ToByteString(ReplaceSlotStr));
    }
    this->Save_SlotName_Equal_ResourceFileName_List = TmpList;

}