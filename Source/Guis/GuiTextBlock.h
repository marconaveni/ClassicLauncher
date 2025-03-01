#ifndef GUI_TEXT_BLOCK_H
#define GUI_TEXT_BLOCK_H

#include <string>
#include "Entity/EntityGui.h"

namespace ClassicLauncher
{

    enum TextOverflowPolicy
    {
        none = 0x0,
        clip = 0x1
    };

    class EntityGui;

    class GuiTextBlock : public EntityGui
    {
    private:

        Font mFont;
        std::string mText;
        std::string mPathFont;
        int mSize;
        int mSpacing;
        Color mColor;
        bool mTextMovement;
        int mDesiredWidth;
        float mOffset;
        bool mToLeft;
        float mDelay;
        Vector2 mMensuredText;
        float mSpeed;
        float mMaxDelay;
        TextOverflowPolicy mTextOverflowPolicy;

        Vector2 MeasureTextBox();

    public:

        GuiTextBlock(const std::string& path, int size = 16, int spacing = 0);
        virtual ~GuiTextBlock() override;
        virtual EntityType GetType() const override { return EntityType::GuiTextBlockClass; }
        void LoadNewFont(const std::string& path, int size = 16, int spacing = 0);
        void UpdateFont(const std::string& path);
        virtual void Update() override;
        virtual void Draw() override;
        virtual void End() override;
        void SetText(const std::string& text);
        void SetSize(int size);
        void SetSpacing(int spacing);
        void SetColor(Color tint);
        Vector2 GetMeasureTextBox();
        const std::string& GetText() { return mText; }
        void SetTextOverflowPolicy(TextOverflowPolicy textOverflowPolicy);
        void SetDesiredWidth(int newWidth);
        void SetOffSetMoveText(float speed, float maxDelay);
        void UnloadText();
    };

}  // namespace ClassicLauncher

#endif