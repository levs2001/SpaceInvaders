#include "TextSystem.h"
#include "tinyxml\tinyxml.h"
#include <iostream>
#include <stdlib.h>
//#include "..\Game.h"

static TiXmlDocument InitFile(std::string fileName);
static void SetLevel(Level& level, TiXmlNode* pParent);
static void AddRow(Level& level, TiXmlElement* rowEl, int yCentr);
static void AddTimeToMove(Level& level, TiXmlElement* timeToMoveEl);
static void AddTimeToShot(Level& level, TiXmlElement* timeToShotEl);
static TimeAliensC GetTimeToAliensC(Level& level, TiXmlElement* timeToEl);

void SetLevelsFromFile(std::deque<Level>& levels, std::string fileName) {
	TiXmlDocument xmlFile = InitFile(fileName);

	TiXmlNode* xmlFileNode = &xmlFile;
	xmlFileNode = xmlFileNode->FirstChild();

	while (xmlFileNode) {
		int type = xmlFileNode->Type();

		if (type == TiXmlNode::TINYXML_ELEMENT) {
			if ((std::string)xmlFileNode->Value() == "level") {
				levels.push_back(Level());
				SetLevel(levels.back(), xmlFileNode->FirstChild());
			}
		}
		xmlFileNode = xmlFileNode->NextSibling();
	}
}

static void SetLevel(Level& level, TiXmlNode* levelEl) {
	if (!&levelEl) return;
	size_t row_count = 0;

	int type = levelEl->Type();
	while (levelEl) {
		if ((std::string)levelEl->Value() == "row") {
			AddRow(level, levelEl->ToElement(), (row_count + 1) * ALIEN_DEFAULT_SIZE / 2 + row_count * ALIEN_DIST_Y + FIELD_MIN_Y);
			row_count++;
		}
		else if ((std::string)levelEl->Value() == "timeToMove")
			AddTimeToMove(level, levelEl->ToElement());
		else if ((std::string)levelEl->Value() == "timeToShot")
			AddTimeToShot(level, levelEl->ToElement());

		levelEl = levelEl->NextSibling();
	}
}

static void AddRow(Level& level, TiXmlElement* rowEl, int yCentr) {
	size_t xSize = 0;
	size_t ySize = 0;

	TiXmlAttribute* pAttrib = rowEl->FirstAttribute();

	while (pAttrib) {
		if ((std::string)pAttrib->Name() == "aSizeX") {
			xSize = atoi(pAttrib->Value());
		}
		else if ((std::string)pAttrib->Name() == "aSizeY") {
			ySize = atoi(pAttrib->Value());
		}
		pAttrib = pAttrib->Next();
	}

	Row row(yCentr, xSize, ySize);
	level.rows.push_back(row);
}

static void AddTimeToMove(Level& level, TiXmlElement* timeToMoveEl) {
	TimeAliensC timeAliensC = GetTimeToAliensC(level, timeToMoveEl);
	level.timesToMove.push_back(timeAliensC);
}

static void AddTimeToShot(Level& level, TiXmlElement* timeToShotEl) {
	TimeAliensC timeAliensC = GetTimeToAliensC(level, timeToShotEl);
	level.timesToShot.push_back(timeAliensC);
}

static TimeAliensC GetTimeToAliensC(Level& level, TiXmlElement* timeToEl) {
	size_t time = 0;
	size_t aliensC = 0;

	TiXmlAttribute* pAttrib = timeToEl->FirstAttribute();

	while (pAttrib) {
		if ((std::string)pAttrib->Name() == "time") {
			time = atoi(pAttrib->Value());
		}
		else if ((std::string)pAttrib->Name() == "aliensC") {
			aliensC = atoi(pAttrib->Value());
		}
		pAttrib = pAttrib->Next();
	}

	TimeAliensC timeAliensC = { time, aliensC };
	return timeAliensC;
}

static TiXmlDocument InitFile(std::string fileName) {
	TiXmlDocument xmlFile = TiXmlDocument(fileName.c_str());
	bool loadOkay = xmlFile.LoadFile();
	if (loadOkay)
		return xmlFile;
	else
		throw "Can't load file";
}

//void TextSystem::Init(const string pFilename) {
//	xmlFile = TiXmlDocument(pFilename.c_str());
//	bool loadOkay = xmlFile.LoadFile();
//	if (loadOkay) {
//		SetMenuFromFile(&xmlFile);
//	}
//	else {
//		cout << "Failed to load file " << pFilename << endl;
//	}
//}

//void TextSystem::SetMenuFromFile(TiXmlNode* pParent) {
//	if (!&pParent) return;
//
//	TiXmlNode* pChild;
//	TiXmlText* pText;
//	int type = pParent->Type();
//	size_t attrCount;
//
//	if (type == TiXmlNode::TINYXML_ELEMENT) {
//		if ((string)pParent->Value() == "menu")
//			//SetMenuBlock(pParent->ToElement());
//		else if ((string)pParent->Value() == "button")
//			//SetButton(pParent->ToElement());
//	}
//
//	for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) {
//		SetMenuFromFile(pChild);
//	}
//}

//void TextSystem::SetMenuBlock(TiXmlElement* pElement) {
//	if (!pElement) return;
//	MenuBlock menuBlock;
//
//	string name = "";
//	classXY coord;
//	classXY size;
//
//	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
//
//	while (pAttrib) {
//		if ((string)pAttrib->Name() == "name") {
//			name = pAttrib->Value();
//		}
//		else if ((string)pAttrib->Name() == "xCoord") {
//			coord.x = stof(pAttrib->Value());
//		}
//		else if ((string)pAttrib->Name() == "yCoord") {
//			coord.y = stof(pAttrib->Value());
//		}
//		else if ((string)pAttrib->Name() == "xSize") {
//			size.x = stof(pAttrib->Value());
//		}
//		else if ((string)pAttrib->Name() == "ySize") {
//			size.y = stof(pAttrib->Value());
//		}
//		pAttrib = pAttrib->Next();
//	}
//
//	menuBlock.SetName(name);
//	menuBlock.SetCoord(coord);
//	menuBlock.SetSize(size);
//	menues.push_back(menuBlock);
//}
//
//void TextSystem::SetButton(TiXmlElement* pElement) {
//	if (!pElement) return;
//	string name = "";
//	classXY coord;
//	classXY size;
//	string menuName = "";
//	string submenuName = "";
//
//	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
//
//	while (pAttrib) {
//		if ((string)pAttrib->Name() == "name") {
//			name = pAttrib->Value();
//		}
//		else if ((string)pAttrib->Name() == "xCoord") {
//			coord.x = stof(pAttrib->Value());
//		}
//		else if ((string)pAttrib->Name() == "yCoord") {
//			coord.y = stof(pAttrib->Value());
//		}
//		else if ((string)pAttrib->Name() == "xSize") {
//			size.x = stof(pAttrib->Value());
//		}
//		else if ((string)pAttrib->Name() == "ySize") {
//			size.y = stof(pAttrib->Value());
//		}
//		else if ((string)pAttrib->Name() == "menuName") {
//			menuName = pAttrib->Value();
//		}
//		else if ((string)pAttrib->Name() == "submenuName") {
//			submenuName = pAttrib->Value();
//		}
//		pAttrib = pAttrib->Next();
//	}
//
//	MenuBlock* menu = FindMenuByName(menuName);
//	MenuBlock* submenu = FindMenuByName(submenuName);
//	MyButton button(name, coord, size, submenu);
//	if (menu != nullptr)
//		menu->AddBut(button);
//}