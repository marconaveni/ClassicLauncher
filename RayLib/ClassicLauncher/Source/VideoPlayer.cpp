#include "VideoPlayer.h"

#include <iostream>

#include "Types.h"
#include "Print.h"

// Callback de bloqueio para fornecer o buffer de vídeo
void* lock(void* data, void** p_pixels)
{
	Image* frame = static_cast<Image*>(data);
	*p_pixels = frame->data;
	return nullptr;
}

// Callback de desbloqueio após a cópia do buffer de vídeo
void unlock(void* data, void* id, void* const* p_pixels)
{
	// Nada a fazer aqui
}

// Callback de exibição (não utilizado)
void display(void* data, void* id)
{
	// Nada a fazer aqui
}


void VideoPlayer::Init(const char* path, bool bAutoPlay)
{
	// Inicializar libVLC
	inst = libvlc_new(0, NULL);
	if (!inst)
	{
		LOG(LOGERROR, "is not possible initialize instance libVLC\n");
		return;
	}

	// Abrir o media player
	media = libvlc_media_new_path(inst, path);
	mediaPlayer = libvlc_media_player_new_from_media(media);
	libvlc_media_release(media);

	// Cria uma imagem para armazenar o quadro atual do vídeo
	image = {
		MemAlloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4), // 4 bytes por pixel (RGBA)
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
	};

	libvlc_video_set_callbacks(mediaPlayer, lock, unlock, display, &image);
	libvlc_video_set_format(mediaPlayer, "RGBA", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH * 4);

	// Criar uma textura para o vídeo
	Image imageDefault = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
	textureDefault = LoadTextureFromImage(imageDefault);
	UnloadImage(imageDefault);

	if (bAutoPlay)
	{
		Play();
	}
}

VideoPlayer::VideoPlayer()
{
	endVideoCallback = nullptr;
	Init("");
}

VideoPlayer::VideoPlayer(const char* path, const bool bAutoPlay)
{
	endVideoCallback = nullptr;
	Init(path, bAutoPlay);
}

void VideoPlayer::SetEndVideoCallback(EndVideoCallback registerEndVideoCallback)
{
	endVideoCallback = registerEndVideoCallback;
}

void VideoPlayer::PlayVideo(const char* videoPath)
{
	if (!inst) // Inicializar libVLC
	{
		inst = libvlc_new(0, NULL);
	}

	media = libvlc_media_new_path(inst, videoPath); // Cria um novo media
	if (!media)
	{
		std::cerr << "Não foi possível carregar o arquivo de mídia: " << videoPath << '\n';
		return;
	}

	libvlc_media_player_set_media(mediaPlayer, media); // Define a mídia para o player
	libvlc_media_release(media); // Libera a mídia, pois o player já possui uma referência a ela
	Play(); // Inicia a reprodução do vídeo
}

void VideoPlayer::DrawVideoFrame(const Rectangle videoSize)
{
	if (IsVideoFinished()) // Verifica se está finalizado
	{
		LOG(LOGINFO, "Video is finished");
		endVideoCallback();
		libvlc_media_player_stop(mediaPlayer);
	}
	else if (IsVideoPlaying())
	{

		//width = libvlc_video_get_width(mediaPlayer); // Obtém a largura e altura do vídeo
		//height = libvlc_video_get_height(mediaPlayer);
		libvlc_video_get_size(mediaPlayer, 0, &width, &height);
		proportion.SetProportion(width, height);

		Image resizedFrame = ImageCopy(image);
		ImageResize(&resizedFrame, static_cast<int>(videoSize.width), static_cast<int>(videoSize.height));
		//ImageBlurGaussian(&resizedFrame, 1);
		texture = LoadTextureFromImage(resizedFrame);
		//GenTextureMipmaps(&texture);
		//SetTextureFilter(texture, TEXTURE_FILTER_POINT);
		UnloadImage(resizedFrame);
		DrawTexture(texture, static_cast<int>(videoSize.x), static_cast<int>(videoSize.y), WHITE);

	}
	else if(IsVideoStopped())
	{
		DrawTextureRec(textureDefault, videoSize, Vector2{ videoSize.x, videoSize.y }, WHITE);
	}
}

void VideoPlayer::UnloadVideoFrame()
{
	if (IsVideoPlaying())
	{
		UnloadTexture(texture);
	}
}

void VideoPlayer::Play()
{
	libvlc_media_player_play(mediaPlayer); // Reproduzir o vídeo
}

void VideoPlayer::Pause()
{
	libvlc_media_player_pause(mediaPlayer);
}

void VideoPlayer::Stop()
{
	libvlc_media_player_stop(mediaPlayer);
}

void VideoPlayer::CloseVideo()
{
	// Parar e liberar o media player
	libvlc_media_player_stop(mediaPlayer);
	libvlc_media_player_release(mediaPlayer);
	libvlc_release(inst);

	// Libera a textura
	UnloadTexture(texture);
	UnloadTexture(textureDefault);
}

void VideoPlayer::SetVolume(const int volume)
{
	// ajuste o volume conforme necessário
	libvlc_audio_set_volume(mediaPlayer, volume);
}

Proportion VideoPlayer::GetProportion()
{
	return proportion;
}

Vector2 VideoPlayer::GetVideoSize()
{
	return Vector2{ static_cast<float>(width),static_cast<float>(height) };
}

bool VideoPlayer::IsVideoFinished()
{
	const libvlc_state_t state = libvlc_media_player_get_state(mediaPlayer);
	return state == libvlc_Ended || state == libvlc_Error;
}

bool VideoPlayer::IsVideoPlaying()
{
	const libvlc_state_t state = libvlc_media_player_get_state(mediaPlayer);
	return state == libvlc_Playing;
}

bool VideoPlayer::IsVideoStopped()
{
	const libvlc_state_t state = libvlc_media_player_get_state(mediaPlayer);
	return state == libvlc_Stopped;
}