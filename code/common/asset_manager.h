#pragma once

#include <map>
#include <typeindex>

class AssetManager
{
public:
	static AssetManager& getInstance();

	template<class T>
	inline void insertAsset(const char* assetName, T* pAsset);

	template<class T>
	inline T* acquireAsset(const char* assetName);

	template<class T>
	inline bool releaseAsset(T* pAsset);

private:
	struct AssetInfo;

	AssetManager() {};
	AssetManager(const AssetManager&) = delete;

	std::map<std::type_index, std::map<const std::string, AssetInfo>> assetMapMap_;
};

struct AssetManager::AssetInfo
{
	void* pAsset;
	size_t nReferences;

	AssetInfo();
	AssetInfo(void* pAsset);
};

template<class T>
inline void AssetManager::insertAsset(const char* assetName, T* pAsset)
{
	auto& assetMap = assetMapMap_[std::type_index(typeid(T))];

	assetMap.insert(std::pair<const std::string, AssetInfo>(assetName, pAsset));
}

template<class T>
inline T* AssetManager::acquireAsset(const char* assetName)
{
	auto& assetMap = assetMapMap_[std::type_index(typeid(T))];

	auto it = assetMap.find(assetName);
	if (it != assetMap.end())
	{
		it->second.nReferences++;

		return reinterpret_cast<T*>(it->second.pAsset);
	}

	return nullptr;
}

template<class T>
inline bool AssetManager::releaseAsset(T* pAsset)
{
	auto& assetMap = assetMapMap_[std::type_index(typeid(T))];

	for (auto it = assetMap.begin(); it != assetMap.end(); it++)
	{
		if (it->second.pAsset == pAsset)
		{
			it->second.nReferences--;
			if (it->second.nReferences == 0)
			{
				assetMap.erase(it);

				return true;
			}

			return false;
		}
	}
}
