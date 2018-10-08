#include "video.h"

Video::Video(const char* videoFilename, std::function<void(const void*)> copyCallback) :
	frameWidth_(-1),
	frameHeight_(-1),
	ratio_(-1),
	pAVFormatContext_(NULL),
	pAVCodecContext_(NULL),
	pRgbAVFrame_(NULL),
	pYuvAVFrame_(NULL),
	pSwsContext_(NULL),
	pAVPacket_(NULL),
	pWorkingThread_(nullptr),
	currentFrameIndex_(-1),
	requestedFrameIndex_(0),
	lastDecodedFrameIndex_(-1),
	copyCallback_(copyCallback)
{
	try
	{
		av_register_all();

		pAVPacket_ = av_packet_alloc();
		if (pAVPacket_ == NULL)
		{
			throw std::exception();
		}

		openFile(videoFilename);

		pWorkingThread_ = new std::thread(&Video::workingThreadProc, this);
	}
	catch (...)
	{
		if (pWorkingThread_ != nullptr)
		{
			pWorkingThread_->join();

			delete pWorkingThread_;
			pWorkingThread_ = nullptr;
		}

		if (pRgbAVFrame_ != NULL)
		{
			av_frame_free(&pRgbAVFrame_);
			pRgbAVFrame_ = NULL;
		}

		if (pYuvAVFrame_ != NULL)
		{
			av_frame_free(&pYuvAVFrame_);
			pYuvAVFrame_ = NULL;
		}

		if (pSwsContext_ != NULL)
		{
			sws_freeContext(pSwsContext_);
			pSwsContext_ = NULL;
		}

		if (pAVCodecContext_ != NULL)
		{
			avcodec_close(pAVCodecContext_);
			pAVCodecContext_ = NULL;
		}

		if (pAVFormatContext_ != NULL)
		{
			avformat_close_input(&pAVFormatContext_);
			pAVFormatContext_ = NULL;
		}

		if (pAVPacket_ != NULL)
		{
			av_packet_free(&pAVPacket_);
			pAVPacket_ = NULL;
		}

		throw;
	}
}

Video::~Video()
{
	if (pWorkingThread_ != nullptr)
	{
		pWorkingThread_->join();

		delete pWorkingThread_;
		pWorkingThread_ = nullptr;
	}

	if (pRgbAVFrame_ != NULL)
	{
		av_frame_free(&pRgbAVFrame_);
		pRgbAVFrame_ = NULL;
	}

	if (pYuvAVFrame_ != NULL)
	{
		av_frame_free(&pYuvAVFrame_);
		pYuvAVFrame_ = NULL;
	}

	if (pSwsContext_ != NULL)
	{
		sws_freeContext(pSwsContext_);
		pSwsContext_ = NULL;
	}

	if (pAVCodecContext_ != NULL)
	{
		avcodec_close(pAVCodecContext_);
		pAVCodecContext_ = NULL;
	}

	if (pAVFormatContext_ != NULL)
	{
		avformat_close_input(&pAVFormatContext_);
		pAVFormatContext_ = NULL;
	}

	if (pAVPacket_ != NULL)
	{
		av_packet_free(&pAVPacket_);
		pAVPacket_ = NULL;
	}
}

