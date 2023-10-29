#pragma once
#include "Components.h"
#include "ComponentBase.h"
#include "App.h"
class LoginPage :
    public View::PageBase
{
public:
    LoginPage(){}

    void initComponents(); 

    int confirmLogin(View::MouseEvent& e, View::ComponentBase* com);    // È·ÈÏµÇÈë

    int confirmSignUp(View::MouseEvent& e, View::ComponentBase* com);   // È·ÈÏ×¢²á

    int toggleLogin(View::MouseEvent& e, View::ComponentBase* com); // ÇÐ»»µÇÂ¼Ò³Ãæ

    int toggleSignUp(View::MouseEvent& e, View::ComponentBase* com);    // ÇÐ»»×¢²áÒ³Ãæ

private:
    View::Text* boxTitle;
    View::Text* errorInfo;
    View::InputText* acInput;
    View::InputText* pwInput;
    View::Button* leftButton;
    View::Button* rightButton;
};

