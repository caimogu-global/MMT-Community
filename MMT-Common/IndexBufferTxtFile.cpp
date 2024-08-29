#include "IndexBufferTxtFile.h"
#include <set>
#include "MMTFileUtils.h"
#include "MMTStringUtils.h"
#include "MMTFormatUtils.h"


IndexBufferTxtFile::IndexBufferTxtFile() {

}


IndexBufferTxtFile::IndexBufferTxtFile(std::wstring fileName,bool readIBData) {
    std::filesystem::path filePath(fileName);
    this->FileName = filePath.filename().wstring();
    this->Index = this->FileName.substr(0, 6);
    this->Hash = this->FileName.substr(11, 8);

    //LOG.LogOutput("Index Buffer FileName: " + FileName);
    std::wstring line;
    std::wifstream file(fileName);
    int count = 0;

    std::set<uint32_t> uniqueVertexCountSet;
    while (std::getline(file, line)) {
        std::wstring topologyStr = L"topology:";
        std::wstring firstIndexStr = L"first index:";
        std::wstring indexCountStr = L"index count:";
        std::wstring formatStr = L"format:";
        std::wstring byteOffsetStr = L"byte offset:";
        if (boost::algorithm::starts_with(line, topologyStr)) {
            std::wstring topology = line.substr(topologyStr.length());
            boost::algorithm::trim(topology);
            this->Topology = topology;
        }
        else if (boost::algorithm::starts_with(line, firstIndexStr)) {
            std::wstring firstIndex = line.substr(firstIndexStr.length());
            boost::algorithm::trim(firstIndex);
            this->FirstIndex = firstIndex;
        }
        else if (boost::algorithm::starts_with(line, indexCountStr)) {
            std::wstring indexCount = line.substr(indexCountStr.length());
            boost::algorithm::trim(indexCount);
            this->IndexCount = indexCount;
        }
        else if (boost::algorithm::starts_with(line, formatStr)) {
            std::wstring formatLine = line.substr(formatStr.length());
            boost::algorithm::trim(formatLine);
            this->Format = formatLine;
        }
        else if (boost::algorithm::starts_with(line, byteOffsetStr)) {
            std::wstring byteOffsetLine = line.substr(byteOffsetStr.length());
            boost::algorithm::trim(byteOffsetLine);
            this->ByteOffset = byteOffsetLine;
        }
        else {

            if (!readIBData) {
                if (count > 8) {
                    //LOG.LogOutput("大于第5行，退出");
                    break;
                }
            }
            else {
                //如果要读取IB的内容，就在这里判断如果split 空格长度为3，则转换成UINT32的std::byte然后放入Buffer
                std::vector<std::wstring> splitLine = MMTString::SplitString(line,L" ");
                if (splitLine.size() == 3) {
                    for (std::wstring splitLineStr: splitLine) {
                        uint32_t tmpNumber = std::stoi(splitLineStr);
                        uniqueVertexCountSet.insert(tmpNumber);
                        this->IBTxtToBufMap[IndexNumberCount] = MMTFormat::Int_To_ByteVector(tmpNumber);
                        this->NumberList.push_back(tmpNumber);
                        IndexNumberCount++;
                        //LOG.Info(std::to_string(tmpNumber));
                        if (tmpNumber > this->MaxNumber) {
                            this->MaxNumber = tmpNumber;
                        }
                        if (tmpNumber < this->MinNumber) {
                            this->MinNumber = tmpNumber;
                        }
                    }
                }
            }

        }
        
        count++;
    }
    file.close();

    this->UniqueVertexCount = (uint32_t) uniqueVertexCountSet.size();

    //LOG.Info(L"IndexBuffer Parse Over");
}


void IndexBufferTxtFile::SaveToFile_UINT32(std::wstring FileWritePath, uint32_t Offset) {
    std::ofstream file(FileWritePath, std::ios::binary);
    for (const auto& data : this->NumberList) {
        uint32_t paddedData = data + Offset;
        file.write(reinterpret_cast<const char*>(&paddedData), sizeof(uint32_t));
    }
    file.close();
}