void Video::openFile(const char* filename)
{
	// Open video file
	if (avformat_open_input(&pAVFormatContext_, filename, NULL, NULL) != 0)
	{
		throw std::exception();
	}

	// Get information on the streams
	if (avformat_find_stream_info(pAVFormatContext_, NULL) < 0)
	{
		throw std::exception();
	}

	// Find the first stream, decoder, and open the codec
	videoStreamIndex_ = -1;
	AVCodec* pAVCodec = NULL;
	for (unsigned int i = 0u; i < pAVFormatContext_->nb_streams; i++)
	{
		if (pAVFormatContext_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStreamIndex_ = i;

			// Find the decoder
			pAVCodec = avcodec_find_decoder(pAVFormatContext_->streams[i]->codecpar->codec_id);
			if (pAVCodec == NULL)
			{
				throw std::exception();
			}

			// Open the codec
			pAVCodecContext_ = avcodec_alloc_context3(pAVCodec);
			avcodec_parameters_to_context(pAVCodecContext_, pAVFormatContext_->streams[i]->codecpar);
			avcodec_open2(pAVCodecContext_, pAVCodec, NULL);

			break;
		}
	}

	if (videoStreamIndex_ == -1)
	{
		throw std::exception();
	}

	pSwsContext_ = sws_getContext(
		pAVCodecContext_->width,
		pAVCodecContext_->height,
		pAVCodecContext_->pix_fmt,
		pAVCodecContext_->width,
		pAVCodecContext_->height,
		AV_PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);
	if (pSwsContext_ == NULL)
	{
		throw std::exception();
	}

	// Allocate video frame
	pYuvAVFrame_ = av_frame_alloc();
	if (pYuvAVFrame_ == NULL)
	{
		throw std::exception();
	}
	pYuvAVFrame_->format = pAVFormatContext_->streams[videoStreamIndex_]->codecpar->format;
	pYuvAVFrame_->width = pAVCodecContext_->width;
	pYuvAVFrame_->height = pAVCodecContext_->height;
	pYuvAVFrame_->nb_samples;
	pYuvAVFrame_->channels = pAVCodecContext_->channels;
	av_frame_get_buffer(pYuvAVFrame_, 1);

	pRgbAVFrame_ = av_frame_alloc();
	if (pRgbAVFrame_ == NULL)
	{
		throw std::exception();
	}
	pRgbAVFrame_->format = AV_PIX_FMT_RGB24;
	pRgbAVFrame_->width = pAVCodecContext_->width;
	pRgbAVFrame_->height = pAVCodecContext_->height;
	pRgbAVFrame_->nb_samples;
	pRgbAVFrame_->channels = pAVCodecContext_->channels;
	av_frame_get_buffer(pRgbAVFrame_, 1);

	frameWidth_ = pAVCodecContext_->width;
	frameHeight_ = pAVCodecContext_->height;

	frameTime_ = 0.0333f;
	elapsedTime_ = 0.0f;
	ratio_ = (float)frameHeight_ / (float)frameWidth_;
	endOfVideo_ = false;
}

bool Video::processNextFrame(float deltaTime)
{
	if (endOfVideo_)
	{
		return false;
	}

	elapsedTime_ += deltaTime;
	if (elapsedTime_ >= frameTime_)
	{
		mutex_.lock();

		do
		{
			currentFrameIndex_++;

			elapsedTime_ -= frameTime_;

		} while (elapsedTime_ >= frameTime_);

		requestedFrameIndex_ = currentFrameIndex_ + 1;

		if (lastDecodedFrameIndex_ == currentFrameIndex_)
		{
			copyCallback_(pRgbAVFrame_->data[0]);
		}

		mutex_.unlock();

		conditionVariable_.notify_one();

		return true;
	}

	return true;
}

int Video::getWidth() const
{
	return frameWidth_;
}

int Video::getHeight() const
{
	return frameHeight_;
}

float Video::getRatio() const
{
	return ratio_;
}

float Video::getFrameTime() const
{
	return frameTime_;
}

void Video::workingThreadProc()
{
	while (av_read_frame(pAVFormatContext_, pAVPacket_) >= 0)
	{
		if (pAVPacket_->stream_index == videoStreamIndex_)
		{
			avcodec_send_packet(pAVCodecContext_, pAVPacket_);

			if (avcodec_receive_frame(pAVCodecContext_, pYuvAVFrame_) == 0)
			{
				std::unique_lock<std::mutex> uniqueLock(mutex_);

				// Convert image from YUV to RGB
				sws_scale(
					pSwsContext_,
					pYuvAVFrame_->data,
					pYuvAVFrame_->linesize,
					0,
					pAVCodecContext_->height,
					pRgbAVFrame_->data,
					pRgbAVFrame_->linesize
				);

				frameTime_ = (float)pAVCodecContext_->framerate.den / (float)pAVCodecContext_->framerate.num;

				lastDecodedFrameIndex_ = requestedFrameIndex_;

				conditionVariable_.wait(
					uniqueLock,
					[&] {return lastDecodedFrameIndex_ != requestedFrameIndex_; }
				);
			}
		}

		av_packet_unref(pAVPacket_);
	}

	endOfVideo_ = true;
}
