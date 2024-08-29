#include "Functions_HSR.h"

#include "GlobalConfigs.h"
#include "VertexBufferTxtFileDetect.h"
#include "IndexBufferTxtFile.h"
#include "FmtFile.h"
#include "FrameAnalysisData.h"
#include "IndexBufferBufFile.h"
#include "VertexBufferBufFile.h"

#include "MMTConstants.h"

void Functions_HSR::ExtractModel() {
    FrameAnalysisData FAData = G.GetFrameAnalysisData();

    for (const auto& pair : G.DrawIB_ExtractConfig_Map) {

        std::wstring DrawIB = pair.first;
        DrawIBConfig extractConfig = pair.second;
        //配置初始化区域
        std::wstring OutputDrawIBFolder = G.OutputFolder + DrawIB + L"\\";
        std::filesystem::create_directories(OutputDrawIBFolder);

        if (extractConfig.GameType == L"Auto") {
            extractConfig.GameType = AutoGameType(DrawIB);
            if (extractConfig.GameType == L"") {
                LOG.Error(MMT_Tips::TIP_AutoGameType_NoGameTypeDetected);
            }
        }

        D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[MMTString::ToByteString(extractConfig.GameType)];
        LOG.Info("GameType: " + d3d11GameType.GameType);

        extractConfig.WorkGameType = MMTString::ToByteString(extractConfig.GameType);

        //统计所有的FrameAnalysis文件名
        //首先通过txt文件来找到pointlist和trianglelist的索引
        std::map<std::wstring, uint32_t> PointlistIndexVertexCountMap = FAData.Read_PointlistIndex_VertexCount_Map(VertexCountType::BufFileTotal);
        std::map<std::wstring, uint32_t> TrianglelistIndexVertexCountMap = FAData.Read_TrianglelistIndex_VertexCount_Map(VertexCountType::BufFileRemoveNull,DrawIB);
        std::vector<std::wstring> TrianglelistIndices = FAData.ReadTrianglelistIndexList(DrawIB);

        uint32_t TranglelistMaxVertexCount = 0;
        for (const auto& TrianglelistIndexPair: TrianglelistIndexVertexCountMap) {
            if (TrianglelistIndexPair.second > TranglelistMaxVertexCount) {
                TranglelistMaxVertexCount = TrianglelistIndexPair.second;
            }
        }
        LOG.Info(L"TranglelistMaxVertexCount: " + std::to_wstring(TranglelistMaxVertexCount));
        LOG.NewLine();

        LOG.Info(L"PointlistIndexVertexNumberMap:");
        std::wstring PointlistExtractIndex = L"";
        for (const auto& pair : PointlistIndexVertexCountMap) {
            std::wstring index = pair.first;
            uint64_t vertexCount = pair.second;
            LOG.Info(L"Index: " + index + L", VertexCount: " + std::to_wstring(vertexCount));
            if (vertexCount == TranglelistMaxVertexCount) {
                PointlistExtractIndex = index;
                break;
                //只匹配第一个遇到的就行了，和AutoGameType里保持一致
            }
        }
        LOG.Info(L"max vertex count pointlist index: " + PointlistExtractIndex);
        LOG.NewLine();

        // 遍历和输出trianglelistIndexVertexNumberMap
        LOG.Info(L"TrianglelistIndexVertexNumberMap:");
        for (const auto& pair : TrianglelistIndexVertexCountMap) {
            LOG.Info(L"Index:" + pair.first + L", VertexCount: " + std::to_wstring(pair.second));
        }
        LOG.NewLine();


        int MaxTrianglelistStride = 0;
        std::wstring TrianglelistExtractIndex = L"";
        std::wstring TexcoordExtractSlot = MMTString::ToWideString(d3d11GameType.ElementNameD3D11ElementMap["TEXCOORD"].ExtractSlot);
        LOG.Info(L"TexcoordExtractSlot: " + TexcoordExtractSlot);

        for (const auto& pair : TrianglelistIndexVertexCountMap) {
            std::wstring Index = pair.first;
            std::vector<std::wstring> filenames = FAData.FindFrameAnalysisFileNameListWithCondition(Index + L"-" + TexcoordExtractSlot, L".txt");
            if (filenames.size() == 0) {
                LOG.Info(Index + L"'s slot " + TexcoordExtractSlot + L" can't find. skip this.");
                continue;
            }
            std::wstring filename = filenames[0];
            VertexBufferDetect vbDetect(G.WorkFolder + filename);
            if (vbDetect.fileRealStride > MaxTrianglelistStride) {
                MaxTrianglelistStride = vbDetect.fileRealStride;
                //TODO 这里判断展示的和Buffer的相等才能添加，导致崩坏三无法添加
                //那么去掉可不可以呢？反正提取的时候是从Buffer提取，和展示的顶点数量无关
                //我觉得应该是Buffer和最大值相等才能提取
                if (vbDetect.fileBufferVertexCount == TranglelistMaxVertexCount) {
                    TrianglelistExtractIndex = Index;
                }
            }
        }
        LOG.Info(L"MaxTrianglelistStride: " + std::to_wstring(MaxTrianglelistStride));
        LOG.Info(L"TrianglelistExtractIndex: " + TrianglelistExtractIndex);
        if (TrianglelistExtractIndex == L"") {
            //原神动态高精度需要单独提示
            if (G.GameName == L"GI") {
                LOG.Error("Can't find TrianglelistExtract Index! Do you forget to turn off [Dynamic Character Resolution] in your graphic setting?");
            }
            else {
                LOG.Error("Can't find TrianglelistExtract Index!");
            }
        }

        std::string VertexLimitVB = MMTString::ToByteString(FAData.FindFrameAnalysisFileNameListWithCondition(TrianglelistExtractIndex + L"-vb0", L".txt")[0].substr(11, 8));
        extractConfig.VertexLimitVB = VertexLimitVB;
        LOG.NewLine();

        //根据Category直接从对应索引提取对应Buffer文件
        LOG.Info("Extract from Buffer file: ");
        std::vector<std::unordered_map<uint64_t, std::vector<std::byte>>> categoryFileBufList;
        std::vector<std::string> ElementList;
        std::unordered_map<std::string, std::string> CategoryHashMap;

        //TODO 这里的不一定能保证顺序完全正确
        for (std::string Category: d3d11GameType.OrderedCategoryNameList) {
            std::string CategorySlot = d3d11GameType.CategorySlotMap[Category];
            std::string CategoryTopology = d3d11GameType.CategoryTopologyMap[Category];
            std::wstring ExtractIndex = PointlistExtractIndex;

            if (CategoryTopology == "trianglelist") {
                ExtractIndex = TrianglelistExtractIndex;
            }

            //获取此索引对应槽位的RealStride
            std::vector<std::wstring> filenames = FAData.FindFrameAnalysisFileNameListWithCondition(ExtractIndex + L"-" + MMTString::ToWideString(CategorySlot), L".txt");
            if (filenames.size() == 0) {
                LOG.Error(ExtractIndex + L"'s extract slot " + TexcoordExtractSlot + L" can't find.");
            }
            std::wstring filename = filenames[0];
            CategoryHashMap[Category] = MMTString::ToByteString(filename.substr(11, 8));
            VertexBufferDetect vbDetect(G.WorkFolder + filename);

            //这里使用fileRealStride时，无法保证txt里是真实的数据，所以导致提取失败
            // TODO 找到更加兼容的办法
            //int stride = vbDetect.fileRealStride;

            int stride = vbDetect.fileShowStride;

            bool patchBlendWeights = false;
            ElementList = d3d11GameType.OrderedFullElementList;
            patchBlendWeights = d3d11GameType.PatchBLENDWEIGHTS;


            //获取顶点数
            std::wstring bufFileName = filename.substr(0, filename.length() - 4) + L".buf";
            int bufFileSize = MMTFile::GetFileSize(G.WorkFolder + bufFileName);
            int vertexNumber = bufFileSize / stride;

            std::unordered_map<uint64_t, std::vector<std::byte>> fileBuf = MMTFile::ReadBufMapFromFile(G.WorkFolder + bufFileName, vertexNumber);
            LOG.Info(L"Extract from: " + bufFileName + L" VertexNumber:" + std::to_wstring(vertexNumber));
            LOG.Info("Category:" + Category + " CategorySlot:" + CategorySlot + " CategoryTopology:" + CategoryTopology);

            //自动补全BLENDWEIGHT 1,0,0,0  目前仅支持R32G32B32A32_FLOAT类型补全，目前也只遇到过这一种类型
            if (Category == "Blend" && patchBlendWeights) {
                std::unordered_map<uint64_t, std::vector<std::byte>> patchedFileBuf = MMTFormat::PatchBlendBuf_BLENDWEIGHT_1000(fileBuf);
                categoryFileBufList.push_back(patchedFileBuf);
            }
            else {
                categoryFileBufList.push_back(fileBuf);
            }


        }
        extractConfig.CategoryHashMap = CategoryHashMap;
        LOG.NewLine();

        //Buffer文件组合成一个
        std::vector<std::byte> finalVB0Buf;
        for (int i = 0; i < TranglelistMaxVertexCount; i++) {
            for (std::unordered_map<uint64_t, std::vector<std::byte>>& tmpFileBuf : categoryFileBufList) {
                finalVB0Buf.insert(finalVB0Buf.end(), tmpFileBuf[i].begin(), tmpFileBuf[i].end());
            }
        }

        LOG.Info("ElementList:");
        std::vector<std::string> TmpElementList;
        for (std::string ElementName : ElementList) {
            LOG.Info("ElementName: " + ElementName);
            TmpElementList.push_back(boost::algorithm::to_upper_copy(ElementName));
        }
        extractConfig.TmpElementList = TmpElementList;



        //生成FMT文件
        FmtFile fmtFileData;
        fmtFileData.ElementNameList = ElementList;
        fmtFileData.d3d11GameType = d3d11GameType;
        fmtFileData.Format = L"DXGI_FORMAT_R32_UINT";
        fmtFileData.Stride = d3d11GameType.getElementListStride(fmtFileData.ElementNameList);
        LOG.NewLine();


        //IB的Buffer文件根据索引拆分
        LOG.Info(L"Start to read trianglelist IB file info");
        // 读确定与众不同的trianglelist indices的ib文件，根据first index来确认。
        std::map<int, std::wstring> firstIndexFileNameMap;
        for (std::wstring index : TrianglelistIndices) {
            std::vector<std::wstring> trianglelistIBFileNameList = FAData.FindFrameAnalysisFileNameListWithCondition(index + L"-ib", L".txt");
            std::wstring trianglelistIBFileName;
            if (trianglelistIBFileNameList.empty()) {
                continue;
            }
            trianglelistIBFileName = trianglelistIBFileNameList[0];
            //LOG.Info(L"trianglelistIBFileName: " + trianglelistIBFileName);
            IndexBufferTxtFile indexBufferFileData = IndexBufferTxtFile(G.WorkFolder + trianglelistIBFileName, false);
            std::wstring firstIndex = indexBufferFileData.FirstIndex;
            //LOG.Info(L"firstIndex: " + firstIndex);
            firstIndexFileNameMap[std::stoi(firstIndex)] = trianglelistIBFileName;
        }
        LOG.Info(L"Output and see the final first index and trianglelist file:");
        std::vector<std::string> firstIndexList;
        for (auto it = firstIndexFileNameMap.begin(); it != firstIndexFileNameMap.end(); ++it) {
            LOG.Info(L"firstIndex: " + std::to_wstring(it->first) + L", trianglelistIBFileName: " + it->second);
            firstIndexList.push_back(std::to_string(it->first));
        }

        LOG.NewLine();

        //现在就可以输出了，根据每个MatchFirstIndex依次输出即可。
        LOG.Info(L"拼接并输出所有文件");
        int outputCount = 1;
        std::vector<std::string> MatchFirstIndexList;
        std::vector<std::string> PartNameList;
        std::unordered_map<std::wstring, std::wstring> MatchFirstIndex_PartName_Map;
        for (const auto& pair : firstIndexFileNameMap) {
            std::wstring IBReadFileName = pair.second;
            std::wstring IBReadBufferFileName = IBReadFileName.substr(0, IBReadFileName.length() - 4) + L".buf";
            std::wstring IBReadBufferFilePath = G.WorkFolder + IBReadBufferFileName;

            std::wstring IBReadFilePath = G.WorkFolder + IBReadFileName;
            IndexBufferTxtFile ibFileData(IBReadFilePath, false);

            MatchFirstIndexList.push_back(MMTString::ToByteString(ibFileData.FirstIndex));
            PartNameList.push_back(std::to_string(outputCount));

            //
            MatchFirstIndex_PartName_Map[ibFileData.FirstIndex] = std::to_wstring(outputCount);

            //分别输出fmt,ib,vb
            std::wstring OutputIBBufFilePath = OutputDrawIBFolder + DrawIB + L"-" + std::to_wstring(outputCount) + L".ib";
            std::wstring OutputVBBufFilePath = OutputDrawIBFolder + DrawIB + L"-" + std::to_wstring(outputCount) + L".vb";
            std::wstring OutputFmtFilePath = OutputDrawIBFolder + DrawIB + L"-" + std::to_wstring(outputCount) + L".fmt";

            //输出FMT文件
            fmtFileData.OutputFmtFile(OutputFmtFilePath);

            //输出IB文件
            IndexBufferBufFile ibBufFile(IBReadBufferFilePath, ibFileData.Format);
            ibBufFile.SelfDivide(std::stoi(ibFileData.FirstIndex), std::stoi(ibFileData.IndexCount));
            ibBufFile.SaveToFile_UINT32(OutputIBBufFilePath, -1 * ibBufFile.MinNumber);

            //输出VB文件
            VertexBufferBufFile vbBufFile;
            vbBufFile.FinalVB0Buf = finalVB0Buf;
            vbBufFile.SelfDivide(ibBufFile.MinNumber, ibBufFile.MaxNumber, fmtFileData.Stride);
            vbBufFile.SaveToFile(OutputVBBufFilePath);

            //PartName数自增
            outputCount++;
        }
        LOG.NewLine();
        extractConfig.MatchFirstIndexList = MatchFirstIndexList;
        extractConfig.PartNameList = PartNameList;

        //移动通用贴图数据
        MoveAllUsedTexturesToOutputFolder(DrawIB, OutputDrawIBFolder);

        std::vector<std::wstring> trianglelistIndexList = FAData.ReadTrianglelistIndexList(DrawIB);
        //通用贴图移动完毕之后，这里我们按游戏进行分别处理
        //这里的核心原理是每个Shader处理的槽位里的数据类型是固定不变的，且Shader的Hash也是不变的，只需要在版本更新时在这里更新添加即可。
        //如此即可做到完全精确的贴图类型识别。


        AutoDetectTextureFiles ( DrawIB, MatchFirstIndex_PartName_Map, d3d11GameType.GPUPreSkinning);
        for (const auto& pair : this->PartName_TextureSlotReplace_Map) {
            extractConfig.PartName_TextureSlotReplace_Map[pair.first] = pair.second.Save_SlotName_Equal_ResourceFileName_List;
        }

        //输出tmp.json
        extractConfig.SaveTmpJsonConfigs(OutputDrawIBFolder);
        this->PartName_TextureSlotReplace_Map.clear();


        LOG.NewLine();
    }

    LOG.Info("Extract model success");
}

