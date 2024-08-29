#include "MigotoFormat.h"
#include <iostream>


//--------------------------------------------------------------------------------------------------------------------
M_Condition::M_Condition() {

}


bool M_Condition::isActive(std::unordered_map<std::wstring, std::wstring> ActiveKeyValueMap) {
	//根据key组合，判断其中每一个Expression是否生效
	//这里分几种情况分别进行判断
	//LOG.NewLine();
	//LOG.Info("Start to calculate if condition is active:");
	if (this->LogicList.size() == 0) {
		//LOG.Info("LogicList is empty, only one single expression detected:");
		//没有逻辑符号说明只有一个条件
		M_ConditionExpression condition_expression = this->ConditionExpressionList[0];

		if (condition_expression.VarValue == ActiveKeyValueMap[condition_expression.VarName]) {
			//LOG.Info(L"Expression VarName: " + condition_expression.VarName);
			//LOG.Info(L"Expression VarValue: " + condition_expression.VarValue);
			//LOG.Info("Find match key value! expression active!");
			return true;
		}
		else {
			//LOG.Info(L"Match Key Value: " + ActiveKeyValueMap[condition_expression.VarName]);
			//LOG.Info("Can't find match key value in ActiveKeyValueMap, expression not active!");
			return false;
		}
	}
	else {
		//LOG.Info("LogicList is not empty, multiple expression detected:");
		//有逻辑符号说明有多个条件，这里又要分多种情况进行处理，一种是只有&&的情况，一种是&& 与 || 并存的情况
		//TODO 由于我们目前遇到的Mod大多数都只使用到了简单的&&逻辑，所以暂时并不支持||

		//查找是否出现了||
		bool findOrLogic = false;
		for (std::wstring LogicStr : this->LogicList) {
			if (LogicStr == L"||") {
				findOrLogic = true;
			}
		}
		if (!findOrLogic) {
			//如果没找到 || 说明所有的Expression都必须生效才行
			bool allLogicMatch = true;

			//TODO 
			for (M_ConditionExpression condition_expression : this->ConditionExpressionList) {
				//LOG.Info(L"Expression VarName: " + condition_expression.VarName);
				//LOG.Info(L"Expression VarValue: " + condition_expression.VarValue);
				if (condition_expression.VarValue != ActiveKeyValueMap[condition_expression.VarName]) {
					//LOG.Info(L"Match Key Value: " + ActiveKeyValueMap[condition_expression.VarName]);
					allLogicMatch = false;
					break;
				}
			}

			if (allLogicMatch) {
				//LOG.Info("All key value match, condition active.");
				return true;
			}
			else {
				//LOG.Info("Not all key value match in all && logic, condition active fail.");
				return false;
			}


		}
		else {
			//在含有||时，如果正好大小为LogicList大小+1，说明全是||，这时候可以直接任意一个生效则整体生效
			if (this->ConditionExpressionList.size() == this->LogicList.size() + 1) {
				for (M_ConditionExpression condition_expression : this->ConditionExpressionList) {
					LOG.Info(L"Expression VarName: " + condition_expression.VarName);
					LOG.Info(L"Expression VarValue: " + condition_expression.VarValue);

					if (condition_expression.VarValue == ActiveKeyValueMap[condition_expression.VarName]) {
						return true;
					}
				}
			}
			else {
				LOG.Error("Currently Can't Parse && and || in logic expression in the same condition, please contact NicoMico to fix this or waiting for later version update.");
			}

		}

	}
	LOG.NewLine();
	return false;
}

//用于检测并分割字符串中包含&& 或 || 
std::vector<std::wstring> split_logic_get_expression(const std::wstring& expression) {
	std::vector<std::wstring> result;
	std::wstring current;

	for (size_t i = 0; i < expression.length(); i++) {
		if (expression[i] == '&' && expression[i + 1] == '&') {
			if (!current.empty()) {
				boost::algorithm::trim(current);
				result.push_back(current);
				current.clear();
			}
			i++;
		}
		else if (expression[i] == '|' && expression[i + 1] == '|') {
			if (!current.empty()) {
				boost::algorithm::trim(current);
				result.push_back(current);
				current.clear();
			}
			i++;
		}
		else {
			if (expression[i] != '|' && expression[i] != '&') {
				current += expression[i];
			}
		}
	}

	if (!current.empty()) {
		result.push_back(current);
	}

	return result;
}


std::vector<std::wstring> split_logic_get_logic(const std::wstring& expression) {
	std::vector<std::wstring> result;
	std::wstring current;
	for (size_t i = 0; i < expression.length(); i++) {
		if (expression[i] == '&' && expression[i + 1] == '&') {
			result.push_back(L"&&");
			i++;
		}
		else if (expression[i] == '|' && expression[i + 1] == '|') {
			result.push_back(L"||");
			i++;
		}
	}
	return result;
}


