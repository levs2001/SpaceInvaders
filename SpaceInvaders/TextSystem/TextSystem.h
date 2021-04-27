#pragma once
#include "tinyxml\tinyxml.h"
#include "..\Game.h"

void SetLevelsFromFile(std::deque<Level>& levels, std::string fileName);