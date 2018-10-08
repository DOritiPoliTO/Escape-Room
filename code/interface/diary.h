#pragma once

#include "../graphics/renderer.h"

class Diary
{
public:
	class Entry;

	Diary();
	~Diary();

	void open();
	void addEntry(Entry* pItem);
	void update(float deltaTime);

private:
	Billboard2D* pGridBillboard_;
	Billboard2D* pSelectionBillboard_;
	DirectionalLight* pDirLight_;
	Text2D* pRotateItemText_;
	Text2D* pChooseItemText_;
	Text2D* pDiaryText_;
	std::vector<Entry*> pEntries_;
	int selectedItemIndex_;
	bool rotateItem_;
	float selectionBillboardAlphaTarget_;
	bool flag_;
	float elapsedTime_;
};

class Diary::Entry
{
public:
	Entry(const char* G3DModelFilename, const char* textString);
	~Entry();

	ShadedModel3D* getShadedModel3D();
	Text2D* getText2D();

private:
	ShadedModel3D* pShadedModel3D_;
	Text2D* pText2D_;
};
