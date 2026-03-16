#include "RenderTexture.h"

namespace ray
{
#include "rlgl.h"
} // namespace ray

#include "Window/Window.h"

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
            m_texture.m_id = ray::rlLoadTexture(NULL, width, height, ray::RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
            m_texture.m_width = width;
            m_texture.m_height = height;
            m_texture.m_format = ray::RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
            m_texture.m_mipmaps = 1;

            // Create depth renderbuffer/texture
            m_depth.m_id = ray::rlLoadTextureDepth(width, height, true);
            m_depth.m_width = width;
            m_depth.m_height = height;
            m_depth.m_format = 19; // DEPTH_COMPONENT_24BIT?
            m_depth.m_mipmaps = 1;

            // Attach color texture and depth renderbuffer/texture to FBO
            ray::rlFramebufferAttach(m_id, m_texture.m_id, ray::RL_ATTACHMENT_COLOR_CHANNEL0, ray::RL_ATTACHMENT_TEXTURE2D, 0);
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
                m_texture.Unload();
                m_depth.Unload();
            }
            // NOTE: Depth texture/renderbuffer is automatically
            // queried and deleted before deleting framebuffer
            ray::rlUnloadFramebuffer(m_id);
            m_id = 0;
        }
    }

} // namespace ClassicLauncher