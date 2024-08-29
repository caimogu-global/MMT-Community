#include "Functions_Basic.h"

#include "GlobalConfigs.h"

#include <set>

void Functions_Basic::MoveAllUsedTexturesToOutputFolder(std::wstring DrawIB, std::wstring OutputFolder) {
	LOG.Info(L"移动所有用到的贴图文件:");
	FrameAnalysisData FAData = G.GetFrameAnalysisData();
	std::wstring TrianglelistTexturesFolderPath = OutputFolder + L"TrianglelistTextures\\";
	std::wstring DedupedTexturesFolderPath = OutputFolder + L"DedupedTextures\\";
	std::filesystem::create_directories(DedupedTexturesFolderPath);
	std::filesystem::create_directories(TrianglelistTexturesFolderPath);

	for (std::wstring index : FAData.ReadTrianglelistIndexList(DrawIB)) {
		std::vector<std::wstring> pixelShaderTextureAllFileNameList = MMTFile::FindTextureFileList(G.WorkFolder, index + L"-ps-t");
		for (std::wstring psTextureFileName : pixelShaderTextureAllFileNameList) {
			std::filesystem::copy_file(G.WorkFolder + psTextureFileName, TrianglelistTexturesFolderPath + psTextureFileName, std::filesystem::copy_options::skip_existing);
			
			std::wstring DedupedFileName = FAData.FindDedupedTextureName(G.WorkFolder, psTextureFileName);
			if (DedupedFileName != L"") {
				std::filesystem::copy_file(G.WorkFolder + psTextureFileName, DedupedTexturesFolderPath + DedupedFileName, std::filesystem::copy_options::skip_existing);
			}
		}
	}
	LOG.NewLine();
}