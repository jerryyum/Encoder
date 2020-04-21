#ifndef BAR_CODE_WIDGET_HPP
#define BAR_CODE_WIDGET_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/hal/Types.hpp>
#include <gui/model/BARCode.hpp>

using namespace barcodegen;

class BARCodeWidget : public touchgfx::Widget
{
public:
    // 高度为条形码的高度，不包含 border 的高度
    BARCodeWidget(int16_t height = 156);

    virtual void draw(const touchgfx::Rect& invalidatedArea) const;

    virtual touchgfx::Rect getSolidRect() const;

    void setBARCode(BARCode* barcode);

    void setBARCodeHeight(int16_t h);

    void setScale(uint8_t s);

    void setBorderWidth(uint8_t width);

private:
    void updateSize();

    BARCode* code;
    int16_t codeHeight; // 不包含 border 的高度
    uint8_t scale;
    uint8_t borderWidth;
};

#endif // BAR_CODE_WIDGET_HPP