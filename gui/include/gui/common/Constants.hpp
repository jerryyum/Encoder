#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// 将 char * 转换成 const uint8_t *
#define toUINT8(p) reinterpret_cast<const uint8_t*>(p)

// 将 char * 转换成 uint8_t *
#define toUINT8V(p) reinterpret_cast<uint8_t*>(p)

// 将 char * 转换成 const char *
#define toINT8(p) reinterpret_cast<const char*>(p)

// 除以 2
#define divBY2(n) ((n) >> 1)

// 乘以 2
#define mulBY2(n) ((n) << 1)

// 便于居中对齐的宏定义
#define ALIGN_CENTER_VERTICAL_ON_SCREEN(w) (divBY2(HAL::DISPLAY_HEIGHT - (w).getHeight()))

#define ALIGN_CENTER_HORIZONTAL_ON_SCREEN(w) (divBY2(HAL::DISPLAY_WIDTH - (w).getWidth()))

#define ALIGN_CENTER_VERTICAL_IN_CONTAINER(w) (divBY2(getHeight() - (w).getHeight()))

#define ALIGN_CENTER_HORIZONTAL_IN_CONTAINER(w) (divBY2(getWidth() - (w).getWidth()))

#define GENERIC_PADDING 15

// title 位置
#define TITLE_POS_Y 30

// title 高度
#define TITLE_HEIGHT 30

// 二维码宽度
#define QRCODE_WIDTH 200

// 条形码宽度
#define BARCODE_WIDTH 300

// 条形码高度
#define BARCODE_HEIGHT 120

// 默认字体颜色 FFFFFF
#define C_WHITE (colortype(0xFFFFFF))

#endif // CONSTANTS_HPP