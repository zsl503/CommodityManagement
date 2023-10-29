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

    int confirmLogin(View::MouseEvent& e, View::ComponentBase* com);    // ȷ�ϵ���

    int confirmSignUp(View::MouseEvent& e, View::ComponentBase* com);   // ȷ��ע��

    int toggleLogin(View::MouseEvent& e, View::ComponentBase* com); // �л���¼ҳ��

    int toggleSignUp(View::MouseEvent& e, View::ComponentBase* com);    // �л�ע��ҳ��

private:
    View::Text* boxTitle;
    View::Text* errorInfo;
    View::InputText* acInput;
    View::InputText* pwInput;
    View::Button* leftButton;
    View::Button* rightButton;
};

