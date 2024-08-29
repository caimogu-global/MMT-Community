#include "IndexBufferBufFile.h"
#include "VertexBufferBufFile.h"
#include "MMTFormatUtils.h"
#include "GlobalConfigs.h"
#include "WWUtil.h"

#include "Functions_ToF.h"


void Functions_ToF::GenerateMod() {

    for (const auto& pair : G.DrawIB_ExtractConfig_Map) {
        std::wstring DrawIB = pair.first;
        DrawIBConfig extractConfig = pair.second;

        D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[extractConfig.WorkGameType];

        std::wstring timeStr = MMTString::GetFormattedDateTimeForFilename().substr(0, 10);
        std::wstring BufferReadFolder = G.OutputFolder + DrawIB + L"\\";
        std::wstring BufferOutputFolder = G.OutputFolder + timeStr + L"\\" + DrawIB + L"\\";
        std::filesystem::create_directories(BufferOutputFolder);

        bool findValidFile = false;
        for (std::string partName : extractConfig.PartNameList) {
            std::wstring VBFileName = MMTString::ToWideString(partName) + L".vb";
            if (std::filesystem::exists(BufferReadFolder + VBFileName)) {
                findValidFile = true;
                break;
            }
        }
        if (!findValidFile) {
            LOG.Info(L"Detect didn't export vb file for DrawIB: " + DrawIB + L" , so skip this drawIB generate.");
            continue;
        }


        std::unordered_map<std::string, uint32_t> CategoryStrideMap = d3d11GameType.getCategoryStrideMap(extractConfig.TmpElementList);
        std::vector<std::string> CategoryList = d3d11GameType.getCategoryList(extractConfig.TmpElementList);
        //输出查看每个Category的步长
        for (const auto& pair : CategoryStrideMap) {
            const std::string& key = pair.first;
            int value = pair.second;
            LOG.Info("Category: " + key + ", CategoryStride: " + std::to_string(value));
        }
        LOG.NewLine();

        //查看CategoryList
        LOG.Info(L"CategoryList:");
        for (std::string categoryName : CategoryList) {
            LOG.Info("Ordered CategoryName: " + categoryName);
        }
        LOG.NewLine();



        //(1) 输出BUF文件
        //读取vb文件，每个vb文件都按照category分开装载不同category的数据
        int SplitStride = d3d11GameType.getElementListStride(extractConfig.TmpElementList);
        LOG.Info(L"SplitStride: " + std::to_wstring(SplitStride));
        int drawNumber = 0;
        std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::vector<std::byte>>> partName_VBCategoryDaytaMap;
        std::unordered_map<std::string, int> partNameOffsetMap;
        std::unordered_map<std::string, int> partNameVertexCountMap;
        for (std::string partName : extractConfig.PartNameList) {
            std::wstring VBFileName = MMTString::ToWideString(partName) + L".vb";
            uint64_t VBFileSize = MMTFile::GetFileSize(BufferReadFolder + VBFileName);
            uint64_t vbFileVertexNumber = VBFileSize / SplitStride;

            partNameVertexCountMap[partName] = (int)vbFileVertexNumber;
            LOG.Info(L"Processing VB file: " + VBFileName + L" size is: " + std::to_wstring(VBFileSize) + L" byte." + L" vertex number is: " + std::to_wstring(vbFileVertexNumber));
            VertexBufferBufFile vbBufFile(BufferReadFolder + VBFileName, d3d11GameType, extractConfig.TmpElementList);
            partName_VBCategoryDaytaMap[MMTString::ToWideString(partName)] = vbBufFile.CategoryVBDataMap;


            //设置offset
            partNameOffsetMap[partName] = drawNumber;
            //添加到drawNumber
            drawNumber = drawNumber + (int)vbFileVertexNumber;
        }



        //(2) 转换并输出每个IB文件,这里注意输出的IB要加上Offset，因为我们Blender导出的IB都是从0开始的
        //先设置读取ib文件所使用的Format,从1.fmt文件中自动读取
        std::wstring IBReadDxgiFormat = MMTFile::FindMigotoIniAttributeInFile(BufferReadFolder + L"1.fmt", L"format");
        for (std::string partName : extractConfig.PartNameList) {
            std::wstring IBFileName = MMTString::ToWideString(partName) + L".ib";
            std::wstring readIBFileName = BufferReadFolder + IBFileName;
            std::wstring writeIBFileName = BufferOutputFolder + IBFileName;
            LOG.Info(L"Converting IB file: " + IBFileName);
            IndexBufferBufFile ibBufFile(readIBFileName, IBReadDxgiFormat);
            ibBufFile.SaveToFile_UINT32(writeIBFileName, partNameOffsetMap[partName]);
        }
        LOG.Info(L"Output ib file over");
        LOG.NewLine();


        LOG.Info(L"Combine and put partName_VBCategoryDaytaMap's content back to finalVBCategoryDataMap");
        //将partName_VBCategoryDaytaMap里的内容，放入finalVBCategoryDataMap中组合成一个，供后续使用
        std::unordered_map<std::wstring, std::vector<std::byte>> finalVBCategoryDataMap;
        for (std::string partName : extractConfig.PartNameList) {
            std::unordered_map<std::wstring, std::vector<std::byte>> tmpVBCategoryDataMap = partName_VBCategoryDaytaMap[MMTString::ToWideString(partName)];
            for (size_t i = 0; i < CategoryList.size(); ++i) {
                const std::string& category = CategoryList[i];
                std::vector<std::byte> tmpCategoryData = tmpVBCategoryDataMap[MMTString::ToWideString(category)];
                if (category == "Normal") {
                    for (int index = 0; index < tmpCategoryData.size(); index = index + 8) {
                        //1.获取NORMAL和TANGENT值
                        std::byte NormalValueX = tmpCategoryData[index + 0];
                        std::byte NormalValueY = tmpCategoryData[index + 1];
                        std::byte NormalValueZ = tmpCategoryData[index + 2];
                        std::byte TangentValueX = tmpCategoryData[index + 4];
                        std::byte TangentValueY = tmpCategoryData[index + 5];
                        std::byte TangentValueZ = tmpCategoryData[index + 6];

                        //2.经过观察NORMAL的值为TANGENT前三位直接放过来，最后一位设为0x7F
                        tmpCategoryData[index + 0] = TangentValueX;
                        tmpCategoryData[index + 1] = TangentValueY;
                        tmpCategoryData[index + 2] = TangentValueZ;
                        tmpCategoryData[index + 3] = std::byte(0x7F);

                        //3.翻转NORMAL的前三位并放到TANGENT的前三位，NORMAL的W设为0x7F
                        tmpCategoryData[index + 4] = MMTFormat::ReverseSNORMValueSingle(NormalValueX);
                        tmpCategoryData[index + 5] = MMTFormat::ReverseSNORMValueSingle(NormalValueY);
                        tmpCategoryData[index + 6] = MMTFormat::ReverseSNORMValueSingle(NormalValueZ);
                    }
                }
                std::vector<std::byte>& finalCategoryData = finalVBCategoryDataMap[MMTString::ToWideString(category)];
                finalCategoryData.insert(finalCategoryData.end(), tmpCategoryData.begin(), tmpCategoryData.end());

            }
        }
        LOG.NewLine();

        //直接输出
        for (const auto& pair : finalVBCategoryDataMap) {
            const std::wstring& categoryName = pair.first;
            const std::vector<std::byte>& categoryData = pair.second;
            LOG.Info(L"Output buf file, current category: " + categoryName + L" Length:" + std::to_wstring(categoryData.size() / drawNumber));
            //如果没有那就不输出
            if (categoryData.size() == 0) {
                LOG.Info(L"Current category's size is 0, can't output, skip this.");
                continue;
            }
            std::wstring categoryGeneratedName = DrawIB + categoryName;
            // 构建输出文件路径
            std::wstring outputDatFilePath = BufferOutputFolder + categoryGeneratedName + L".buf";
            // 打开输出文件 将std::vecto的内容写入文件
            std::ofstream outputFile(MMTString::ToByteString(outputDatFilePath), std::ios::binary);
            outputFile.write(reinterpret_cast<const char*>(categoryData.data()), categoryData.size());
            outputFile.close();
            LOG.Info(L"Write " + categoryName + L" data into file: " + outputDatFilePath);
        }
        LOG.NewLine();


        //(3) 生成ini文件
        std::wstring outputIniFileName = BufferOutputFolder + extractConfig.DrawIB + L".ini";
        std::wofstream outputIniFile(outputIniFileName);

        //如果是使用CS计算，则突破顶点数量限制
        //TODO 有空研究一下鸣潮这种特殊情况应该在哪里调用Dispatch

        bool debug = false;
        if (d3d11GameType.GPUPreSkinning && debug) {
            outputIniFile << L"\n";
            outputIniFile << L"; -------------- Break Vertex Count Limit -----------------" << L"\n" << L"\n";
            outputIniFile << L"[TextureOverride_CSReplace_VertexLimitBreak]" << L"\n";
            //TODO 需要收集cs-cb0的hash值
            outputIniFile << L"hash = " << MMTString::ToWideString(extractConfig.VertexLimitVB) << L"\n";

            //先读取csinfo.json里的数据
            WuwaCSInfoJsonObject wwcsinfoObject(BufferReadFolder);

            //遍历每个partName
            int currentOffset = 0;
            for (std::string partName : extractConfig.PartNameList) {
                int currentVertexCount = partNameVertexCountMap[partName];
                WuwaCSInfo wwcsInfo = wwcsinfoObject.PartNameWuwaCSInfoMap[partName];
                int originalVertexCount = wwcsInfo.CalculateTime;
                int originalOffset = wwcsInfo.Offset;
                LOG.Info("CurrentVertexCount: " + std::to_string(currentVertexCount) + " OriginalVertexCount: " + std::to_string(originalVertexCount));
                outputIniFile << L";------------" + MMTString::ToWideString(partName) + L"-----------" << L"\n";

                if (originalVertexCount == currentVertexCount) {
                    //顶点数相同时，无需改变顶点数，但是如果偏移不同还是需要改偏移的。
                    if (currentOffset != originalOffset) {
                        if (wwcsInfo.ComputeShaderHash == "4d0760c2c7406824") {
                            //修改3个偏移数
                            outputIniFile << L"csreplace = cs-cb0, 1, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n";
                            outputIniFile << L"csreplace = cs-cb0, 2, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n";
                            outputIniFile << L"csreplace = cs-cb0, 3, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n";
                        }
                        else if (wwcsInfo.ComputeShaderHash == "1ff924db9d4048d1") {
                            //修改2个偏移数
                            outputIniFile << L"csreplace = cs-cb0, 1, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n";
                            outputIniFile << L"csreplace = cs-cb0, 2, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n";
                        }

                    }
                }
                else {
                    if (currentOffset != originalOffset) {
                        //顶点数不同，偏移也不同时全部都要修改
                        if (wwcsInfo.ComputeShaderHash == "4d0760c2c7406824") {
                            //修改3个偏移数
                            outputIniFile << L"csreplace = cs-cb0, 1, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n";
                            outputIniFile << L"csreplace = cs-cb0, 2, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n";
                            outputIniFile << L"csreplace = cs-cb0, 3, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n" << L"\n";
                            //修改1个顶点数
                            outputIniFile << L"csreplace = cs-cb0, 4, " + std::to_wstring(originalVertexCount) + L"," + std::to_wstring(currentVertexCount) << L"\n";

                        }
                        else if (wwcsInfo.ComputeShaderHash == "1ff924db9d4048d1") {
                            //修改2个偏移数
                            outputIniFile << L"csreplace = cs-cb0, 1, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n";
                            outputIniFile << L"csreplace = cs-cb0, 2, " + std::to_wstring(originalOffset) + L"," + std::to_wstring(currentOffset) << L"\n" << L"\n";
                            //修改1个顶点数
                            outputIniFile << L"csreplace = cs-cb0, 3, " + std::to_wstring(originalVertexCount) + L"," + std::to_wstring(currentVertexCount) << L"\n";

                        }
                    }
                    else {
                        //顶点数不同，偏移相同时，只需要改顶点数
                        if (wwcsInfo.ComputeShaderHash == "4d0760c2c7406824") {
                            //修改1个顶点数
                            outputIniFile << L"csreplace = cs-cb0, 4, " + std::to_wstring(originalVertexCount) + L"," + std::to_wstring(currentVertexCount) << L"\n";

                        }
                        else if (wwcsInfo.ComputeShaderHash == "1ff924db9d4048d1") {
                            //修改1个顶点数
                            outputIniFile << L"csreplace = cs-cb0, 3, " + std::to_wstring(originalVertexCount) + L"," + std::to_wstring(currentVertexCount) << L"\n";

                        }
                    }
                }

                currentOffset = currentOffset + currentVertexCount;
            }

        }

        outputIniFile << L"\n";
        outputIniFile << L"; -------------- TextureOverride VB -----------------" << L"\n" << L"\n";
        //1.写出VBResource部分
        for (std::string categoryName : CategoryList) {
            std::wstring fileName = extractConfig.DrawIB + MMTString::ToWideString(categoryName) + L".buf";
            std::wstring filePath = BufferOutputFolder + fileName;
            int fileSize = MMTFile::GetFileSize(filePath);
            std::string categoryHash = extractConfig.CategoryHashMap[categoryName];
            std::string categorySlot = d3d11GameType.CategorySlotMap[categoryName];
            outputIniFile << L"[TextureOverride_" + MMTString::ToWideString(categoryName) + L"_Replace]" << L"\n";
            outputIniFile << L"hash = " << MMTString::ToWideString(categoryHash) << "" << L"\n";
            outputIniFile << "this = " << L"Resource_VB_" + MMTString::ToWideString(categoryName) + L"" << L"\n" << L"\n";
        }

        outputIniFile << L"\n";
        outputIniFile << L"; -------------- Resource VB -----------------" << L"\n" << L"\n";
        //ResourceVB
        for (std::string categoryName : CategoryList) {
            std::wstring fileName = extractConfig.DrawIB + MMTString::ToWideString(categoryName) + L".buf";
            std::wstring filePath = BufferOutputFolder + fileName;
            int fileSize = MMTFile::GetFileSize(filePath);
            std::string categoryHash = extractConfig.CategoryHashMap[categoryName];
            std::string categorySlot = d3d11GameType.CategorySlotMap[categoryName];


            outputIniFile << L"[Resource_VB_" + MMTString::ToWideString(categoryName) + L"]" << L"\n";
            outputIniFile << L"byte_width = " << std::to_wstring(fileSize) << L"\n";
            if (categoryName == "Texcoord") {
                outputIniFile << L"type = Buffer" << L"\n";
                outputIniFile << L"FORMAT = R16G16_FLOAT" << L"\n";
            }
            else if (categoryName == "Normal") {
                outputIniFile << L"type = Buffer" << L"\n";
                outputIniFile << L"FORMAT = R8G8B8A8_SNORM" << L"\n";
            }
            else if (categoryName == "Position" && categorySlot == "vb0") {
                outputIniFile << L"type = Buffer" << L"\n";
                outputIniFile << L"FORMAT = R32G32B32_FLOAT" << L"\n";
            }
            else {
                outputIniFile << L"type = ByteAddressBuffer" << L"\n";
                outputIniFile << "stride = " << CategoryStrideMap[categoryName] << L"\n";
            }
            outputIniFile << "filename = " << fileName << L"\n" << L"\n";


        }

        outputIniFile << L"\n";
        outputIniFile << L"; -------------- IB Skip -----------------" << L"\n" << L"\n";

        //IB SKIP部分
        outputIniFile << L"[Resource_BakIB]" << L"\n";
        outputIniFile << L"[TextureOverride_" + extractConfig.DrawIB + L"_IB_SKIP]" << L"\n";
        outputIniFile << L"hash = " + extractConfig.DrawIB << L"\n";
        outputIniFile << "handling = skip" << L"\n";
        outputIniFile << L"\n";

        outputIniFile << L"\n";
        outputIniFile << L"; -------------- TextureOverride IB & Resource IB-----------------" << L"\n" << L"\n";

        //TextureOverride IB部分
        for (int i = 0; i < extractConfig.PartNameList.size(); ++i) {
            std::string partName = extractConfig.PartNameList[i];
            LOG.Info(L"Start to output UE4 ini file.");

            //按键开关支持
            bool generateSwitchKey = false;
            std::wstring activateFlagName = L"ActiveFlag_" + extractConfig.DrawIB;
            std::wstring switchVarName = L"SwitchVar_" + extractConfig.DrawIB;
            std::wstring replace_prefix = L"";

            LOG.Info(L"Generate Switch Key ini :" + std::to_wstring(generateSwitchKey));

            //4.IBOverride部分
            std::string IBFirstIndex = extractConfig.MatchFirstIndexList[i];

            outputIniFile << L"[TextureOverride_IB_" + extractConfig.DrawIB + L"_" + MMTString::ToWideString(partName) + L"]" << L"\n";
            outputIniFile << L"hash = " + extractConfig.DrawIB << L"\n";
            outputIniFile << L"Resource_BakIB = ref ib" << L"\n";
            outputIniFile << L"match_first_index = " + MMTString::ToWideString(IBFirstIndex) << L"\n";
            if (generateSwitchKey) {
                outputIniFile << L"if $" + switchVarName + L" == 1" << L"\n";
            }
            outputIniFile << replace_prefix << L"ib = Resource_IB_" + extractConfig.DrawIB + L"_" + MMTString::ToWideString(partName) << L"\n";
            outputIniFile << replace_prefix << "drawindexed = auto" << L"\n";
            outputIniFile << L"ib = Resource_BakIB" << L"\n";

            if (generateSwitchKey) {
                outputIniFile << "endif" << L"\n";
            }
            outputIniFile << L"\n";
        }

        outputIniFile << L"\n";
        outputIniFile << L"; -------------- IB Resource -----------------" << L"\n" << L"\n";

        //2.写出IBResource部分
        for (int i = 0; i < extractConfig.PartNameList.size(); ++i) {
            std::string partName = extractConfig.PartNameList[i];
            outputIniFile << L"[Resource_IB_" + extractConfig.DrawIB + L"_" + MMTString::ToWideString(partName) + L"]" << L"\n";
            outputIniFile << "type = Buffer" << L"\n";
            outputIniFile << "format = DXGI_FORMAT_R32_UINT" << L"\n";
            outputIniFile << "filename = " << MMTString::ToWideString(partName) + L".ib" << L"\n" << L"\n";
        }


        outputIniFile.close();

        LOG.NewLine();
        LOG.Info("Generate mod completed!");
        LOG.NewLine();


    }

}