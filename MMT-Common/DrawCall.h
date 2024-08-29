#pragma once

#include <string>
#include <vector>

class DrawCall {
public:
	std::wstring DrwaIndex;
	std::vector<std::wstring> APICallStringList;
	std::vector<std::wstring> DumpCallStringList;

	DrawCall() {};
};
