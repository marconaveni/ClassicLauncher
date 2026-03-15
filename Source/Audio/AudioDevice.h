#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

namespace ClassicLauncher
{

    class AudioDevice
    {
    public:

        AudioDevice(const AudioDevice&) = delete;
        AudioDevice& operator=(const AudioDevice&) = delete;

        void Init();
        void Shutdown();
        bool IsReady();
        void SetMasterVolume(float volume);

        static AudioDevice& GetInstance();

    private:

        AudioDevice() = default;
        ~AudioDevice() = default;

        bool m_isInitialized{false};
    };
} // namespace ClassicLauncher

#endif // AUDIO_DEVICE_H