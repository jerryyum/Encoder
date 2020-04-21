#include <gui/widgets/QRCodeWidget.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <string>

QRCodeWidget::QRCodeWidget() :
    code(0),
    scale(1),
    borderWidth(15)
{
}

void QRCodeWidget::setQRCode(QRCode* qrCode)
{
    code = qrCode;
    updateSize();
}

void QRCodeWidget::draw(const touchgfx::Rect& invalidatedArea) const
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

    uint8_t* framebuffer = (uint8_t*)touchgfx::HAL::getInstance()->lockFrameBuffer();

    /*
    for (int16_t y = invalidatedArea.y; y < invalidatedArea.bottom(); y++)
    {
        for (int16_t x = invalidatedArea.x; x < invalidatedArea.right(); x++)
        {
            void* addr = &framebuffer[(absolute.x + x + (absolute.y + y) * displayWidth) * byteDepth];
            if (borderWidth <= x && x < w - borderWidth && borderWidth <= y && y < h - borderWidth)
            {
                uint8_t value = code->getModule((x - borderWidth) / scale, (y - borderWidth) / scale) ? 0x00 : 0xff;
                memset(addr, value, byteDepth);
            }
            else
            {
                memset(addr, 0xff, byteDepth);
            }
        }
    }
    */

    // 上面是没有优化的代码，在真机上基本无法运行（但易于理解），下面是经过优化的代码
    int16_t qrsize = code->getSize();
    int16_t borderSize = borderWidth * byteDepth;
    int16_t qrBlockSize = scale * byteDepth;
    int16_t bufSize = w * byteDepth;
    uint16_t copyLen = invalidatedArea.width * byteDepth;
    uint8_t* buffer = new uint8_t[bufSize];

    for (int16_t y = invalidatedArea.y; y < invalidatedArea.bottom(); y++)
    {
        if (y == invalidatedArea.y || y == borderWidth || (y - borderWidth) % scale == 0)
        {
            uint8_t* addr = buffer;
            if (borderWidth <= y && y < h - borderWidth)
            {
                memset(addr, 0xff, borderSize); // 右 border
                addr += borderSize;

                // qr block
                int16_t j = (y - borderWidth) / scale;
                for (int16_t i = 0; i < qrsize; i++)
                {
                    uint8_t value = code->getModule(i, j) ? 0x00 : 0xff;
                    memset(addr, value, qrBlockSize);
                    addr += qrBlockSize;
                }

                memset(addr, 0xff, borderSize); // 右 border
            }
            else
            {
                memset(addr, 0xff, bufSize); // 上下 border
            }
        }

        uint8_t* frameAddr = &framebuffer[(absolute.x + invalidatedArea.x + (absolute.y + y) * displayWidth) * byteDepth];
        memcpy(frameAddr, buffer + invalidatedArea.x, copyLen);
    }

    delete[] buffer;

    touchgfx::HAL::getInstance()->unlockFrameBuffer();
}

touchgfx::Rect QRCodeWidget::getSolidRect() const
{
    return touchgfx::Rect(0, 0, getWidth(), getHeight());
}

void QRCodeWidget::setScale(uint8_t s)
{
    scale = s;
    updateSize();
}

void QRCodeWidget::setBorderWidth(uint8_t width)
{
    borderWidth = width;
    updateSize();
}

void QRCodeWidget::updateSize()
{
    if (code)
    {
        setWidth(code->getSize() * scale + 2 * borderWidth);
        setHeight(code->getSize() * scale + 2 * borderWidth);
    }
}
