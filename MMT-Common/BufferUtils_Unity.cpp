#include "BufferUtils.h"

#include "GlobalConfigs.h"
#include "MMTFormatUtils.h"

#include "MMTConstants.h"


namespace BufferUtils {

    void Unity_COLOR_AverageNormal(std::unordered_map<std::wstring, std::vector<std::byte>>& finalVBCategoryDataMap, DrawIBConfig basicConfig, D3D11GameType d3d11GameType) {

        LOG.Info(L"开始执行COLOR重计算，使用average_normal算法：");
        //D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[basicConfig.WorkGameType];
        
        //获取Position分类的内容
        std::vector<std::byte> PositionCategoryValues = finalVBCategoryDataMap[L"Position"];

        //获取POSITION和TANGENT
        std::vector<std::vector<double>> POSITION_VALUES;
        std::vector<std::vector<double>> NORMAL_VALUES;

        int positionStride = 0;
        //因为POSITION不会出现像TEXCOORD那样的动态步长，所以这里直接获取OrderedElementList，统计出Position的长度
        for (std::string elementName : d3d11GameType.OrderedFullElementList) {
            D3D11Element d3d11Element = d3d11GameType.ElementNameD3D11ElementMap[elementName];
            if (d3d11Element.Category == "Position") {
                int byteWidth = d3d11Element.ByteWidth;
                positionStride += byteWidth;
            }
        }
        LOG.Info(L"PositionStride: " + std::to_wstring(positionStride));
        for (std::size_t i = 0; i < PositionCategoryValues.size(); i = i + positionStride)
        {
            std::vector<std::byte> POSITION_X = MMTFormat::GetRange_Byte(PositionCategoryValues, i, i + 4);
            //LOG.LogOutput(L"POSITION_X: " + std::to_wstring(bytesToFloat(POSITION_X)));
            std::vector<std::byte> POSITION_Y = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 4, i + 8);
            std::vector<std::byte> POSITION_Z = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 8, i + 12);
            std::vector<double> POSITION_DOUBLE = { MMTFormat::ByteVectorToFloat(POSITION_X), MMTFormat::ByteVectorToFloat(POSITION_Y) ,MMTFormat::ByteVectorToFloat(POSITION_Z) };
            POSITION_VALUES.push_back(POSITION_DOUBLE);

