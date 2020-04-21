#include <gui/widgets/BARCodeWidget.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <string>

BARCodeWidget::BARCodeWidget(int16_t height) :
    codeHeight(height),
    scale(1),
    borderWidth(15)
{
}

void BARCodeWidget::draw(const touchgfx::Rect& invalidatedArea) const
{
    if (!code)
    {
        return;
    }

// #ifdef SIMULATOR
    uint16_t displayWidth = touchgfx::HAL::DISPLAY_WIDTH;
    uint8_t bitDepth = touchgfx::HAL::lcd().bitDepth();
// #else
    // 实际的 FrameBuffer 与硬件配置相关
    // uint16_t displayWidth = GUI_FRAME_BUFFER_WIDTH;
    // uint8_t bitDepth = GUI_DISPLAY_BPP;
// #endif

    uint8_t byteDepth = bitDepth >> 3; // 暂不支持 bitDepth 为 1/2/4 的情形
    touchgfx::Rect absolute = getAbsoluteRect();
    int16_t w = getWidth();
    int16_t h = getHeight();

    int16_t barsize = code->getSize();
    int16_t borderSize = borderWidth * byteDepth;
    int16_t barBlockSize = scale * byteDepth;
    int16_t bufSize = w * byteDepth;
    uint16_t copyLen = invalidatedArea.width * byteDepth;
    uint8_t* buffer = new uint8_t[bufSize];

    uint8_t* framebuffer = (uint8_t*)touchgfx::HAL::getInstance()->lockFrameBuffer();

    for (int16_t y = invalidatedArea.y; y < invalidatedArea.bottom(); y++)
    {
        // buffer 代表每一行 N 个点的色值（3 byte），3 种情况需要重新计算 buffer 值的情况：第一行、条形码的上下边缘
        if (y == invalidatedArea.y || y == borderWidth || y == h - borderWidth)
        {
            if (borderWidth <= y && y < h - borderWidth)
            {
                uint8_t* addr = buffer;
                memset(addr, 0xff, borderSize); // 右 border
                addr += borderSize;

                // bar block
                for (int16_t i = 0; i < barsize; i++)
                {
                    uint8_t value = code->getModule(i) ? 0xff : 0x00;
                    memset(addr, value, barBlockSize);
                    addr += barBlockSize;
                }

                memset(addr, 0xff, borderSize); // 右 border
            }
            else
            {
                memset(buffer, 0xff, bufSize); // 上下 border
            }
        }

        uint8_t* frameAddr = &framebuffer[(absolute.x + invalidatedArea.x + (absolute.y + y) * displayWidth) * byteDepth];
        memcpy(frameAddr, buffer + invalidatedArea.x, copyLen);
    }

    touchgfx::HAL::getInstance()->unlockFrameBuffer();
}

touchgfx::Rect BARCodeWidget::getSolidRect() const
{
    return touchgfx::Rect(0, 0, getWidth(), getHeight());
}

void BARCodeWidget::setBARCode(BARCode* barcode)
{
    code = barcode;
    updateSize();
}

void BARCodeWidget::setBARCodeHeight(int16_t h)
{
    codeHeight = h;
    updateSize();
}

void BARCodeWidget::setScale(uint8_t s)
{
    scale = s;
    updateSize();
}

void BARCodeWidget::setBorderWidth(uint8_t width)
{
    borderWidth = width;
    updateSize();
}

void BARCodeWidget::updateSize()
{
    if (code)
    {
        setWidth(code->getSize() * scale + 2 * borderWidth);
        setHeight(codeHeight + 2 * borderWidth);
    }
}
