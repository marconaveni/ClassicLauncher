#ifndef GUI_TEXT_BLOCK_H
#define GUI_TEXT_BLOCK_H

#include <string>

#include "Entity/Entity.h"

namespace ClassicLauncher
{

    
    
    class GuiTextBlock : public Entity
    {
        public:
        
        enum TextOverflowPolicy : std::uint8_t
        {
            NONE = 0,
            CLIP = 1
        };
        
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
        void SetAutoSize(bool enable);
        void SetSizeFont(int size);
        void SetSpacing(int spacing);
        Vector2f GetMeasureTextBox();
        const std::string& GetText() { return m_text; }
        void SetTextOverflowPolicy(TextOverflowPolicy textOverflowPolicy);
        void SetOffSetMoveText(float speed, float maxDelay);

        inline void SetTextCenter(bool enable) { m_centerText = enable; }

    private:

        std::string m_text{};
        std::string m_pathFont{};
        std::string m_nameFont{};
        int m_sizeText{0};
        int m_spacing{0};
        float m_offsetText{0};
        bool m_isLeft{false};
        float m_delay{0.0f};
        Vector2f m_mensuredText{};
        float m_speed{0.5f};
        float m_maxDelay{3.0f};
        TextOverflowPolicy m_textOverflowPolicy{TextOverflowPolicy::NONE};
        bool m_centerText{false};
        bool m_autoSize{false};
        float m_positionText{0.0f};
        float m_renderScale{1};

        Vector2f MeasureTextBox();
    };

} // namespace ClassicLauncher

#endif