#ifndef RENDER_TEXTURE
#define RENDER_TEXTURE

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
        [[nodiscard]] Vector2i GetSize() const { return m_texture.GetSize(); }

    private:

        unsigned int m_id{};
        Texture m_texture{};
        Texture m_depth{};
        
    };

}  // namespace ClassicLauncher

#endif  // RENDER_TEXTURE