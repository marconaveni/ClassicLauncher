#include "RenderTexture.h"

namespace ray
{
#include "rlgl.h"
} // namespace ray

#include "Window/RayWindow.h"

typedef enum
{
    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R16,           // 16 bpp (1 channel - half float)
    PIXELFORMAT_UNCOMPRESSED_R16G16B16,     // 16*3 bpp (3 channels - half float)
    PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,  // 16*4 bpp (4 channels - half float)
    PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} PixelFormat;

namespace ClassicLauncher
{
    RenderTexture::RenderTexture(int width, int height)
    {
        Load(width, height);
    }

    RenderTexture::~RenderTexture()
    {
        Unload();
    }

    bool RenderTexture::Load(int width, int height)
    {
        m_id = ray::rlLoadFramebuffer(); // Load an empty framebuffer

        if (m_id > 0)
        {
            ray::rlEnableFramebuffer(m_id);

            // Create color texture (default to RGBA)
            m_texture.m_id = ray::rlLoadTexture(NULL, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
            m_texture.m_width = width;
            m_texture.m_height = height;
            m_texture.m_format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
            m_texture.m_mipmaps = 1;

            // Create depth renderbuffer/texture
            m_depth.m_id = ray::rlLoadTextureDepth(width, height, true);
            m_depth.m_width = width;
            m_depth.m_height = height;
            m_depth.m_format = 19; // DEPTH_COMPONENT_24BIT?
            m_depth.m_mipmaps = 1;

            // Attach color texture and depth renderbuffer/texture to FBO
            ray::rlFramebufferAttach(m_id,
                                     m_texture.m_id,
                                     ray::RL_ATTACHMENT_COLOR_CHANNEL0,
                                     ray::RL_ATTACHMENT_TEXTURE2D,
                                     0);
            ray::rlFramebufferAttach(m_id, m_depth.m_id, ray::RL_ATTACHMENT_DEPTH, ray::RL_ATTACHMENT_RENDERBUFFER, 0);

            // Check if fbo is complete with attachments (valid)
            // if (rlFramebufferComplete(m_target.m_id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully",
            // m_target.m_id);

            ray::rlDisableFramebuffer();
        }

        return ray::rlFramebufferComplete(m_id);
    }

    void RenderTexture::SetSmooth(bool status)
    {
        m_texture.SetSmooth(status);
        m_depth.SetSmooth(status);
    }

    bool RenderTexture::IsValid() const
    {
        return ((m_id > 0) &&        // Validate OpenGL id (loaded on GPU)
                m_depth.IsValid() && // Validate FBO depth texture/renderbuffer attachment
                m_texture.IsValid());
    }

    void RenderTexture::Unload()
    {


        if (m_id > 0)
        {
            if (m_texture.m_id > 0)
            {
                // Color texture attached to FBO is deleted
                //m_texture.Unload();
            }
            // NOTE: Depth texture/renderbuffer is automatically
            // queried and deleted before deleting framebuffer
            ray::rlUnloadFramebuffer(m_id);
            m_id = 0;
        }
    }

} // namespace ClassicLauncher