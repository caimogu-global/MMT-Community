#include "ConstantBufferBufFile.h"
#include "MMTFileUtils.h"
#include "MMTFormatUtils.h"
#include <istream>
#include <fstream>


ConstantBufferBufFile::ConstantBufferBufFile() {

}


ConstantBufferBufFile::ConstantBufferBufFile(std::wstring CBFilePath) {
    int bufFileSize = MMTFile::GetFileSize(CBFilePath);
    int stride = 4;
    int numberCount = bufFileSize / stride;
    std::ifstream ConstantBufferFile(CBFilePath, std::ifstream::binary);
    std::vector<std::byte> Buffer(bufFileSize);
    ConstantBufferFile.read(reinterpret_cast<char*>(Buffer.data()), bufFileSize);
    ConstantBufferFile.close();


    ConstantBufferValue tmpConstantBufferValue;
    for (int i = 0; i < numberCount; i++) {
        std::vector<std::byte> stdByteVector = MMTFormat::GetRange_Byte(Buffer, i * stride, i * stride + stride);
        int num = MMTFormat::STDByteVector_To_UINT32T(stdByteVector);
        //LOG.Info("num:"+std::to_string(num));

        if (i % stride == 0) {
            tmpConstantBufferValue.X = num;
        }
        else if (i % stride == 1) {
            tmpConstantBufferValue.Y = num;
        }
        else if (i % stride == 2) {
            tmpConstantBufferValue.Z = num;
        }
        else if (i % stride == 3) {
            tmpConstantBufferValue.W = num;
        }

        this->lineCBValueMap[i / 4] = tmpConstantBufferValue;
    }


}