#include "asset_manager.h"


AssetManager& AssetManager::getInstance()
{
	static AssetManager instance;
	return instance;
}

AssetManager::AssetInfo::AssetInfo() :
	pAsset(nullptr),
	nReferences(0)
{
}

AssetManager::AssetInfo::AssetInfo(void* pAsset) :
	AssetInfo()
{
	this->pAsset = pAsset;
	this->nReferences = 1;
}
