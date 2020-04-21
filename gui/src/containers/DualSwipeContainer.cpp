/**
  ******************************************************************************
  * This file is created for screen switch up&down
  *
  * CopyRight(C) 2019 YiWang <yiwang@baos.cn>
  * 
  ******************************************************************************
  */

#include <gui/containers/DualSwipeContainer.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
DualSwipeContainer::DualSwipeContainer() :
    Container(),
    currentState(NO_ANIMATION),
    numberOfPages(0),
    animationCounter(0),
    swipeCutoff(80),
    dragDistance(0),
    animateDistance(0),
    startDistance(0),
    currentPage(0),
    endElasticDistance(30),
    pages(SOUTH),
    pageAdjustCallback(0),
    pageIndicator()
{
    touchgfx::Application::getInstance()->registerTimerWidget(this);

    setTouchable(true);

    Container::add(pages);
    Container::add(pageIndicator);
}

DualSwipeContainer::DualSwipeContainer(SwipeDirection swipeDirection) :
    Container(),
    currentState(NO_ANIMATION),
    numberOfPages(0),
    animationCounter(0),
    swipeCutoff(80),
    dragDistance(0),
    animateDistance(0),
    startDistance(0),
    currentPage(0),
    endElasticDistance(30),
    pageAdjustCallback(0),
    pageIndicator()
{
    pages = (swipeDirection == HORIZONTAL) ? EAST : SOUTH;
    pageIndicator.swipeDirection = swipeDirection;

    touchgfx::Application::getInstance()->registerTimerWidget(this);

    setTouchable(true);

    Container::add(pages);
    Container::add(pageIndicator);
}

DualSwipeContainer::~DualSwipeContainer()
{
    touchgfx::Application::getInstance()->unregisterTimerWidget(this);
}

void DualSwipeContainer::setSwipeDirection(SwipeDirection swipeDirection)
{
    pageIndicator.swipeDirection = swipeDirection;

    if (pageIndicator.swipeDirection == HORIZONTAL)
    {
        // pages = EAST;
        pages.setDirection(EAST);
    }
    else ///< VERTICAL
    {
        // pages = SOUTH;
        pages.setDirection(SOUTH);
    }
}

void DualSwipeContainer::setPageAdjustCallback(GenericCallback<const DualSwipeContainer&, const uint8_t, const uint8_t>& callback)
{
    pageAdjustCallback = &callback;
}

void DualSwipeContainer::add(Drawable& page)
{
    pages.add(page);
    numberOfPages++;

    pageIndicator.setNumberOfPages(numberOfPages);

    setWidth(page.getWidth());
    setHeight(page.getHeight());
}

void DualSwipeContainer::remove(Drawable& page)
{
    Drawable* tmp = pages.getFirstChild();

    if (!numberOfPages)
    {
        return;
    }

    // looks for the child matching page
    // to ensure that the page indicator only countsdown if a page is removed
    while (tmp)
    {
        if (tmp == &page)
        {
            pages.remove(page);
            numberOfPages--;

            if (!numberOfPages)
            {
                setWidth(0);
                setHeight(0);
            }
            else
            {
                pageIndicator.setNumberOfPages(numberOfPages);
            }
            return;
        }
        else
        {
            tmp = tmp->getNextSibling();
        }
    }
}

void DualSwipeContainer::setEndSwipeElasticDistance(uint16_t distance)
{
    endElasticDistance = distance;
}

void DualSwipeContainer::setSwipeCutoff(uint16_t cutoff)
{
    swipeCutoff = cutoff;
}

void DualSwipeContainer::setPageIndicatorBitmaps(const touchgfx::Bitmap& normalPage, const touchgfx::Bitmap& highlightedPage)
{
    pageIndicator.setBitmaps(normalPage, highlightedPage);
}

void DualSwipeContainer::setPageIndicatorXY(int16_t x, int16_t y)
{
    pageIndicator.setXY(x, y);
}

void DualSwipeContainer::setPageIndicatorXYWithCenteredCoordinate(int16_t x, int16_t y)
{
    if (pageIndicator.swipeDirection == HORIZONTAL)
    {
        pageIndicator.setXY(x - pageIndicator.getWidth() / 2, y);
    }
    else ///< VERTICAL
    {
        pageIndicator.setXY(x, y - (pageIndicator.getHeight() * numberOfPages) / 2);
    }
}

