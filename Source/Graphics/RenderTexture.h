#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "Graphics/Texture.h"

namespace ClassicLauncher
{

    class RenderTexture
    {
    public:

        RenderTexture() = default;
        RenderTexture(int width, int height);
        ~RenderTexture();
        RenderTexture(const RenderTexture&) = delete;
        RenderTexture& operator=(const RenderTexture&) = delete;

        bool Load(int width, int height);
        void SetSmooth(bool status);
        [[nodiscard]] bool IsValid() const;
        void Unload();
        [[nodiscard]] unsigned int GetId() const { return m_id; }
        [[nodiscard]] unsigned int GetTextureId() const { return m_texture.m_id; }
        [[nodiscard]] Sizef GetSize() const { return m_texture.GetSize(); }
        Texture* GetTexture() { return &m_texture; }

    private:

        unsigned int m_id{0};
        Texture m_texture{};
        Texture m_depth{};
    };

} // namespace ClassicLauncher

#endif // RENDER_TEXTURE_H