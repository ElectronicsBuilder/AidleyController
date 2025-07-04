/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef PRESPLASHSCREENVIEWBASE_HPP
#define PRESPLASHSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/presplashscreen_screen/preSplashScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/Image.hpp>

class preSplashScreenViewBase : public touchgfx::View<preSplashScreenPresenter>
{
public:
    preSplashScreenViewBase();
    virtual ~preSplashScreenViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::BoxWithBorder boxWithBorderMain;
    touchgfx::Image droneImage;

private:

};

#endif // PRESPLASHSCREENVIEWBASE_HPP