void DualSwipeContainer::setSelectedPage(uint8_t pageIndex)
{
    currentPage = pageIndex;
    pageIndicator.setHighlightPosition(currentPage);
    adjustPages();
}

void DualSwipeContainer::handleTickEvent()
{
    if (currentState == ANIMATE_SWIPE_CANCELLED_PREVIOUS)
    {
        animateSwipeCancelledPrevious();
    }
    else if (currentState == ANIMATE_SWIPE_CANCELLED_LATTER)
    {
        animateSwipeCancelledLatter();
    }
    else if (currentState == ANIMATE_PREVIOUS)
    {
        animatePrevious();
    }
    else if (currentState == ANIMATE_LATTER)
    {
        animateLatter();
    }
}

void DualSwipeContainer::handleClickEvent(const ClickEvent& evt)
{
    // If an animation is already in progress do not
    // react to clicks
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    if (evt.getType() == ClickEvent::RELEASED)
    {
        // Save current position for use during animation
        animateDistance = dragDistance;
        startDistance = (pageIndicator.swipeDirection == HORIZONTAL) ? (pages.getX()) : (pages.getY());
        if (dragDistance < 0)
        {
            if (currentPage == getNumberOfPages() - 1 || dragDistance > -swipeCutoff)
            {
                currentState = ANIMATE_SWIPE_CANCELLED_PREVIOUS;
            }
            else
            {
                currentState = ANIMATE_PREVIOUS;
            }
        }
        else if (dragDistance > 0)
        {
            if (currentPage == 0 || dragDistance < swipeCutoff)
            {
                currentState = ANIMATE_SWIPE_CANCELLED_LATTER;
            }
            else
            {
                currentState = ANIMATE_LATTER;
            }
        }
    }
}

void DualSwipeContainer::handleDragEvent(const DragEvent& evt)
{
    // If an animation is already in progress do not
    // react to drags
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    if (pageIndicator.swipeDirection == HORIZONTAL)
    {
        dragDistance += evt.getDeltaX();
    }
    else ///< VERTICAL
    {
        dragDistance += evt.getDeltaY();
    }

    // Do not show too much background next to end pages
    if (currentPage == 0 && dragDistance > endElasticDistance)
    {
        dragDistance = static_cast<int16_t>(endElasticDistance);
    }
    else if (currentPage == getNumberOfPages() - 1 && dragDistance < -endElasticDistance)
    {
        dragDistance = -static_cast<int16_t>(endElasticDistance);
    }

    adjustPages();
}

void DualSwipeContainer::handleGestureEvent(const GestureEvent& evt)
{
    // Do not accept gestures while animating
    if (currentState != NO_ANIMATION)
    {
        return;
    }

    if (((evt.getType() == evt.SWIPE_HORIZONTAL) && (pageIndicator.swipeDirection == HORIZONTAL)) ||
        ((evt.getType() == evt.SWIPE_VERTICAL) && (pageIndicator.swipeDirection == VERTICAL)))
    {
        // Save current position for use during animation
        animateDistance = dragDistance;
        startDistance = (evt.getType() == evt.SWIPE_HORIZONTAL) ? (pages.getX()) : (pages.getY());

        if (evt.getVelocity() < 0 && currentPage < getNumberOfPages() - 1)
        {
            currentState = ANIMATE_PREVIOUS;
        }
        else if (evt.getVelocity() > 0 && currentPage > 0)
        {
            currentState = ANIMATE_LATTER;
        }
    }
}

void DualSwipeContainer::adjustPages()
{
    if (pageIndicator.swipeDirection == HORIZONTAL)
    {
        pages.moveTo(-static_cast<int16_t>(currentPage * getWidth()) + dragDistance, 0);
    }
    else ///< VERTICAL
    {
        pages.moveTo(0, -static_cast<int16_t>(currentPage * getHeight()) + dragDistance);
    }
}

void DualSwipeContainer::animateSwipeCancelledPrevious()
{
    uint8_t duration = 14;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, -animateDistance, duration);
        dragDistance = animateDistance + delta;

        adjustPages();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        dragDistance = 0;
        adjustPages();
    }
    animationCounter++;
}

void DualSwipeContainer::animateSwipeCancelledLatter()
{
    uint8_t duration = 14;

    if (animationCounter <= duration)
    {
        int16_t delta = EasingEquations::backEaseOut(animationCounter, 0, animateDistance, duration);
        dragDistance = animateDistance - delta;

        adjustPages();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        dragDistance = 0;
        adjustPages();
    }
    animationCounter++;
}

