#include <filesystem>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "FrameAnalysisData.h"
#include "VertexBufferTxtFileDetect.h"
#include "IndexBufferTxtFile.h"
#include "MMTFileUtils.h"
#include "MMTLogUtils.h"
#include "IndexBufferBufFile.h"
#include "MMTStringUtils.h"
#include <set>
#include <unordered_map>
#include "ConstantBufferBufFile.h"
#include "IndexBufferTxtFile.h"


void FrameAnalysisData::ReadFrameAnalysisFileList(std::wstring WorkFolder) {
    this->WorkFolder = WorkFolder;
    this->FrameAnalysisFileNameList.clear();
    for (const auto& entry : std::filesystem::directory_iterator(WorkFolder)) {
        if (!(entry.is_regular_file())) {
            continue;
        }
        this->FrameAnalysisFileNameList.push_back(entry.path().filename().wstring());
    }

    //如果找不到.buf文件则弹出警告
    if (this->FindFrameAnalysisFileNameListWithCondition(L"", L".buf").size() == 0) {
        LOG.Error("Can't find any .buf file, please set your analyse_option config in your d3dx.ini to \nanalyse_options = dump_rt dump_tex  dump_cb dump_vb dump_ib buf txt  ");
    }
}


FrameAnalysisData::FrameAnalysisData() {

}


FrameAnalysisData::FrameAnalysisData(std::wstring WorkFolder, std::wstring DrawIB) {
    this->ReadFrameAnalysisFileList(WorkFolder);
    this->TrianglelistIndexList = this->ReadTrianglelistIndexList(DrawIB);
    this->PointlistIndexList = this->ReadPointlistIndexList();
}


FrameAnalysisData::FrameAnalysisData(std::wstring WorkFolder) {
    this->ReadFrameAnalysisFileList(WorkFolder);
}


std::vector<std::wstring> FrameAnalysisData::FindFrameAnalysisFileNameListWithCondition(std::wstring searchStr, std::wstring endStr) {
    std::vector<std::wstring> findFileNameList;
    for (std::wstring fileName :this->FrameAnalysisFileNameList ) {
        if (fileName.find(searchStr) != std::string::npos && fileName.substr(fileName.length() - endStr.length()) == endStr) {
            findFileNameList.push_back(fileName);
        }
    }
    return findFileNameList;
}


std::vector<std::wstring> FrameAnalysisData::ReadTrianglelistIndexList(std::wstring drawIB) {
    //首先根据DrawIB，获取所有的Trianglelist Index
    std::vector<std::wstring> trianglelistIndexList;
    for (std::wstring fileName : this->FrameAnalysisFileNameList) {
        //只需要txt文件
        if (!boost::algorithm::ends_with(fileName, L".txt")) {
            continue;
        }
        //必须包含DrawIB
        if (fileName.find(L"-ib=" + drawIB) != std::string::npos) {
            IndexBufferTxtFile ibTxtFile(this->WorkFolder + fileName, false);
            if (ibTxtFile.Topology == L"trianglelist") {
                if (FindFrameAnalysisFileNameListWithCondition(ibTxtFile.Index + L"-vb0", L".txt").size() != 0) {
                    trianglelistIndexList.push_back(ibTxtFile.Index);
                }
            }
        }
    }
    return trianglelistIndexList;
}


std::vector<std::wstring> FrameAnalysisData::ReadPointlistIndexList() {
    std::vector<std::wstring> pointlistIndexList;
    for (std::wstring fileName: this->FrameAnalysisFileNameList) {
        //只需要txt文件
        if (!boost::algorithm::ends_with(fileName, L".txt")) {
            continue;
        }

        if (fileName.find(L"-vb0") != std::string::npos) {
            VertexBufferDetect vbDetect(this->WorkFolder + fileName);
            if (vbDetect.Topology == L"pointlist") {
                pointlistIndexList.push_back(vbDetect.Index);
            }
        }
    }
    return pointlistIndexList;
}


