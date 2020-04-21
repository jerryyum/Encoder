#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/MainScreenViewBase.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>
#include <gui/containers/DualSwipeContainer.hpp>
#include <gui/widgets/QRCodeWidget.hpp>
#include <gui/widgets/BARCodeWidget.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextArea.hpp>

class MainScreenView : public MainScreenViewBase
{
public:
    MainScreenView();
    virtual ~MainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

protected:
    DualSwipeContainer swipeContainer;

    Container qrcodeContainer;
    TextArea qrcodeTextArea;
    QRCodeWidget qrCodeWidget;
    QRCode qrCode;

    Container barcodeContainer;
    TextArea barcodeTextArea;
    BARCodeWidget barCodeWidget;
    BARCode barCode;
};

#endif // MAINSCREENVIEW_HPP
