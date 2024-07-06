

#include "VideoPlayerComponent.h"

#include <string>
#include "Math.h"
#include "UtilsFunctionLibrary.h"
#include "Render.h"
#include "SoundComponent.h"


VideoPlayerComponent* VideoPlayerComponent::GetInstance()
{
	static VideoPlayerComponent object;
	return &object;
}

void VideoPlayerComponent::BeginPlay()
{
	videoPlayer.Init("");
	//videoPlayer.SetEndVideoCallback(  EndVideoCallback());
	videoPlayer.SetEndVideoCallback([this]() { this->EndVideoCallback(); });
}

void VideoPlayerComponent::PlayVideo(const char* videoPath)
{
	const std::string normalizedVideoPath = UtilsFunctionLibrary::NormalizePath(videoPath);
	videoPlayer.PlayVideo(normalizedVideoPath.c_str());
}

void VideoPlayerComponent::Tick()
{
	if (!bCustomSize && IsVideoPlaying())
	{
		Vector2 newSize{ Math::Clamp(videoPlayer.GetVideoSize().x, 0, 1280) , Math::Clamp(videoPlayer.GetVideoSize().y, 0, 720) };
		UtilsFunctionLibrary::SetSizeWithProportion(newSize, 1280, 720);
		position.width = newSize.x;
		position.height = newSize.y;
		position.x = Render::GetInstance()->GetScreenWidthGame() / 2 - position.width / 2 ;
	}
}

void VideoPlayerComponent::Draw()
{
	videoPlayer.DrawVideoFrame(position);
}

void VideoPlayerComponent::EndDraw()
{
	videoPlayer.UnloadVideoFrame();
}

void VideoPlayerComponent::EndPlay()
{
	videoPlayer.CloseVideo();
}

void VideoPlayerComponent::Play()
{
	videoPlayer.Play();
}

void VideoPlayerComponent::Pause()
{
	videoPlayer.Pause();
}


void VideoPlayerComponent::SetVolume(int volume)
{
	videoPlayer.SetVolume(volume);
}

void VideoPlayerComponent::SetCustomSize(bool bEnable, Vector2 newSize)
{
	bCustomSize = bEnable;
	if (bCustomSize)
	{
		position.width = newSize.x;
		position.height = newSize.y;
	}
}

std::string VideoPlayerComponent::GetProportion()
{
	const Proportion proportion = videoPlayer.GetProportion();
	return std::to_string(proportion.simplifiedWidth) + ":" + std::to_string(proportion.simplifiedHeight);
}

bool VideoPlayerComponent::IsVideoPlaying()
{
	return videoPlayer.IsVideoPlaying();
}

void VideoPlayerComponent::EndVideoCallback()
{
	SoundComponent::GetInstance()->PlayMusic();
}
