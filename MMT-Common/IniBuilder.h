#pragma once
#include <vector>
#include <string>
#include "MMTConstants.h"



class IniSection {
public:
	IniSectionType SectionType;
	std::vector<std::wstring> SectionLineList;

	IniSection() {};
	IniSection(IniSectionType InputSectionType) {
		this->SectionType = InputSectionType;
	};

	void Append(std::wstring InputLine) {
		this->SectionLineList.push_back(InputLine);
	}

	void NewLine() {
		this->SectionLineList.push_back(L"");
	}

};


class IniBuilder {
private:
	std::vector<std::wstring> LineList;
	void AppendSectionLine(IniSectionType InputIniSectionType);

	std::vector<IniSection> IniSectionList;
public:
	void AppendSection(IniSection InputIniSection);
	void SaveToFile(std::wstring FilePath);

};