std::map<std::wstring, uint32_t> FrameAnalysisData::Read_TrianglelistIndex_VertexCount_Map(VertexCountType InputVertexCountType, std::wstring DrawIB) {
    if (this->DrawIB_TrianglelistIndex_VertexCount_Map.contains(DrawIB)) {
        return this->DrawIB_TrianglelistIndex_VertexCount_Map[DrawIB];
    }

    //1.因为是根据DrawIB决定的，所以结果不能重用，必须每次都重新赋值
    std::map<std::wstring, uint32_t> Tmp_TrianglelistIndex_VertexCount_Map;
    for (std::wstring FileName : this->FrameAnalysisFileNameList) {
        if (!FileName.ends_with(L".txt")) {
            continue;
        }
        if (FileName.find(L"-ib") != std::string::npos && FileName.find(DrawIB) != std::string::npos) {
            //LOG.Info(L"Processing ib file: " + FileName);
            IndexBufferTxtFile indexBufferFileData = IndexBufferTxtFile(this->WorkFolder + FileName, false);
            if (indexBufferFileData.Topology == L"trianglelist") {
                std::vector<std::wstring> tmpNameList = this->FindFrameAnalysisFileNameListWithCondition(indexBufferFileData.Index + L"-vb0", L".txt");
                if (tmpNameList.size() == 0) {
                    //LOG.Info(L"Special Type, Continue : Can't find vb0 for -ib:" + FileName);
                    continue;
                }
                VertexBufferDetect vertexBufferDetect(this->WorkFolder + tmpNameList[0]);

                if (VertexCountType::BufFileRemoveNull == InputVertexCountType) {
                    Tmp_TrianglelistIndex_VertexCount_Map[vertexBufferDetect.Index] = vertexBufferDetect.fileBufferRealVertexCount;
                }
                else if(VertexCountType::BufFileTotal == InputVertexCountType){
                    Tmp_TrianglelistIndex_VertexCount_Map[vertexBufferDetect.Index] = vertexBufferDetect.fileBufferVertexCount;
                }
                else if (VertexCountType::TxtFileCountLine == InputVertexCountType) {
                    Tmp_TrianglelistIndex_VertexCount_Map[vertexBufferDetect.Index] = vertexBufferDetect.fileRealVertexCount;
                }
                else if (VertexCountType::TxtFileShow == InputVertexCountType) {
                    Tmp_TrianglelistIndex_VertexCount_Map[vertexBufferDetect.Index] = vertexBufferDetect.fileShowVertexCount;
                }
            }
        }
    }
    this->DrawIB_TrianglelistIndex_VertexCount_Map[DrawIB] = Tmp_TrianglelistIndex_VertexCount_Map;
    return this->DrawIB_TrianglelistIndex_VertexCount_Map[DrawIB];
}

std::map<std::wstring, uint32_t> FrameAnalysisData::Read_PointlistIndex_VertexCount_Map(VertexCountType InputVertexCountType) {
    //因为Pointlist是固定的，所以可以重复使用之前的结果
    if (this->PointlistIndex_VertexCount_Map.size() != 0) {
        return this->PointlistIndex_VertexCount_Map;
    }

    std::map<std::wstring, uint32_t> Tmp_PointlistIndex_VertexCount_Map;
    for (std::wstring FileName : this->FrameAnalysisFileNameList) {
        if (!FileName.ends_with(L".txt")) {
            continue;
        }
        if (FileName.find(L"-vb0") != std::string::npos) {
            VertexBufferDetect vbDetect(this->WorkFolder + FileName);
            if (vbDetect.Topology == L"pointlist") {
                if (VertexCountType::BufFileRemoveNull == InputVertexCountType) {
                    Tmp_PointlistIndex_VertexCount_Map[vbDetect.Index] = vbDetect.fileBufferRealVertexCount;
                }
                else if (VertexCountType::BufFileTotal == InputVertexCountType) {
                    Tmp_PointlistIndex_VertexCount_Map[vbDetect.Index] = vbDetect.fileBufferVertexCount;
                }
                else if (VertexCountType::TxtFileCountLine == InputVertexCountType) {
                    Tmp_PointlistIndex_VertexCount_Map[vbDetect.Index] = vbDetect.fileRealVertexCount;
                }
                else if (VertexCountType::TxtFileShow == InputVertexCountType) {
                    Tmp_PointlistIndex_VertexCount_Map[vbDetect.Index] = vbDetect.fileShowVertexCount;
                }
            }
        }
    }

    this->PointlistIndex_VertexCount_Map = Tmp_PointlistIndex_VertexCount_Map;
    return this->PointlistIndex_VertexCount_Map;
}


