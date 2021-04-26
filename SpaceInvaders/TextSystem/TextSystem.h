#pragma once
#include "tinyxml\tinyxml.h"
#include <iostream>
#include "..\Game.h"

void SetLevelsFromFile(std::deque<Level>& levels, std::string fileName);

//void SetLevelsFromFile(std::deque<Level> levels, std::string fileName);
//class TextSystem {
//public:
	//void Init(const string pFilename);

	//vector<MenuBlock>& GetMenues(){
	//	return menues;
	//}

//private:
	//vector<MenuBlock> menues;
	//TiXmlDocument xmlFile;

	//void SetMenuFromFile(TiXmlNode* pParent);

	//void SetMenuBlock(TiXmlElement* pElement);

	//void SetButton(TiXmlElement* pElement);

	//MenuBlock* FindMenuByName(string name) {
	//	for (auto& menu : menues)
	//		if (menu.GetName() == name)
	//			return &menu;
	//	return nullptr;
	//}
//};