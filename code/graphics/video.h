#pragma once

extern "C"
{
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include <exception>
#include <thread>
#include <mutex>

class Video
{
public:
	Video(const char* filename, std::function<void(const void*)> copyCallback);
	~Video();

	bool processNextFrame(float deltaTime);

	int getWidth() const;
	int getHeight() const;
	float getRatio() const;
	float getFrameTime() const;

private:
	int frameWidth_;
	int frameHeight_;
	float ratio_;
	float frameTime_;
	float elapsedTime_;
	AVPacket* pAVPacket_;
	AVFormatContext* pAVFormatContext_;
	int videoStreamIndex_;
	AVCodecContext* pAVCodecContext_;
	AVFrame* pRgbAVFrame_;
	AVFrame* pYuvAVFrame_;
	SwsContext* pSwsContext_;
	std::mutex mutex_;
	std::condition_variable conditionVariable_;
	bool shouldDecodeFrame_;
	std::thread* pWorkingThread_;
	int requestedFrameIndex_;
	int currentFrameIndex_;
	int lastDecodedFrameIndex_;
	bool endOfVideo_;
	std::function<void(const void*)> copyCallback_;

	void openFile(const char* filename);
	void workingThreadProc();
};