//TODO Deprecated
std::wstring FrameAnalysisData::GetRealDrawIndexFromLog(std::wstring DrawIB) {
    std::vector<std::wstring> originalIBFileList = this->FindFrameAnalysisFileNameListWithCondition(DrawIB, L".txt");
    if (originalIBFileList.size() > 0) {
        return DrawIB;
    }
    //这里我们要通过解析log文件来找到这个IB的真实IB值
    std::wstring LogTxtFilePath = this->WorkFolder + L"log.txt";
    if (!std::filesystem::exists(LogTxtFilePath)) {
        LOG.Error(L"Can't find log.txt in your FrameAnalysisFolder: " + this->WorkFolder);
    }
    LOG.Info("Find log.txt in FrameAnalysisFolder");
    std::vector<std::wstring> LogLineList = MMTFile::ReadAllLinesW(LogTxtFilePath);

    //查找包含hash=DrawIB的那一行
    std::wstring Index = L"";
    for (std::wstring logLine : LogLineList) {
        if (logLine.find(L"hash=" + DrawIB) != std::wstring::npos) {
            Index = logLine.substr(0, 6);
            break;
        }
    }
    if (Index == L"") {
        LOG.Error(L"Can't find valid index for: " + DrawIB);
    }
    else {
        LOG.Info(L"Find Index:" + Index);
    }

    std::vector<std::wstring> ibHashFileList = this->FindFrameAnalysisFileNameListWithCondition(Index + L"-ib", L".txt");
    if (ibHashFileList.size() == 0) {
        LOG.Error(L"Can't find ib file for index: " + Index);
    }
    std::wstring ibFileName = ibHashFileList[0];
    LOG.Info(L"Find ibFileName: " + ibFileName);
    std::wstring NewIndex = ibFileName.substr(0, 6);
    LOG.Info(L"Find NewIB Hash success: " + NewIndex);
    LOG.NewLine();
    return NewIndex;
}

std::wstring FrameAnalysisData::GetRealIB_IfNoModIB_FromLog(std::wstring DrawIB) {
    std::vector<std::wstring> originalIBFileList = this->FindFrameAnalysisFileNameListWithCondition(DrawIB,L".txt");
    if (originalIBFileList.size() > 0) {
        return DrawIB;
    }
    //这里我们要通过解析log文件来找到这个IB的真实IB值
    std::wstring LogTxtFilePath = this->WorkFolder + L"log.txt";
    if (!std::filesystem::exists(LogTxtFilePath)) {
        LOG.Error(L"Can't find log.txt in your FrameAnalysisFolder: " + this->WorkFolder);
    }
    LOG.Info("Find log.txt in FrameAnalysisFolder");
    std::vector<std::wstring> LogLineList = MMTFile::ReadAllLinesW(LogTxtFilePath);

    //查找包含hash=DrawIB的那一行
    std::wstring Index = L"";
    for (std::wstring logLine : LogLineList) {
        if (logLine.find(L"hash=" + DrawIB) != std::wstring::npos) {
            Index = logLine.substr(0, 6);
            break;
        }
    }
    if (Index == L"") {
        LOG.Error(L"Can't find valid index for: " + DrawIB);
    }
    else {
        LOG.Info(L"Find Index:" + Index);
    }

    std::vector<std::wstring> ibHashFileList = this->FindFrameAnalysisFileNameListWithCondition(Index + L"-ib", L".txt");
    if (ibHashFileList.size() == 0) {
        LOG.Error(L"Can't find ib file for index: " + Index);
    }
    std::wstring ibFileName = ibHashFileList[0];
    LOG.Info(L"Find ibFileName: " + ibFileName);
    std::wstring NewIBHash = ibFileName.substr(10, 8);
    LOG.Info(L"Find NewIB Hash success: " + NewIBHash);
    LOG.NewLine();
    return NewIBHash;

}



std::wstring FrameAnalysisData::FindDedupedTextureName(std::wstring WorkFolder, std::wstring TextureFileName) {
    //去deduped文件里找这个贴图对应的hash对应的文件，然后截取获得format信息
    //因为diffuse贴图肯定是用BC7_UNORM_SRGB或者BC7_UNORM  例如0b3e1922-BC7_UNORM_SRGB.dds 
    //这样可以过滤掉一槽位不同的一部分结果。
    //比较特殊，开启texture_hash = 1 后无法生效,比如WWMI无法生效。
    std::wstring dedupedFolderPath = WorkFolder + L"deduped\\";
    std::wstring DiffuseHashValue = MMTString::GetFileHashFromFileName(TextureFileName);
    if (TextureFileName.ends_with(L".dds")) {
        std::vector<std::wstring> textureDDSFileNameList = MMTFile::FindFileNameListWithCondition(dedupedFolderPath, DiffuseHashValue, L".dds");
        if (textureDDSFileNameList.size() == 0) {
            LOG.Warning(L"无法在deduped文件夹中找到此Hash值的贴图文件。");
            return L"";
        }
        else {
            return textureDDSFileNameList[0];
        }
    }
    else if (TextureFileName.ends_with(L".jpg")) {
        std::vector<std::wstring> textureJPGFileNameList = MMTFile::FindFileNameListWithCondition(dedupedFolderPath, DiffuseHashValue, L".jpg");
        if (textureJPGFileNameList.size() == 0) {
            LOG.Warning(L"无法在deduped文件夹中找到此Hash值的贴图文件。");
            return L"";
        }
        else {
            return textureJPGFileNameList[0];
        }
    }
    else {
        return L"";
    }
}