            std::vector<std::byte> NORMAL_X = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 12, i + 16);
            std::vector<std::byte> NORMAL_Y = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 16, i + 20);
            std::vector<std::byte> NORMAL_Z = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 20, i + 24);
            std::vector<double> NORMAL_DOUBLE = { MMTFormat::ByteVectorToFloat(NORMAL_X), MMTFormat::ByteVectorToFloat(NORMAL_Y) ,MMTFormat::ByteVectorToFloat(NORMAL_Z) };
            NORMAL_VALUES.push_back(NORMAL_DOUBLE);
        }
        LOG.Info(L"Start to CalculateAverageNormalsAndStoreTangent");
        //把顶点相同的法线 3维向量 相加然后归一化，作为所有值为这个顶点值的相同向量
        //然后对每个对应的TANGENT，都用这个值作为前3位，0作为最后一位

        //计算出平局法线的TANGENT
        std::vector<std::vector<double>> TANGENT_VALUES = MMTFormat::NormalNormalizeStoreTangent(POSITION_VALUES, NORMAL_VALUES);
        LOG.Info(L"TANGENT_VALUES size: " + std::to_wstring(TANGENT_VALUES.size()));
        LOG.Info(L"CalculateAverageNormalsAndStoreTangent success");


        //准备一个新的用于返回
        std::unordered_map<std::wstring, std::vector<std::byte>> newVBCategoryDataMap;

        //首先要确定Color在哪个槽位里
        D3D11Element colorElement = d3d11GameType.ElementNameD3D11ElementMap["COLOR"];
        std::wstring colorCategory = MMTString::ToWideString(colorElement.Category);
        std::vector<std::byte> colorCategoryValues = finalVBCategoryDataMap[colorCategory];

        //确定Color所在槽位的步长
        int colorCategoryStride = d3d11GameType.CategoryStrideMap[MMTString::ToByteString(colorCategory)];

        //确定Color所在的槽位偏移
        int colorOffset = 0;

        //TODO 确定Color的ByteWidth,这里我们固定为4，因为只有这一种情况，其实也用不到，不过留作后续兼容使用
        //int colorByteWidth = 4;

        for (std::string elementName : basicConfig.TmpElementList) {
            D3D11Element d3d11Element = d3d11GameType.ElementNameD3D11ElementMap[elementName];

            //首先必须属于color所在的槽位
            if (d3d11Element.Category == MMTString::ToByteString(colorCategory)) {
                if (elementName != "COLOR") {
                    colorOffset += d3d11Element.ByteWidth;
                }
                else {
                    break;
                }
            }
        }

        //开始更改
        std::vector<std::byte> newTexcoordCategoryValues;
        int count = 0;
        for (std::size_t i = 0; i < colorCategoryValues.size(); i = i + colorCategoryStride)
        {
            //先获取COLOR前的所有值
            std::vector<std::byte> beforeValues = MMTFormat::GetRange_Byte(colorCategoryValues, i, i + colorOffset);
            std::vector<std::byte> ColorValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset, i + colorOffset + 4);
            std::vector<std::byte> afterValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 4, i + colorCategoryStride);
            //新准备的COLOR值
            std::vector<std::byte> newColorValues;
            std::vector<double> TANGENT_DOUBLES = TANGENT_VALUES[count];

            //R,G,B分别存储归一化之后Normal的分量
            newColorValues.push_back(MMTFormat::PackNumberToByte(static_cast<uint32_t>(round(TANGENT_DOUBLES[0] * 255))));
            newColorValues.push_back(MMTFormat::PackNumberToByte(static_cast<uint32_t>(round(TANGENT_DOUBLES[1] * 255))));
            newColorValues.push_back(MMTFormat::PackNumberToByte(static_cast<uint32_t>(round(TANGENT_DOUBLES[2] * 255))));

            //Alpha通道固定为128
            newColorValues.push_back(ColorValues[3]);

            newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), beforeValues.begin(), beforeValues.end());
            newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), newColorValues.begin(), newColorValues.end());
            newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), afterValues.begin(), afterValues.end());

            count++;
        }

        finalVBCategoryDataMap[colorCategory] = newTexcoordCategoryValues;
        LOG.NewLine();
    }





    //兼容COLOR在Position分类中的情况，一种算法同时兼容Position和Texcoord
    void Unity_Reset_COLOR(std::unordered_map<std::wstring, std::vector<std::byte>>& finalVBCategoryDataMap, DrawIBConfig basicConfig, D3D11GameType d3d11GameType) {
        LOG.Info(L"Start to reset COLOR Values:");

        //读取各个RGB分量的值
        std::wstring rgb_r = MMTString::ToWideString(basicConfig.ColorMap["rgb_r"]);
        std::wstring rgb_g = MMTString::ToWideString(basicConfig.ColorMap["rgb_g"]);
        std::wstring rgb_b = MMTString::ToWideString(basicConfig.ColorMap["rgb_b"]);
        std::wstring rgb_a = MMTString::ToWideString(basicConfig.ColorMap["rgb_a"]);



        //首先要确定Color在哪个槽位里
        //D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[basicConfig.WorkGameType];
        D3D11Element colorElement = d3d11GameType.ElementNameD3D11ElementMap["COLOR"];
        std::wstring colorCategory = MMTString::ToWideString(colorElement.Category);
        std::vector<std::byte> colorCategoryValues = finalVBCategoryDataMap[colorCategory];

        //确定Color所在槽位的步长
        int colorCategoryStride = d3d11GameType.CategoryStrideMap[MMTString::ToByteString(colorCategory)];
        LOG.Info(L"当前重置COLOR值的格式:"+MMTString::ToWideString(colorElement.Format));

        if (colorElement.Format == DXGI_FORMAT::R8G8B8A8_UNORM) {

            //准备好数字用于替换
            int rgb_r_num = -1;
            int rgb_g_num = -1;
            int rgb_b_num = -1;
            int rgb_a_num = -1;

            //尝试进行格式转换
            if (L"default" != rgb_r) {
                rgb_r_num = std::stoi(rgb_r);
            }
            if (L"default" != rgb_g) {
                rgb_g_num = std::stoi(rgb_g);
            }
            if (L"default" != rgb_b) {
                rgb_b_num = std::stoi(rgb_b);
            }
            if (L"default" != rgb_a) {
                rgb_a_num = std::stoi(rgb_a);
            }

            //如果都是default那干脆啥也别干直接返回
            if (rgb_a_num == -1 && rgb_g_num == -1 && rgb_b_num == -1 && rgb_a_num == -1) {
                LOG.Info(L"未检测到需要修改的COLOR值分量");
                LOG.NewLine();
            }
            else {
                //准备好数字用于写入
                std::byte patchRgb_R = MMTFormat::PackNumberToByte(rgb_r_num);
                std::byte patchRgb_G = MMTFormat::PackNumberToByte(rgb_g_num);
                std::byte patchRgb_B = MMTFormat::PackNumberToByte(rgb_b_num);
                std::byte patchRgb_A = MMTFormat::PackNumberToByte(rgb_a_num);

                LOG.Info(L"rgb_r_num: " + std::to_wstring(rgb_r_num));
                LOG.Info(L"rgb_g_num: " + std::to_wstring(rgb_g_num));
                LOG.Info(L"rgb_b_num: " + std::to_wstring(rgb_b_num));
                LOG.Info(L"rgb_a_num: " + std::to_wstring(rgb_a_num));

                //确定Color所在的槽位偏移
                int colorOffset = 0;
                LOG.Info(L"colorOffset: " + std::to_wstring(colorOffset));

                for (std::string elementName : basicConfig.TmpElementList) {
                    D3D11Element d3d11Element = d3d11GameType.ElementNameD3D11ElementMap[elementName];

                    //首先必须属于color所在的槽位
                    if (d3d11Element.Category == MMTString::ToByteString(colorCategory)) {
                        if (elementName != "COLOR") {
                            colorOffset += d3d11Element.ByteWidth;
                        }
                        else {
                            break;
                        }
                    }
                }

                //开始更改
                std::vector<std::byte> newTexcoordCategoryValues;
                for (std::size_t i = 0; i < colorCategoryValues.size(); i = i + colorCategoryStride)
                {
                    //先获取COLOR前的所有值
                    std::vector<std::byte> beforeValues = MMTFormat::GetRange_Byte(colorCategoryValues, i, i + colorOffset);
                    std::vector<std::byte> ColorValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset, i + colorOffset + 4);
                    std::vector<std::byte> afterValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 4, i + colorCategoryStride);
                    //新准备的COLOR值
                    std::vector<std::byte> newColorValues;

                    if (rgb_r_num != -1) {
                        newColorValues.push_back(patchRgb_R);
                    }
                    else {
                        newColorValues.push_back(MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset, i + colorOffset + 1)[0]);
                    }

                    if (rgb_g_num != -1) {
                        newColorValues.push_back(patchRgb_G);
                    }
                    else {
                        newColorValues.push_back(MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 1, i + colorOffset + 2)[0]);
                    }

                    if (rgb_b_num != -1) {
                        newColorValues.push_back(patchRgb_B);
                    }
                    else {
                        newColorValues.push_back(MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 2, i + colorOffset + 3)[0]);
                    }

                    if (rgb_a_num != -1) {
                        newColorValues.push_back(patchRgb_A);
                    }
                    else {
                        newColorValues.push_back(MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 3, i + colorOffset + 4)[0]);
                    }

                    newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), beforeValues.begin(), beforeValues.end());
                    newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), newColorValues.begin(), newColorValues.end());
                    newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), afterValues.begin(), afterValues.end());
                }

                finalVBCategoryDataMap[colorCategory] = newTexcoordCategoryValues;
                LOG.NewLine();
            }

            
        }
        else if (colorElement.Format == DXGI_FORMAT::R32G32B32A32_FLOAT) {
            //准备好数字用于替换
            float rgb_r_num = -1;
            float rgb_g_num = -1;
            float rgb_b_num = -1;
            float rgb_a_num = -1;

            //尝试进行格式转换
            if (L"default" != rgb_r) {
                rgb_r_num = std::stoi(rgb_r) / 255.0f;
            }
            if (L"default" != rgb_g) {
                rgb_g_num = std::stoi(rgb_g) / 255.0f;
            }
            if (L"default" != rgb_b) {
                rgb_b_num = std::stoi(rgb_b) / 255.0f;
            }
            if (L"default" != rgb_a) {
                rgb_a_num = std::stoi(rgb_a) / 255.0f;
            }

            LOG.Info(L"rgb_r_num: " + std::to_wstring(rgb_r_num));
            LOG.Info(L"rgb_g_num: " + std::to_wstring(rgb_g_num));
            LOG.Info(L"rgb_b_num: " + std::to_wstring(rgb_b_num));
            LOG.Info(L"rgb_a_num: " + std::to_wstring(rgb_a_num));

            //如果都是default那干脆啥也别干直接返回
            if (rgb_a_num == -1 && rgb_g_num == -1 && rgb_b_num == -1 && rgb_a_num == -1) {
                LOG.Info(L"未检测到需要修改的COLOR值分量");
                LOG.NewLine();
            }
            else {
                //TODO 要实现这里的话，就必须先实现D3D11中那些数据格式和byte格式的灵活转换。
             //准备好数字用于写入
                std::vector<std::byte> patchRgb_R = MMTFormat::PackNumberR32_FLOAT_littleIndian(rgb_r_num);
                std::vector<std::byte> patchRgb_G = MMTFormat::PackNumberR32_FLOAT_littleIndian(rgb_g_num);
                std::vector<std::byte> patchRgb_B = MMTFormat::PackNumberR32_FLOAT_littleIndian(rgb_b_num);
                std::vector<std::byte> patchRgb_A = MMTFormat::PackNumberR32_FLOAT_littleIndian(rgb_a_num);
                LOG.Info(L"patchRgb_R.size(): " + std::to_wstring(patchRgb_R.size()));
                LOG.Info(L"patchRgb_G.size(): " + std::to_wstring(patchRgb_G.size()));
                LOG.Info(L"patchRgb_B.size(): " + std::to_wstring(patchRgb_B.size()));
                LOG.Info(L"patchRgb_A.size(): " + std::to_wstring(patchRgb_A.size()));

                //确定Color所在的槽位偏移
                int colorOffset = 0;
                for (std::string elementName : basicConfig.TmpElementList) {
                    D3D11Element d3d11Element = d3d11GameType.ElementNameD3D11ElementMap[elementName];

                    //首先必须属于color所在的槽位
                    if (d3d11Element.Category == MMTString::ToByteString(colorCategory)) {
                        if (elementName != "COLOR") {
                            colorOffset += d3d11Element.ByteWidth;
                        }
                        else {
                            break;
                        }
                    }
                }
                LOG.Info(L"colorOffset: " + std::to_wstring(colorOffset));
                LOG.Info(L"colorCategoryStride: " + std::to_wstring(colorCategoryStride));
                LOG.Info(L"colorCategoryValues.size(): " + std::to_wstring(colorCategoryValues.size()));

                //开始更改
                std::vector<std::byte> newTexcoordCategoryValues;
                for (std::size_t i = 0; i < colorCategoryValues.size(); i = i + colorCategoryStride)
                {
                    //先获取COLOR前的所有值
                    std::vector<std::byte> beforeValues = MMTFormat::GetRange_Byte(colorCategoryValues, i, i + colorOffset);
                    std::vector<std::byte> ColorValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset, i + colorOffset + 16);
                    std::vector<std::byte> afterValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 16, i + colorCategoryStride);
                    //新准备的COLOR值
                    std::vector<std::byte> newColorValues;

                    if (rgb_r_num != -1.0f) {
                        newColorValues.insert(newColorValues.end(), patchRgb_R.begin(), patchRgb_R.end());
                    }
                    else {
                        std::vector<std::byte> tmpValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset, i + colorOffset + 4);
                        newColorValues.insert(newColorValues.end(), tmpValues.begin(), tmpValues.end());

                    }

                    if (rgb_g_num != -1.0f) {
                        newColorValues.insert(newColorValues.end(), patchRgb_G.begin(), patchRgb_G.end());

                    }
                    else {
                        std::vector<std::byte> tmpValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 4, i + colorOffset + 8);
                        newColorValues.insert(newColorValues.end(), tmpValues.begin(), tmpValues.end());

                    }

                    if (rgb_b_num != -1.0f) {
                        newColorValues.insert(newColorValues.end(), patchRgb_B.begin(), patchRgb_B.end());
                    }
                    else {
                        std::vector<std::byte> tmpValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 8, i + colorOffset + 12);
                        newColorValues.insert(newColorValues.end(), tmpValues.begin(), tmpValues.end());


                    }

                    if (rgb_a_num != -1.0f) {
                        newColorValues.insert(newColorValues.end(), patchRgb_A.begin(), patchRgb_A.end());
                    }
                    else {
                        std::vector<std::byte> tmpValues = MMTFormat::GetRange_Byte(colorCategoryValues, i + colorOffset + 12, i + colorOffset + 16);
                        newColorValues.insert(newColorValues.end(), tmpValues.begin(), tmpValues.end());

                    }

                    newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), beforeValues.begin(), beforeValues.end());
                    newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), newColorValues.begin(), newColorValues.end());
                    newTexcoordCategoryValues.insert(newTexcoordCategoryValues.end(), afterValues.begin(), afterValues.end());
                }

                finalVBCategoryDataMap[colorCategory] = newTexcoordCategoryValues;
                LOG.NewLine();
            }
        }
        else {
            LOG.Warning(L"不支持的COLOR值格式: " + MMTString::ToWideString(colorElement.Format) + L" 跳过COLOR值重置，请使用VertexColorMaster插件在Blender中手刷COLOR值。");
        }
        LOG.NewLine();
    };




    //TODO 这里的TANGENT计算步长不能写死，必须灵活变化
    void Unity_TANGENT_AverageNormal(std::unordered_map<std::wstring, std::vector<std::byte>>& inputVBCategoryDataMap, D3D11GameType d3d11GameType) {
        LOG.Info(L"Start to recalculate tangent use averageNormal algorithm:");
        std::unordered_map<std::wstring, std::vector<std::byte>> newVBCategoryDataMap;
        //获取Position分类的内容
        std::vector<std::byte> PositionCategoryValues = inputVBCategoryDataMap[L"Position"];

        //获取POSITION和TANGENT
        std::vector<std::vector<double>> POSITION_VALUES;
        std::vector<std::vector<double>> NORMAL_VALUES;

        int positionStride = 0;
        //因为POSITION不会出现像TEXCOORD那样的动态步长，所以这里直接获取OrderedElementList，统计出Position的长度
        for (std::string elementName : d3d11GameType.OrderedFullElementList) {
            D3D11Element d3d11Element = d3d11GameType.ElementNameD3D11ElementMap[elementName];
            if (d3d11Element.Category == "Position") {
                int byteWidth = d3d11Element.ByteWidth;
                positionStride += byteWidth;

            }
        }
        LOG.Info(L"PositionStride: " + std::to_wstring(positionStride));
        for (std::size_t i = 0; i < PositionCategoryValues.size(); i = i + positionStride)
        {
            std::vector<std::byte> POSITION_X = MMTFormat::GetRange_Byte(PositionCategoryValues, i, i + 4);
            //LOG.LogOutput(L"POSITION_X: " + std::to_wstring(bytesToFloat(POSITION_X)));
            std::vector<std::byte> POSITION_Y = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 4, i + 8);
            std::vector<std::byte> POSITION_Z = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 8, i + 12);
            std::vector<double> POSITION_DOUBLE = { MMTFormat::ByteVectorToFloat(POSITION_X), MMTFormat::ByteVectorToFloat(POSITION_Y) ,MMTFormat::ByteVectorToFloat(POSITION_Z) };
            POSITION_VALUES.push_back(POSITION_DOUBLE);

            std::vector<std::byte> NORMAL_X = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 12, i + 16);
            std::vector<std::byte> NORMAL_Y = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 16, i + 20);
            std::vector<std::byte> NORMAL_Z = MMTFormat::GetRange_Byte(PositionCategoryValues, i + 20, i + 24);
            std::vector<double> NORMAL_DOUBLE = { MMTFormat::ByteVectorToFloat(NORMAL_X), MMTFormat::ByteVectorToFloat(NORMAL_Y) ,MMTFormat::ByteVectorToFloat(NORMAL_Z) };
            NORMAL_VALUES.push_back(NORMAL_DOUBLE);
        }
        LOG.Info(L"Start to CalculateAverageNormalsAndStoreTangent");
        //把顶点相同的法线 3维向量 相加然后归一化，作为所有值为这个顶点值的相同向量
        //然后对每个对应的TANGENT，都用这个值作为前3位，0作为最后一位

        //计算出平局法线的TANGENT
        std::vector<std::vector<double>> TANGENT_VALUES = MMTFormat::CalculateAverageNormalsAndStoreTangent(POSITION_VALUES, NORMAL_VALUES);
        LOG.Info(L"TANGENT_VALUES size: " + std::to_wstring(TANGENT_VALUES.size()));
        LOG.Info(L"CalculateAverageNormalsAndStoreTangent success");

        //创建一个新的vector用来装修改后的Position分类的内容
        std::vector<std::byte> newPositionCategoryValues;
        //添加TANGENT到新的newPositionCategoryValues
        int count = 0;
        for (std::size_t i = 0; i < PositionCategoryValues.size(); i = i + positionStride)
        {
            //TODO 这里的positionStride - 16得到的就是除了TANGENT之外的其他的element的总长度
            //因为在HI3Body2中，Position是由POSITION，NORMAL，COLOR，TANGENT组成的，其它的类型则是POSITION，NORMAL，TANGENT
            //所以虽然这样暂时够用了，但是兼容性并不好，以后再来修改吧。
            std::vector<std::byte> POSITION_NORMAL_VALUES = MMTFormat::GetRange_Byte(PositionCategoryValues, i, i + (positionStride - 16));

            std::vector<double> TANGENT_DOUBLES = TANGENT_VALUES[count];

            std::vector<std::byte> TANGENT_X;
            std::vector<std::byte> TANGENT_Y;
            std::vector<std::byte> TANGENT_Z;
            std::vector<std::byte> TANGENT_W;

            double tangent_double_x = TANGENT_DOUBLES[0];

            TANGENT_X = MMTFormat::PackNumberR32_FLOAT_littleIndian((float)TANGENT_DOUBLES[0]);
            TANGENT_Y = MMTFormat::PackNumberR32_FLOAT_littleIndian((float)TANGENT_DOUBLES[1]);
            TANGENT_Z = MMTFormat::PackNumberR32_FLOAT_littleIndian((float)TANGENT_DOUBLES[2]);
            //TANGENT的W分量要么是-1要么是1，这是由当前顶点的手性决定的，这里我们使用Blender导出时自带的分量试一下
            //TANGENT_W = GetRange(PositionCategoryValues, i + 36, i + 40);
            TANGENT_W = MMTFormat::PackNumberR32_FLOAT_littleIndian((float)TANGENT_DOUBLES[3]);

            //LOG.LogOutput(L"Tangent: " + formatedFloat(TANGENT_DOUBLES[0],10) + L"," + formatedFloat(TANGENT_DOUBLES[1],10) + L"," + formatedFloat(TANGENT_DOUBLES[2], 10) + L"," + formatedFloat(TANGENT_DOUBLES[3], 10));

            newPositionCategoryValues.insert(newPositionCategoryValues.end(), POSITION_NORMAL_VALUES.begin(), POSITION_NORMAL_VALUES.end());

            newPositionCategoryValues.insert(newPositionCategoryValues.end(), TANGENT_X.begin(), TANGENT_X.end());
            newPositionCategoryValues.insert(newPositionCategoryValues.end(), TANGENT_Y.begin(), TANGENT_Y.end());
            newPositionCategoryValues.insert(newPositionCategoryValues.end(), TANGENT_Z.begin(), TANGENT_Z.end());
            newPositionCategoryValues.insert(newPositionCategoryValues.end(), TANGENT_W.begin(), TANGENT_W.end());



            count = count + 1;
            //LOG.LogOutput(L"Count: " + std::to_wstring(count));
        }

        LOG.NewLine();
        inputVBCategoryDataMap[L"Position"] = newPositionCategoryValues;
    };



    //通过读取的参数，翻转Normal的X、Y、Z分量，翻转Tangent的X、Y、Z、W分量
    void Unity_Reverse_NORMAL_TANGENT(
        std::unordered_map<std::wstring, std::vector<std::byte>>& finalVBCategoryDataMap, DrawIBConfig basicConfig, D3D11GameType d3d11GameType) {
        LOG.Info(L"Start to flip NORMAL and TANGENT values");
        std::unordered_map<std::wstring, std::vector<std::byte>> newVBCategoryDataMap;
        std::vector<std::byte> PositionCategoryValues = finalVBCategoryDataMap[L"Position"];

        //这里要拿到TmpElementList，然后统计出属于Position槽位的元素有哪些
        //D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[basicConfig.WorkGameType];
        std::vector<std::string> positionElementList;
        int positionStride = 0;
        for (std::string elementName : basicConfig.TmpElementList) {
            D3D11Element elementObject = d3d11GameType.ElementNameD3D11ElementMap[elementName];
            std::wstring category = MMTString::ToWideString(elementObject.Category);
            if (category == L"Position") {
                LOG.Info(L"Detect Position category element: " + MMTString::ToWideString(elementName) + L"  stride: " + std::to_wstring(elementObject.ByteWidth));
                positionElementList.push_back(elementName);
                positionStride += elementObject.ByteWidth;

            }
        }
        LOG.Info(L"PositionStride: " + std::to_wstring(positionStride));

        // 然后动态的读取值，然后只修改NORMAL和TANGENT值
        std::vector<std::byte> newPositionCategoryValues;
        for (std::size_t i = 0; i < PositionCategoryValues.size(); i = i + positionStride) {

            std::vector<std::byte> newPOSITIONValues;
            //遍历positionElementList，但是只处理NORMAL和TANGENT，其他原封不动放到新列表
            int offset = 0;
            for (std::string elementName : positionElementList) {
                //LOG.LogOutput(L"Tmp  offset: " + std::to_wstring(offset));
                D3D11Element elementObject = d3d11GameType.ElementNameD3D11ElementMap[elementName];
                std::wstring format = MMTString::ToWideString(elementObject.Format);
                int elementStrie = elementObject.ByteWidth;

                //LOG.LogOutput(L"Tmp  format: " + format);
                //LOG.LogOutput(L"Tmp  elementStrie: " + std::to_wstring(elementStrie));


                std::vector<std::byte> TmpValues = MMTFormat::GetRange_Byte(PositionCategoryValues, i + offset, i + offset + elementStrie);
                if (elementName == "NORMAL") {
                    // TODO 拆出分量，根据条件反转对应分量，目前我们NORMAL就两种类型，一种R32G32B32_FLOAT 一种R16G16B16A16_FLOAT而且A16全是0
                    if (format == L"R32G32B32_FLOAT") {
                        std::vector<std::byte> FlipedValues;

                        std::vector<std::byte> ValueR = MMTFormat::GetRange_Byte(TmpValues, 0, 4);
                        std::vector<std::byte> ValueG = MMTFormat::GetRange_Byte(TmpValues, 4, 8);
                        std::vector<std::byte> ValueB = MMTFormat::GetRange_Byte(TmpValues, 8, 12);

                        std::vector<std::byte> ValueR_Reversed = basicConfig.NormalReverseX ? MMTFormat::Reverse32BitFloatValue(ValueR) : ValueR;
                        std::vector<std::byte> ValueG_Reversed = basicConfig.NormalReverseY ? MMTFormat::Reverse32BitFloatValue(ValueG) : ValueG;
                        std::vector<std::byte> ValueB_Reversed = basicConfig.NormalReverseZ ? MMTFormat::Reverse32BitFloatValue(ValueB) : ValueB;

                        FlipedValues.insert(FlipedValues.end(), ValueR_Reversed.begin(), ValueR_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueG_Reversed.begin(), ValueG_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueB_Reversed.begin(), ValueB_Reversed.end());

                        newPOSITIONValues.insert(newPOSITIONValues.end(), FlipedValues.begin(), FlipedValues.end());
                    }
                    else if (format == L"R16G16B16A16_FLOAT") {
                        std::vector<std::byte> FlipedValues;

                        std::vector<std::byte> ValueR = MMTFormat::GetRange_Byte(TmpValues, 0, 2);
                        std::vector<std::byte> ValueG = MMTFormat::GetRange_Byte(TmpValues, 2, 4);
                        std::vector<std::byte> ValueB = MMTFormat::GetRange_Byte(TmpValues, 4, 6);
                        std::vector<std::byte> ValueA = MMTFormat::GetRange_Byte(TmpValues, 6, 8);

                        std::vector<std::byte> ValueR_Reversed = basicConfig.NormalReverseX ? MMTFormat::Reverse16BitShortValue(ValueR) : ValueR;
                        std::vector<std::byte> ValueG_Reversed = basicConfig.NormalReverseY ? MMTFormat::Reverse16BitShortValue(ValueG) : ValueG;
                        std::vector<std::byte> ValueB_Reversed = basicConfig.NormalReverseZ ? MMTFormat::Reverse16BitShortValue(ValueB) : ValueB;

                        FlipedValues.insert(FlipedValues.end(), ValueR_Reversed.begin(), ValueR_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueG_Reversed.begin(), ValueG_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueB_Reversed.begin(), ValueB_Reversed.end());
                        //NORMAL的A分量永远是0，不需要反转
                        FlipedValues.insert(FlipedValues.end(), ValueA.begin(), ValueA.end());

                        newPOSITIONValues.insert(newPOSITIONValues.end(), FlipedValues.begin(), FlipedValues.end());
                    }
                    else {
                        newPOSITIONValues.insert(newPOSITIONValues.end(), TmpValues.begin(), TmpValues.end());
                        //LOG.Warning(L"Can't find any format for NORMAL vlaues,use original NORMAL values");
                    }

                }
                else if (elementName == "TANGENT") {

                    if (format == L"R32G32B32A32_FLOAT") {
                        std::vector<std::byte> FlipedValues;

                        std::vector<std::byte> ValueR = MMTFormat::GetRange_Byte(TmpValues, 0, 4);
                        std::vector<std::byte> ValueG = MMTFormat::GetRange_Byte(TmpValues, 4, 8);
                        std::vector<std::byte> ValueB = MMTFormat::GetRange_Byte(TmpValues, 8, 12);
                        std::vector<std::byte> ValueA = MMTFormat::GetRange_Byte(TmpValues, 12, 16);

                        std::vector<std::byte> ValueR_Reversed = basicConfig.TangentReverseX ? MMTFormat::Reverse32BitFloatValue(ValueR) : ValueR;
                        std::vector<std::byte> ValueG_Reversed = basicConfig.TangentReverseY ? MMTFormat::Reverse32BitFloatValue(ValueG) : ValueG;
                        std::vector<std::byte> ValueB_Reversed = basicConfig.TangentReverseZ ? MMTFormat::Reverse32BitFloatValue(ValueB) : ValueB;
                        std::vector<std::byte> ValueA_Reversed = basicConfig.TangentReverseW ? MMTFormat::Reverse32BitFloatValue(ValueA) : ValueA;

                        FlipedValues.insert(FlipedValues.end(), ValueR_Reversed.begin(), ValueR_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueG_Reversed.begin(), ValueG_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueB_Reversed.begin(), ValueB_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueA_Reversed.begin(), ValueA_Reversed.end());

                        newPOSITIONValues.insert(newPOSITIONValues.end(), FlipedValues.begin(), FlipedValues.end());
                    }
                    else if (format == L"R16G16B16A16_FLOAT") {
                        std::vector<std::byte> FlipedValues;

                        std::vector<std::byte> ValueR = MMTFormat::GetRange_Byte(TmpValues, 0, 2);
                        std::vector<std::byte> ValueG = MMTFormat::GetRange_Byte(TmpValues, 2, 4);
                        std::vector<std::byte> ValueB = MMTFormat::GetRange_Byte(TmpValues, 4, 6);
                        std::vector<std::byte> ValueA = MMTFormat::GetRange_Byte(TmpValues, 6, 8);

                        std::vector<std::byte> ValueR_Reversed = basicConfig.TangentReverseX ? MMTFormat::Reverse16BitShortValue(ValueR) : ValueR;
                        std::vector<std::byte> ValueG_Reversed = basicConfig.TangentReverseY ? MMTFormat::Reverse16BitShortValue(ValueG) : ValueG;
                        std::vector<std::byte> ValueB_Reversed = basicConfig.TangentReverseZ ? MMTFormat::Reverse16BitShortValue(ValueB) : ValueB;

                        std::vector<std::byte> ValueA_Reversed = basicConfig.TangentReverseW ? MMTFormat::Reverse16BitShortValue(ValueA) : ValueA;
                        if (ValueA[0] == std::byte(0x00) && ValueA[1] == std::byte(0xBC)) {
                            ValueA_Reversed[0] = std::byte(0x00);
                            ValueA_Reversed[1] = std::byte(0x3C);
                        }
                        else if (ValueA[0] == std::byte(0x00) && ValueA[1] == std::byte(0x3C)) {
                            ValueA_Reversed[0] = std::byte(0x00);
                            ValueA_Reversed[1] = std::byte(0xBC);
                        }

                        FlipedValues.insert(FlipedValues.end(), ValueR_Reversed.begin(), ValueR_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueG_Reversed.begin(), ValueG_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueB_Reversed.begin(), ValueB_Reversed.end());
                        FlipedValues.insert(FlipedValues.end(), ValueA_Reversed.begin(), ValueA_Reversed.end());

                        newPOSITIONValues.insert(newPOSITIONValues.end(), FlipedValues.begin(), FlipedValues.end());
                    }
                    else {
                        newPOSITIONValues.insert(newPOSITIONValues.end(), TmpValues.begin(), TmpValues.end());
                        //LOG.Warning(L"Can't find any format for TANGENT vlaues,use default TANGENT values.");
                    }

                }
                else {
                    //不反转的话直接添加进去就行了
                    newPOSITIONValues.insert(newPOSITIONValues.end(), TmpValues.begin(), TmpValues.end());

                }
                offset += elementStrie;

            }

            //处理完的POSITION values添加到总的Position槽位里
            newPositionCategoryValues.insert(newPositionCategoryValues.end(), newPOSITIONValues.begin(), newPOSITIONValues.end());
        }

        LOG.Info(L"Size: newPositionCategoryValues : " + std::to_wstring(newPositionCategoryValues.size()));
        LOG.Info(L"Size: PositionCategoryValues : " + std::to_wstring(PositionCategoryValues.size()));

        finalVBCategoryDataMap[L"Position"] = newPositionCategoryValues;
        LOG.NewLine();
    };


}