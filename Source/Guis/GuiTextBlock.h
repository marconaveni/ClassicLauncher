#ifndef GUI_TEXT_BLOCK_H
#define GUI_TEXT_BLOCK_H

#include <string>

#include "Entity/Entity.h"
//#include "Graphics/Font.h"
//#include "Graphics/FontManager.h"

namespace ClassicLauncher
{

    enum TextOverflowPolicy : std::uint8_t
    {
        NONE = 0,
        CLIP = 1
    };


    class GuiTextBlock : public Entity
    {
    public:

        GuiTextBlock();
        ~GuiTextBlock();
        void Init(const std::string& path, int size = 16, int spacing = 0);
        virtual EntityType GetType() const override { return EntityType::GuiTextBlockClass; }
        void LoadNewFont(const std::string& path, int size = 16, int spacing = 0);
        void UpdateFont(const std::string& path);
        virtual void Update() override;
        virtual void Draw() override;
        virtual void End() override;
        void SetText(const std::string& text);
        void SetSizeFont(int size);
        void SetSpacing(int spacing);
        Vector2f GetMeasureTextBox();
        const std::string& GetText() { return m_text; }
        void SetTextOverflowPolicy(TextOverflowPolicy textOverflowPolicy);
        void SetOffSetMoveText(float speed, float maxDelay);
        void UnloadText();

        inline void SetTextCenter(bool enable) { m_centerText = enable; }

    private:

        //Font m_font;
        std::string m_text;
        std::string m_pathFont;
        std::string m_nameFont;
        int m_sizeText;
        int m_spacing;
        float m_offsetText;
        bool m_isLeft;
        float m_delay;
        Vector2f m_mensuredText;
        float m_speed;
        float m_maxDelay;
        TextOverflowPolicy m_textOverflowPolicy;
        bool m_centerText = false;
        float m_positionText = 0.0f;
        float m_renderScale = 1;

        Vector2f MeasureTextBox();
    };

} // namespace ClassicLauncher

#endif