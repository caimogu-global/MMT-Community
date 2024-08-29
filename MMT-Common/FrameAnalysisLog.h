#pragma once
#include <string>
#include <vector>
#include <map>

#include "DrawCall.h"


class FrameAnalysisLog {

public:
	std::wstring WorkFolder = L"";

	std::map<std::wstring, DrawCall> Index_DrawCall_Map;

	FrameAnalysisLog();
	
	FrameAnalysisLog(std::wstring WorkFolder);

	int GetIndexDrawCallPsSetShaderResourcesTime(std::wstring Index);
	uint32_t GetIndexDrawCallPsSetConstantBuffersTime(std::wstring Index);

	std::wstring FindRealDedupedResourceFileNameFromDumpCallLog(std::wstring Index, std::wstring FakeFileName);

	uint32_t FindRealDrawIndexedNumberFromLogByIndex(std::wstring Index);
	uint32_t FindRealDrawIndexedFirstIndexFromLogByIndex(std::wstring Index);
};