void DualSwipeContainer::animatePrevious()
{
    uint8_t duration = 10;

    if (animationCounter <= duration)
    {
        int16_t delta = (pageIndicator.swipeDirection == HORIZONTAL) ? (EasingEquations::cubicEaseOut(animationCounter, 0, getWidth() + animateDistance, duration)) : (EasingEquations::cubicEaseOut(animationCounter, 0, getHeight() + animateDistance, duration));
        dragDistance = animateDistance - delta;

        adjustPages();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        currentPage++;
        dragDistance = 0;
        adjustPages();
        pageIndicator.goLatter();
        if (pageAdjustCallback && pageAdjustCallback->isValid())
        {
            pageAdjustCallback->execute(*this, currentPage - 1, currentPage);
        }
    }
    animationCounter++;
}

void DualSwipeContainer::animateLatter()
{
    uint8_t duration = 10;

    if (animationCounter <= duration)
    {
        int16_t delta = (pageIndicator.swipeDirection == HORIZONTAL) ? (EasingEquations::cubicEaseOut(animationCounter, 0, getWidth() - animateDistance, duration)) : (EasingEquations::cubicEaseOut(animationCounter, 0, getHeight() - animateDistance, duration));
        dragDistance = animateDistance + delta;

        adjustPages();
    }
    else
    {
        // Final step: stop the animation
        currentState = NO_ANIMATION;
        animationCounter = 0;
        currentPage--;
        dragDistance = 0;
        adjustPages();
        pageIndicator.goPrevious();
        if (pageAdjustCallback && pageAdjustCallback->isValid())
        {
            pageAdjustCallback->execute(*this, currentPage + 1, currentPage);
        }
    }
    animationCounter++;
}

DualSwipeContainer::PageIndicator::PageIndicator() :
    swipeDirection(VERTICAL),
    numberOfPages(0),
    currentPage(0)
{
    unselectedPages.setXY(0, 0);
    selectedPage.setXY(0, 0);

    add(unselectedPages);
    add(selectedPage);
}

DualSwipeContainer::PageIndicator::~PageIndicator()
{
}

void DualSwipeContainer::PageIndicator::setNumberOfPages(uint8_t size)
{
    numberOfPages = size;

    assert(numberOfPages > 0 && "At least one dot is needed");

    numberOfPages = size;

    if (unselectedPages.getBitmap() != BITMAP_INVALID)
    {
        if (swipeDirection == HORIZONTAL)
        {
            int dotWidth = Bitmap(unselectedPages.getBitmap()).getWidth();
            unselectedPages.setWidth(dotWidth * size);
        }
        else ///< VERTICAL
        {
            int dotHeight = Bitmap(unselectedPages.getBitmap()).getHeight();
            unselectedPages.setHeight(dotHeight * size);
        }

        // adjust size of container according to the actual bitmaps
        setWidth(unselectedPages.getWidth());
        setHeight(unselectedPages.getHeight());
        setHighlightPosition(currentPage = 0);

        invalidate();
    }
}

void DualSwipeContainer::PageIndicator::setBitmaps(const touchgfx::Bitmap& normalPage, const touchgfx::Bitmap& highlightedPage)
{
    selectedPage.setBitmap(highlightedPage);
    unselectedPages.setBitmap(normalPage);
    if (numberOfPages > 0)
    {
        setNumberOfPages(numberOfPages);
    }
}

void DualSwipeContainer::PageIndicator::goLatter()
{
    setHighlightPosition(currentPage = (currentPage + 1) % numberOfPages);
}

void DualSwipeContainer::PageIndicator::goPrevious()
{
    setHighlightPosition(currentPage = (currentPage + numberOfPages - 1) % numberOfPages);
}

void DualSwipeContainer::PageIndicator::setHighlightPosition(uint8_t index)
{
    currentPage = index;
    // note that index is unsigned
    if (index < numberOfPages)
    {
        if (swipeDirection == HORIZONTAL)
        {
            int dotWidth = Bitmap(unselectedPages.getBitmap()).getWidth();
            selectedPage.setX(index * dotWidth);
        }
        else ///< VERTICAL
        {
            int dotHeight = Bitmap(unselectedPages.getBitmap()).getHeight();
            selectedPage.setY(index * dotHeight);
        }
    }
    invalidate();
}
}
