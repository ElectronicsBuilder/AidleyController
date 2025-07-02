#ifndef PRESPLASHSCREENVIEW_HPP
#define PRESPLASHSCREENVIEW_HPP

#include <gui_generated/presplashscreen_screen/preSplashScreenViewBase.hpp>
#include <gui/presplashscreen_screen/preSplashScreenPresenter.hpp>

class preSplashScreenView : public preSplashScreenViewBase
{
public:
    preSplashScreenView();
    virtual ~preSplashScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // PRESPLASHSCREENVIEW_HPP