//传入if或者else if后面的条件判断部分字符串，随后解析出其中涉及的变量和逻辑
M_Condition::M_Condition(std::wstring ConditionStr) {

	LOG.NewLine();
	LOG.Info(L"Start to parse condition expression for: " + ConditionStr);
	//首先查找是否含有&& 或者 || 这种类型符号，如果找到了那就按多个进行处理，否则按单个进行处理
	bool singleExpression = true;
	if (ConditionStr.find(L"&&") != std::wstring::npos) {
		singleExpression = false;
	}
	else if (ConditionStr.find(L"||") != std::wstring::npos) {
		singleExpression = false;
	}

	if (singleExpression) {
		LOG.Info("Can't find && or || in ConditionStr, take it as a single expression condition.");
		//首先偷懒，使用= 或 ==分割，如果分割出来的ConditionVarValue里没有再出现== !=或者变量$这类的东西，说明正常运作
		IniLineObject conditionLine(ConditionStr);
		M_ConditionExpression conditionExpression;
		conditionExpression.VarName = conditionLine.LeftStrTrim;
		conditionExpression.VarValue = conditionLine.RightStrTrim;
		this->ConditionExpressionList.push_back(conditionExpression);
	}
	else {
		LOG.Info("find && or || in ConditionStr, take it as a multiple expression condition.");
		//这里要逐个查找&& 或者 ||并进行分割，得到下一部分，再继续分割得到下一部分，直到找不到&& 或 ||为止
		std::vector<std::wstring> result = split_logic_get_expression(ConditionStr);
		for (std::wstring condition_str : result) {
			LOG.Info(L"ConditionStr: " + condition_str);
			//这里要分情况考虑，如果只是单纯的&&，那么完全可以放到Condition列表里
			//但是如果包含||怎么办？这时候两个有一个生效都会导致整个ConditionMap都生效。
			IniLineObject conditionLine(condition_str);
			M_ConditionExpression conditionExpression;
			conditionExpression.VarName = conditionLine.LeftStrTrim;
			conditionExpression.VarValue = conditionLine.RightStrTrim;
			this->ConditionExpressionList.push_back(conditionExpression);
		}
		this->LogicList = split_logic_get_logic(ConditionStr);
	}

	//检查ConditionVarValue中是否含有变量名，逻辑符号等，进行更复杂的解析判断。
	LOG.Info(L"Parse condition expression over.");
	LOG.NewLine();
}

void M_Condition::show() {

}


//--------------------------------------------------------------------------------------------------------------------
IniLineObject::IniLineObject() {

}

IniLineObject::IniLineObject(std::wstring readLine) {
	int firstDoubleEqualIndex = (int)readLine.find(L"==");
	int firstEqualIndex = (int)readLine.find(L"=");
	//LOG.Info(L"firstDoubleEqualIndex: " + std::to_wstring(firstDoubleEqualIndex));
	//LOG.Info(L"firstEqualIndex: " + std::to_wstring(firstEqualIndex));
	//默认使用==，如果==找不到，那就换成=
	std::wstring delimiter = L"==";
	if (firstDoubleEqualIndex == std::wstring::npos) {
		delimiter = L"=";
	}

	//找到了==或者找到了=都可以接受
	if (firstEqualIndex != std::wstring::npos || firstDoubleEqualIndex != std::wstring::npos) {
		std::vector<std::wstring> lowerReadLineSplitList = MMTString::SplitStringOnlyMatchFirst(readLine, delimiter);
		if (lowerReadLineSplitList.size() < 2) {
			LOG.Error(L"lowerReadLineSplitList size is " + std::to_wstring(lowerReadLineSplitList.size()) + L",please check!");
		}
		std::wstring leftStr = lowerReadLineSplitList[0];
		std::wstring rightStr = lowerReadLineSplitList[1];
		//LOG.Info(L"leftStr:" + leftStr);
		//LOG.Info(L"rightStr:" + rightStr);
		LeftStr = leftStr;
		RightStr = rightStr;
		boost::algorithm::trim(leftStr);
		boost::algorithm::trim(rightStr);
		LeftStrTrim = leftStr;
		RightStrTrim = rightStr;
		valid = true;
	}
	else {
		LeftStr = L"";
		RightStr = L"";
		LeftStrTrim = L"";
		RightStrTrim = L"";
		valid = false;
	}
}

//下面这个留着指定分隔符，上面那个用来判断变量相关的时候
IniLineObject::IniLineObject(std::wstring readLine, std::wstring delimiter) {
	int firstEqualIndex = (int)readLine.find_first_of(delimiter);
	if (firstEqualIndex != std::wstring::npos) {
		std::vector<std::wstring> lowerReadLineSplitList = MMTString::SplitStringOnlyMatchFirst(readLine, delimiter);
		std::wstring leftStr = lowerReadLineSplitList[0];
		std::wstring rightStr = lowerReadLineSplitList[1];
		LeftStr = leftStr;
		RightStr = rightStr;
		boost::algorithm::trim(leftStr);
		boost::algorithm::trim(rightStr);
		LeftStrTrim = leftStr;
		RightStrTrim = rightStr;
		valid = true;
	}
	else {
		LeftStr = L"";
		RightStr = L"";
		LeftStrTrim = L"";
		RightStrTrim = L"";
		valid = false;
	}
}


//--------------------------------------------------------------------------------------------------------------------
M_IniSection::M_IniSection() {

}


//--------------------------------------------------------------------------------------------------------------------
M_Variable::M_Variable() {

}


M_Variable::M_Variable(std::wstring InNameSpace, std::wstring InVariableName, std::wstring InType) {
	this->NameSpace = InNameSpace;
	this->VariableName = InVariableName;
	this->NamespacedVarName = this->NameSpace + L"\\" + this->VariableName;
	this->Type = InType;
}


M_Variable::M_Variable(std::wstring InNameSpace, std::wstring InVariableName, std::wstring InInitializeValue, std::wstring InType) {
	this->NameSpace = InNameSpace;
	this->VariableName = InVariableName;
	this->NamespacedVarName = this->NameSpace + L"\\" + this->VariableName;
	this->InitializeValue = InInitializeValue;
	this->Type = InType;
}


//--------------------------------------------------------------------------------------------------------------------
M_DrawIndexed::M_DrawIndexed() {

}

//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------