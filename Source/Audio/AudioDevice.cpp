#include "Audio/AudioDevice.h"

#include "AudioDevice.h"
#include "Ray.h"

namespace ClassicLauncher
{


    void AudioDevice::Init()
    {
        m_isInitialized = ray::IsAudioDeviceReady();
        if (!m_isInitialized)
        {
            ray::InitAudioDevice();
            m_isInitialized = ray::IsAudioDeviceReady();
        }
    }

    void AudioDevice::Shutdown()
    {
        if (m_isInitialized)
        {
            ray::CloseAudioDevice();
            m_isInitialized = ray::IsAudioDeviceReady();
        }
    }

    bool AudioDevice::IsReady()
    {
        return m_isInitialized;
    }

    void AudioDevice::SetMasterVolume(float volume)
    {
        ray::SetMasterVolume(volume);
    }

    AudioDevice& AudioDevice::GetInstance()
    {
        static AudioDevice instance;
        return instance;
    }

} // namespace ClassicLauncher
