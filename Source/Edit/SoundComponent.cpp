#include "SoundComponent.h"
#include "Types.h"
#include "Print.h"
#include "Math.h"
#include "VideoPlayerComponent.h"


SoundComponent* SoundComponent::GetInstance()
{
	static SoundComponent object;
	return &object;
}

void SoundComponent::LoadUiSounds(const char* clickPath, const char* cursorPath)
{
	UnloadSound(click);
	UnloadSound(cursor);
	click = LoadSound(clickPath);
	cursor = LoadSound(cursorPath);
}

void SoundComponent::LoadCurrentMusic(const char* path)
{
	UnloadMusicStream(currentMusic);
	currentMusic = LoadMusicStream(path);
	currentMusic.looping = false;
	stateSound = StateSound::Stop;
}

void SoundComponent::ClearMusics()
{
	musics.clear();
}

void SoundComponent::LoadPathMusics(const char* path)
{
	ClearMusics();
	FilePathList files = LoadDirectoryFiles(path);
	for (unsigned int i = 0; i < files.count; i++)
	{
		//PRINT_STRING(TextFormat("teste %s", files.paths[i]));
		musics.push_back(files.paths[i]);
	}
	if (musics.size() > 1) 
	{
		const int size = static_cast<int> ( Math::Random(0.0f, static_cast<float>(musics.size())) );
		LoadCurrentMusic(musics[size].c_str());
	}
	else if (musics.size() == 1) 
	{
		LoadCurrentMusic(musics[0].c_str());
	}
	UnloadDirectoryFiles(files);
}

void SoundComponent::BeginPlay()
{
	LoadPathMusics(PATH_SOUND);
}

void SoundComponent::Tick()
{

	if (VideoPlayerComponent::GetInstance()->IsVideoPlaying())
	{
		PauseMusic();
	}
	if (IsMusicReady(currentMusic))
	{
		UpdateMusicStream(currentMusic);

		if (!IsMusicStreamPlaying(currentMusic) && stateSound == StateSound::Ended)
		{
			const int size = static_cast<int> (Math::Random(0.0f, static_cast<float>(musics.size())));
			LoadCurrentMusic(musics[size].c_str());
			PlayMusic();
		}

		if (GetMusicLength() - 1 < GetMusicPlayed())
		{
			stateSound = StateSound::Ended;
			//LOG(LOGINFO, TextFormat("terminou  %f  %f", GetMusicLength(), GetMusicPlayed()));
		}
		//LOG(LOGINFO, TextFormat("%f  %f", GetMusicLength(), GetMusicPlayed()));
	}
}

void SoundComponent::EndPlay()
{
	ClearMusics();
	UnloadSound(click);
	UnloadSound(cursor);
}

void SoundComponent::PlayMusic()
{
	PlayMusicStream(currentMusic);
	stateSound = StateSound::Playing;
}

void SoundComponent::PlayClick()
{
	PlaySound(click);
}

void SoundComponent::PlayCursor()
{
	PlaySound(cursor);
}

void SoundComponent::PauseMusic()
{
	if (stateSound != StateSound::Ended)
	{
		stateSound = Pause;
		PauseMusicStream(currentMusic);
		return;
	}
	StopMusic();
}

void SoundComponent::StopMusic()
{
	StopMusicStream(currentMusic);
	SeekMusic(0);
	stateSound = StateSound::Stop;
}

void SoundComponent::SeekMusic(const float newLength)
{
	SeekMusicStream(currentMusic, newLength);
}

void SoundComponent::SetMusicVolume(const float volume)
{
	volumeMusic = volume;
	::SetMusicVolume(currentMusic, volume);
}

void SoundComponent::SetClickVolume(const float volume)
{
	volumeClick = volume;
	SetMusicVolume(volume);
}

void SoundComponent::SetCursorVolume(const float volume)
{
	volumeCursor = volume;
	SetMusicVolume(volume);
}

bool SoundComponent::IsMusicPlaying() const
{
	return IsMusicStreamPlaying(currentMusic);
}

float SoundComponent::GetMusicLength() const
{
	return GetMusicTimeLength(currentMusic);
}

float SoundComponent::GetMusicPlayed() const
{
	return GetMusicTimePlayed(currentMusic);
}