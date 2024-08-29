#include "D3D11Texture.h"

#include "MMTLogUtils.h"
TextureType::TextureType() {

}



bool TextureType::TimeInPsSetShaderResourceTimeList(int time) {
    for (int num : this->PsSetShaderResourceTimeList) {
        LOG.Info(L"Target Time: " + std::to_wstring(time) + L"  CurrentTime:" + std::to_wstring(num));
        if (num == time) {
            return true;
        }
    }
    return false;
}


uint32_t TextureType::GetMeaningfulDdsFileCount() {
    uint32_t count = 0;
    for (const auto& pair: this->PixelSlot_TextureType_Map) {
        if (pair.second != "NicoMico") {
            count++;
        }
    }
    return count;
}


std::string TextureType::GetDiffuseMapSlot() {
    for (const auto& slotPair : this->PixelSlot_TextureType_Map) {
        std::string slot = slotPair.first;
        std::string textureFileName = slotPair.second;
        if (textureFileName == "DiffuseMap.dds") {
            return slot;
        }
    }
    return "";
}