std::wstring FrameAnalysisData::GetIBMatchFirstIndexByIndex(std::wstring WorkFolder, std::wstring Index) {
    std::vector<std::wstring> ibFileNameList = FindFrameAnalysisFileNameListWithCondition(Index + L"-ib", L".txt");
    std::wstring ibFileName = L"";
    if (ibFileNameList.size() >= 1) {
        ibFileName = ibFileNameList[0];
    }
    else {
        LOG.Warning(L"Can't find ib file for index: " + Index);
        return L"";
    }
    IndexBufferTxtFile ibTxtFile(WorkFolder + ibFileName, false);
    //LOG.Info(L"当前Match_First_index: " + ibTxtFile.FirstIndex);
    return ibTxtFile.FirstIndex;
}


IndexBufferBufFile FrameAnalysisData::GetIBBufFileByIndex(std::wstring Index) {
    IndexBufferBufFile EmptyIBBufFile;
    std::vector<std::wstring> IBTxtFileNameList = this->FindFrameAnalysisFileNameListWithCondition(Index + L"-ib=", L".txt");
    if (IBTxtFileNameList.size() == 0) {
        LOG.Warning(L"无法通过Index获取IB的txt文件。");
        return EmptyIBBufFile;
    }
    std::wstring IBTxtFileName = IBTxtFileNameList[0];
    IndexBufferTxtFile IBTxtFile(this->WorkFolder + IBTxtFileName, false);
    std::wstring Format = IBTxtFile.Format;
    std::wstring IBBufFileName = MMTString::GetFileNameWithOutSuffix(IBTxtFileName) + L".buf";
    IndexBufferBufFile IBBufFile(this->WorkFolder + IBBufFileName, Format);
    IBBufFile.MatchFirstIndex = IBTxtFile.FirstIndex;
    return IBBufFile;
}


std::unordered_map<std::wstring, IndexBufferBufFile> FrameAnalysisData::Get_MatchFirstIndex_IBBufFile_Map_FromLog(std::wstring DrawIB) {
    LOG.Info(L"Get_MatchFirstIndex_IBBufFile_Map_FromLog: ");
    //这里我们要通过解析log文件来找到这个IB的真实IB值
    std::wstring LogTxtFilePath = this->WorkFolder + L"log.txt";
    if (!std::filesystem::exists(LogTxtFilePath)) {
        LOG.Error(L"Can't find log.txt in your FrameAnalysisFolder: " + this->WorkFolder);
    }
    LOG.Info("Find log.txt in FrameAnalysisFolder");
    std::vector<std::wstring> LogLineList = MMTFile::ReadAllLinesW(LogTxtFilePath);

    //查找包含hash=DrawIB的那一行
    std::unordered_map<std::wstring, IndexBufferBufFile> UniqueVertexCount_IBBufFile_Map;
    for (std::wstring logLine : LogLineList) {
        if (logLine.find(L"hash=" + DrawIB) != std::wstring::npos) {
            std::wstring Index = logLine.substr(0, 6);
            //直接找到对应的IB文件
            IndexBufferBufFile IBBufFile = this->GetIBBufFileByIndex(Index);

            //这里需要额外从Log文件里查到Index的真实DrawNumber
            
            if (IBBufFile.UniqueVertexCount != 0) {
                if (!UniqueVertexCount_IBBufFile_Map.contains(IBBufFile.MatchFirstIndex)) {
                    UniqueVertexCount_IBBufFile_Map[IBBufFile.MatchFirstIndex] = IBBufFile;
                    LOG.Info(L"Detect MatchFirstIndex: " + IBBufFile.MatchFirstIndex);
                }
            }
        }
    }
    return UniqueVertexCount_IBBufFile_Map;
    LOG.NewLine();
}


