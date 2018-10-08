#pragma once

#include "../prop.h"
#include "../../interface/diary.h"

class Book :public Prop
{
public:
	Book(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
		const Vector3f& rotationAxis, float rotationAngle);
	~Book();

	bool use(Prop* pOtherProp) override;

private:
	bool flag_;
	Diary::Entry* pDiaryEntry_;

	void onVideoEnd();
};
