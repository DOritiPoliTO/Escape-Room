#include "book.h"
#include "../../game.h"

Book::Book(const char* bodyFilename, const char* modelFilename, const Vector3f& position,
	const Vector3f& rotationAxis, float rotationAngle) :
	flag_(false),
	pDiaryEntry_(nullptr),
	Prop(bodyFilename, modelFilename, true, true, position, rotationAxis, rotationAngle)
{
	pDiaryEntry_ = new Diary::Entry(
		"../EscapeRoom/data/models/book.mdl",
		"I have found a book which has been printed in\n1888 and is entitled 'The Eight Knights'"
	);
	pDiaryEntry_->getShadedModel3D()->position_.y = 0.0f;
	pDiaryEntry_->getShadedModel3D()->scale_ = 0.5f;
}

Book::~Book()
{
	if (pDiaryEntry_ != nullptr)
	{
		delete pDiaryEntry_;
		pDiaryEntry_ = nullptr;
	}
}

bool Book::use(Prop* pOtherProp)
{
	if (!flag_)
	{
		Game::getInstance().playVideo(
			"../EscapeRoom/data/videos/book_animation.mkv",
			std::bind(
				&Book::onVideoEnd,
				this
			)
		);

		flag_ = true;
	}

	return false;
}

void Book::onVideoEnd()
{
	Game::getInstance().addEntryInDiary(pDiaryEntry_);
}
