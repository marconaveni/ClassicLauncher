#include "Audio/AudioDevice.h"

#include "AudioDevice.h"
#include "raylib.h"

namespace ClassicLauncher
{


    void AudioDevice::Init()
    {
        m_isInitialized = ::IsAudioDeviceReady();
        if (!m_isInitialized)
        {
            ::InitAudioDevice();
            m_isInitialized = ::IsAudioDeviceReady();
        }
    }

    void AudioDevice::Shutdown()
    {
        if (m_isInitialized)
        {
            ::CloseAudioDevice();
            m_isInitialized = ::IsAudioDeviceReady();
        }
    }

    bool AudioDevice::IsReady()
    {
        return m_isInitialized;
    }

    void AudioDevice::SetMasterVolume(float volume)
    {
        ::SetMasterVolume(volume);
    }

    AudioDevice& AudioDevice::GetInstance()
    {
        static AudioDevice instance;
        return instance;
    }

} // namespace ClassicLauncher
