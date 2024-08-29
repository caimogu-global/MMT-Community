#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

class IndexBufferTxtFile {
	public:
		//主要字段
		std::wstring FileName;
		std::wstring Index;
		std::wstring Hash;
		std::wstring FirstIndex = L"";
		std::wstring IndexCount;
		std::wstring Topology;
		std::wstring Format;
		std::wstring ByteOffset;

		uint32_t MaxNumber = 0;
		uint32_t MinNumber = 99999999;
		uint32_t UniqueVertexCount = 0;
		uint32_t IndexNumberCount = 0;


		//顶点列表
		std::vector<uint32_t> NumberList;
		
		std::unordered_map<int, std::vector<std::byte>> IBTxtToBufMap;

		IndexBufferTxtFile();

		IndexBufferTxtFile(std::wstring fileName, bool readIBData);

		void SaveToFile_UINT32(std::wstring FileWritePath, uint32_t Offset);

};