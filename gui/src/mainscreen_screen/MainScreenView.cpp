#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <gui/common/Constants.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

MainScreenView::MainScreenView() :
    qrCode(QRCode::encodeText("Hello QRCode!")),
    barCode(BARCode::encodeText("12345678901234567890"))
{
    swipeContainer.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    swipeContainer.setSwipeDirection(DualSwipeContainer::VERTICAL);
    swipeContainer.setSwipeCutoff(50);
    swipeContainer.setEndSwipeElasticDistance(50);
    swipeContainer.setPageIndicatorBitmaps(touchgfx::Bitmap(BITMAP_SWIPE_PAGEINDICATOR_NORMAL_ID),
                                           touchgfx::Bitmap(BITMAP_SWIPE_PAGEINDICATOR_HIGHLIGHT_ID));
    swipeContainer.setPageIndicatorXYWithCenteredCoordinate(HAL::DISPLAY_WIDTH - 25,
                                                            divBY2(HAL::DISPLAY_HEIGHT));
    add(swipeContainer);

    qrcodeContainer.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    barcodeContainer.setPosition(0, 0, HAL::DISPLAY_WIDTH, HAL::DISPLAY_HEIGHT);
    swipeContainer.add(qrcodeContainer);
    swipeContainer.add(barcodeContainer);

    // qrcodeContainer sub widgets
    qrcodeTextArea.setPosition(GENERIC_PADDING,
                               TITLE_POS_Y,
                               HAL::DISPLAY_WIDTH - mulBY2(GENERIC_PADDING),
                               TITLE_HEIGHT);
    qrcodeTextArea.setColor(C_WHITE);
    qrcodeTextArea.setLinespacing(0);
    qrcodeTextArea.setTypedText(touchgfx::TypedText(T_PAY_QRCODE));

    qrCodeWidget.setQRCode(&qrCode);
    qrCodeWidget.setScale(QRCODE_WIDTH / qrCode.getSize());
    qrCodeWidget.setXY(ALIGN_CENTER_HORIZONTAL_ON_SCREEN(qrCodeWidget), ALIGN_CENTER_VERTICAL_ON_SCREEN(qrCodeWidget));

    qrcodeContainer.add(qrcodeTextArea);
    qrcodeContainer.add(qrCodeWidget);

    // barcodeContainer sub widgets
    barcodeTextArea.setPosition(GENERIC_PADDING,
                                TITLE_POS_Y,
                                HAL::DISPLAY_WIDTH - mulBY2(GENERIC_PADDING),
                                TITLE_HEIGHT);
    barcodeTextArea.setColor(C_WHITE);
    barcodeTextArea.setLinespacing(0);
    barcodeTextArea.setTypedText(touchgfx::TypedText(T_PAY_BARCODE));

    barCodeWidget.setBARCode(&barCode);
    barCodeWidget.setBARCodeHeight(BARCODE_HEIGHT);
    barCodeWidget.setScale(BARCODE_WIDTH / barCode.getSize());
    barCodeWidget.setXY(ALIGN_CENTER_HORIZONTAL_ON_SCREEN(barCodeWidget), ALIGN_CENTER_VERTICAL_ON_SCREEN(barCodeWidget));

    barcodeContainer.add(barcodeTextArea);
    barcodeContainer.add(barCodeWidget);
}

void MainScreenView::setupScreen()
{
    MainScreenViewBase::setupScreen();
}

void MainScreenView::tearDownScreen()
{
    MainScreenViewBase::tearDownScreen();
}
