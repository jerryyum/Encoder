/**
  ******************************************************************************
  * This file is created for screen switch up&down
  *
  * CopyRight(C) 2019 YiWang <yiwang@baos.cn>
  * 
  ******************************************************************************
  */

#ifndef DUALSWIPECONTAINER_HPP
#define DUALSWIPECONTAINER_HPP

#include <touchgfx/Callback.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/widgets/TiledImage.hpp>

namespace touchgfx
{
/**
 * @class SwipeContainer SwipeContainer.hpp touchgfx/containers/SwipeContainer.hpp
 *
 * @brief A Dual Direction swipe container.
 *
 * @see touchgfx::Container.
 */
class DualSwipeContainer : public touchgfx::Container
{
public:
    enum SwipeDirection
    {
        HORIZONTAL,
        VERTICAL
    };

    DualSwipeContainer();

    DualSwipeContainer(SwipeDirection swipeDirection);

    virtual ~DualSwipeContainer();

    virtual void handleTickEvent();
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);
    virtual void handleDragEvent(const touchgfx::DragEvent& evt);
    virtual void handleGestureEvent(const touchgfx::GestureEvent& evt);

    /**
     * @fn virtual void SwipeContainer::add(touchgfx::Drawable& page);
     *
     * @brief Adds a page to the container.
     *
     *        Adds a page to the container.
     *
     * @param [in,out] page The page to add.
     *
     * @note All pages must have the same width and height.
     */
    virtual void add(touchgfx::Drawable& page);

    /**
     * @fn virtual void SwipeContainer::remove(Drawable& page);
     *
     * @brief Removes the page from the container.
     *
     *        Removes the page from the container.
     *
     * @param [in,out] page The page to remove.
     *
     * @note This is safe to call even if page is not a page (in which case nothing happens).
     */
    virtual void remove(Drawable& page);

    /**
     * @fn virtual void SwipeContainer::setSwipeCutoff(uint16_t cutoff);
     *
     * @brief Set the swipe cutoff.
     *
     *        Set the swipe cutoff which indicates how far you should drag a page before it
     *        results in a page change.
     *
     * @param cutoff The cutoff in pixels.
     */
    virtual void setSwipeCutoff(uint16_t cutoff);

    /**
     * @fn void SwipeContainer::setPageIndicatorXY(int16_t x, int16_t y);
     *
     * @brief Sets the x and y position of the page indicator.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setPageIndicatorXY(int16_t x, int16_t y);

    /**
     * @fn void SwipeContainer::setPageIndicatorXYWithCenteredCoordinate(int16_t x, int16_t y);
     *
     * @brief Sets the x and y position of the page indicator.
     *
     *        Sets the x and y position of the page indicator. The value specified as x will be
     *        the center coordinate of the page indicators.
     *
     * @param x The center x coordinate.
     * @param y The y coordinate.
     *
     * @note This method should not be used until all pages have been added, the
     *       setPageIndicatorBitmaps has been called and the page indicator therefore has the
     *       correct Coordinate.
     */
    void setPageIndicatorXYWithCenteredCoordinate(int16_t x, int16_t y);

    /**
     * @fn void SwipeContainer::setPageIndicatorBitmaps(const touchgfx::Bitmap& normalPage, const touchgfx::Bitmap& highlightedPage);
     *
     * @brief Sets the bitmaps that are used by the page indicator.
     *
     *        Sets the bitmaps that are used by the page indicator.
     *
     * @param normalPage      The normal page.
     * @param highlightedPage The highlighted page.
     */
    void setPageIndicatorBitmaps(const touchgfx::Bitmap& normalPage, const touchgfx::Bitmap& highlightedPage);

    /**
     * @fn void SwipeContainer::setEndSwipeElasticCoordinate(uint16_t coordinate);
     *
     * @brief When dragging either one of the end pages a part of the background will become
     *        visible until the user stop dragging and the end page swipes back to its
     *        position. The coordinate of this area is set by this method.
     *
     * @param coordinate The coordinate in pixels.
     */
    void setEndSwipeElasticDistance(uint16_t distance);

    /**
     * @fn uint8_t SwipeContainer::getNumberOfPages()
     *
     * @brief Gets number of pages.
     *
     *        Gets number of pages.
     *
     * @return The number of pages.
     */
    uint8_t getNumberOfPages()
    {
        return numberOfPages;
    }