std::vector<std::wstring> FrameAnalysisData::ReadRealTrianglelistIndexListFromLog(std::wstring OriginalDrawIB) {
    //这里我们要通过解析log文件来找到这个IB的真实IB值
    std::wstring LogTxtFilePath = this->WorkFolder + L"log.txt";
    if (!std::filesystem::exists(LogTxtFilePath)) {
        LOG.Error(L"Can't find log.txt in your FrameAnalysisFolder: " + this->WorkFolder);
    }
    LOG.Info("Find log.txt in FrameAnalysisFolder");
    std::vector<std::wstring> LogLineList = MMTFile::ReadAllLinesW(LogTxtFilePath);

    //查找包含hash=DrawIB的那一行
    std::vector<std::wstring> RealTrianglelistIndexList;
    for (std::wstring logLine : LogLineList) {
        if (logLine.find(L"hash=" + OriginalDrawIB) != std::wstring::npos) {
            std::wstring Index = logLine.substr(0, 6);
            RealTrianglelistIndexList.push_back(Index);
        }
    }
    return RealTrianglelistIndexList;
}


std::unordered_map<std::wstring, uint32_t> FrameAnalysisData::Get_Index_CS_CB0_VertexCount_Map_FromCSBufferFiles() {
    //省的重复执行浪费资源
    if (this->Index_CS_CB0_VertexCount_Map.size() != 0) {
        return Index_CS_CB0_VertexCount_Map;
    }
    
    std::unordered_map<std::wstring, uint32_t> Tmp_Index_CS_CB0_VertexCount_Map;

    for (std::wstring FileName: this->FrameAnalysisFileNameList) {
        if (FileName.find(L"-cs=") == std::wstring::npos) {
            continue;
        }
        if (FileName.find(L"-cs-cb0=") == std::wstring::npos) {
            continue;
        }
        if (!FileName.ends_with(L".buf")) {
            continue;
        }
        ConstantBufferBufFile CSBufFile(this->WorkFolder + FileName);
        uint32_t VertexCount = CSBufFile.lineCBValueMap[0].X;
       
        std::wstring Index = FileName.substr(0, 6);
        //LOG.Info(L"Index: " + Index + L" VertexCount: " + std::to_wstring(VertexCount) );
        Tmp_Index_CS_CB0_VertexCount_Map[Index] = VertexCount;
    }
    return Tmp_Index_CS_CB0_VertexCount_Map;
}


std::wstring FrameAnalysisData::GetComputeIndexFromCSCB0_ByVertexCount(uint32_t VertexCount) {
    //根据cs-cb0获取所有cs的Index和顶点数的Map
    std::unordered_map<std::wstring, uint32_t> Index_CS_CB0_VertexCount_Map = this->Get_Index_CS_CB0_VertexCount_Map_FromCSBufferFiles();
    //根据顶点数找到对应的Index作为提取用的Index
    std::wstring CS_ExtractIndex = L"";
    for (const auto& pair : Index_CS_CB0_VertexCount_Map) {
        if (VertexCount == pair.second) {
            CS_ExtractIndex = pair.first;
            break;
        }
    }
    return CS_ExtractIndex;
}


std::map<uint32_t, std::wstring> FrameAnalysisData::Get_MatchFirstIndex_IBFileName_Map(std::wstring DrawIB) {
    std::map<uint32_t, std::wstring> firstIndexFileNameMap;
    std::vector<std::wstring> TrianglelistIndexList = this->ReadTrianglelistIndexList(DrawIB);
    for (std::wstring index : TrianglelistIndexList) {
        std::vector<std::wstring> trianglelistIBFileNameList = this->FindFrameAnalysisFileNameListWithCondition(index + L"-ib", L".txt");
        std::wstring trianglelistIBFileName;
        if (trianglelistIBFileNameList.empty()) {
            continue;
        }
        trianglelistIBFileName = trianglelistIBFileNameList[0];
        IndexBufferTxtFile indexBufferFileData(this->WorkFolder + trianglelistIBFileName, false);
        std::wstring firstIndex = indexBufferFileData.FirstIndex;
        firstIndexFileNameMap[std::stoi(firstIndex)] = trianglelistIBFileName;
    }
    return firstIndexFileNameMap;
}

uint32_t FrameAnalysisData::Get_VertexCount_ByAddIBFileUniqueCount(std::wstring DrawIB) {

    std::map<uint32_t, std::wstring> MatchFirstIndex_IBFileName_Map = this->Get_MatchFirstIndex_IBFileName_Map(DrawIB);
    uint32_t TotalVertexCount = 0;

    for (const auto& pair: MatchFirstIndex_IBFileName_Map) {
        std::wstring IBFileName = pair.second;
        IndexBufferTxtFile IBTxtFile(this->WorkFolder + IBFileName, true);
        TotalVertexCount = TotalVertexCount + IBTxtFile.UniqueVertexCount;
    }

    return TotalVertexCount;
}