    SwipeDirection getSwipeDirection() const
    {
        return pageIndicator.swipeDirection;
    }

    /**
     * @fn void SwipeContainer::setSelectedPage(uint8_t pageIndex);
     *
     * @brief Sets the selected page.
     *
     *        Sets the selected page.
     *
     * @param pageIndex Zero-based index of the page. Range from 0 to numberOfPages-1.
     */
    void setSelectedPage(uint8_t pageIndex);

    /**
     * @fn void SwipeContainer::getSelectedPage(uint8_t pageIndex);
     *
     * @brief Get the selected page.
     *
     *        Get the selected page.
     *
     * @return  Zero-based index of the page. Range from 0 to numberOfPages-1.
     */
    uint8_t getSelectedPage() { return currentPage; }

    void setSwipeDirection(SwipeDirection swipeDirection);

    void setPageAdjustCallback(GenericCallback<const DualSwipeContainer&, const uint8_t, const uint8_t>& callback);

protected:
    enum States
    {
        ANIMATE_SWIPE_CANCELLED_PREVIOUS,
        ANIMATE_SWIPE_CANCELLED_LATTER,
        ANIMATE_PREVIOUS, ///< LEFT or UP
        ANIMATE_LATTER,   ///< RIGHT or DOWN
        NO_ANIMATION
    } currentState;

private:
    static const int16_t DRAG_CANCEL_THRESHOLD = 3; //Pixels to drag before sending cancel event.

    uint8_t numberOfPages;

    uint8_t animationCounter;

    uint16_t swipeCutoff;

    int16_t dragDistance;
    int16_t animateDistance;
    int16_t startDistance;
    uint8_t currentPage;

    // Background that becomes visible when trying to swipe
    // beyond the number of pages.
    uint16_t endElasticDistance;

    touchgfx::ListLayout pages;

    touchgfx::GenericCallback<const DualSwipeContainer&, const uint8_t, const uint8_t>* pageAdjustCallback;

    void adjustPages();

    void animateSwipeCancelledPrevious();
    void animateSwipeCancelledLatter();
    void animatePrevious();
    void animateLatter();

    /**
     * @class PageIndicator PageIndicator.hpp touchgfx/widgets/PageIndicator.hpp
     *
     * @brief A page indicator.
     *
     * @see PageIndicator
     */
    class PageIndicator : public Container
    {
    public:
        PageIndicator();
        virtual ~PageIndicator();

        SwipeDirection swipeDirection;

        /**
         * @fn void PageIndicator::setNumberOfPages(uint8_t size);
         *
         * @brief Sets the number of dots that should be displayed.
         *
         *        Sets the number of dots that should be displayed.
         *
         * @param size The size.
         */
        void setNumberOfPages(uint8_t size);

        /**
         * @fn void PageIndicator::setBitmaps(const Bitmap& normalPage, const Bitmap& highlightedPage);
         *
         * @brief Sets the bitmaps to be used for normal and highlighted dots.
         *
         *        Sets the bitmaps to be used for normal and highlighted dots. Note that the
         *        bitmaps should include the spacing between the dots and that they must have the
         *        same width and height.
         *
         * @param normalPage      The normal dot bitmap.
         * @param highlightedPage The highlighted dot bitmap.
         */
        void setBitmaps(const Bitmap& normalPage, const Bitmap& highlightedPage);

        /**
         * @fn void PageIndicator::goRight(); and goDown();
         *
         * @brief Highlight the dot to the right of the current highlighted dot.
         *
         *        Highlight the dot to the right of the current highlighted dot.
         */
        void goLatter();

        /**
         * @fn void PageIndicator::goLeft(); and goUP();
         *
         * @brief Highlight the dot to the left of the current highlighted dot.
         *
         *        Highlight the dot to the left of the current highlighted dot.
         */

        void goPrevious();

        /**
         * @fn void PageIndicator::setHighlightPosition(uint8_t index);
         *
         * @brief Sets highlight position.
         *
         *        Sets highlight position.
         *
         * @param index Zero-based index of the dot to be highlighted.
         */
        void setHighlightPosition(uint8_t index);

    private:
        TiledImage unselectedPages;
        Image selectedPage;
        uint8_t numberOfPages;
        uint8_t currentPage;
    } pageIndicator;
};
}

#endif // DUALSWIPECONTAINER_HPP
