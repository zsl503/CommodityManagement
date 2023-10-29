#include "LoginPage.h"
#include "ShopApp.h"
using namespace Custom;

void LoginPage::initComponents()
{
	SetConsoleTitle(L"��¼");

	/*----------------New Compoments----------------------*/
	Box* loginBox = new Box(outputHandle,38,5,50,17);
	boxTitle = new View::Text(outputHandle, L"��¼", 23, 2, 10, 1);
	Text* account = new View::Text(outputHandle, L"�˺ţ�", 6, 5, 6, 1);
	Text* passwd = new View::Text(outputHandle, L"���룺", 6, 9, 6, 1);
	Text* welcomeText = new View::Text(outputHandle, L"��ӭʹ�ø�����Ʒ����OS��", 52, 23, 40, 1);
	Text* welcomeText2 = new View::Text(outputHandle, L"Designed By ֣��¡", 54, 25, 30, 1);
	acInput = new View::InputText(outputHandle, L"�������˺�...", 12, 5, 30, 1);
	pwInput = new View::InputText(outputHandle, L"����������...", 12, 9, 30, 1);
	errorInfo = new View::Text(outputHandle, L"", 12, 11, 30, 1);
	View::Line* line1 = new View::Line(outputHandle, 12, 6, true, 32);
	View::Line* line2 = new View::Line(outputHandle, 12, 10, true, 32);
	leftButton = new View::Button(outputHandle, L"  ���û�", 13, 13, 12, 3);
	rightButton = new View::Button(outputHandle, L" ��¼", 31, 13, 8, 3);

	/*----------------Set Compoments----------------------*/
	account->setColorStyle(Color::LIGHTYELLOW, Color::BLACK);
	passwd->setColorStyle(Color::LIGHTYELLOW, Color::BLACK);
	//welcomeText2->setColorStyle(Color::LIGHTPURPLE, Color::BLUE);
	welcomeText->setColorStyle(Color::LIGHTPURPLE, Color::BLACK);
	errorInfo->setShow(false);
	errorInfo->setColorStyle(Color::LIGHTRED, Color::BLACK);
	boxTitle->setColorStyle(Color::PALELIGHTGREEN,Color::BLACK);
	rightButton->onClickFun = std::bind(&LoginPage::confirmLogin, this, std::placeholders::_1, std::placeholders::_2);
	leftButton->onClickFun = std::bind(&LoginPage::toggleSignUp, this, std::placeholders::_1, std::placeholders::_2);
	//acInput->setText(L"201901");
	//pwInput->setText(L"201901");

	/*----------------register Compoments----------------------*/

	std::vector<ComponentBase*> nullCom;

	std::vector<ComponentBase*> loginBoxAllCom = { 
		boxTitle,account,passwd,line1,line2,leftButton,rightButton,errorInfo,
		acInput,pwInput
	};
	std::vector<ComponentBase*> loginBoxMouseCom = { leftButton,rightButton,
		acInput,pwInput
	};
	loginBox->registerComponents(loginBoxAllCom, loginBoxMouseCom, nullCom);
	std::set<ComponentBase*> allComponents = { loginBox,welcomeText,welcomeText2 };
	std::set<ComponentBase*> mouseComponents = { loginBox };
	std::set<ComponentBase*> keyComponents = { loginBox };
	registerComponents(allComponents, mouseComponents, keyComponents);
}

int LoginPage::confirmLogin(View::MouseEvent& e, View::ComponentBase* com)
{

	std::wstring pw = pwInput->getText();
	int ac;
	try
	{
		ac = std::stoi(acInput->getText());
	}
	catch (...)
	{
		errorInfo->setText(L"�˺ű��������֣�");
		errorInfo->setShow(true);
		return 1;
	}
	if (!((ShopApp*)app)->shop.login(ac, pw))
	{
		errorInfo->setText(L"�˺Ż��������");
		errorInfo->setShow(true);
		return 1;
	}
	errorInfo->setText(L"��¼�ɹ���");
	errorInfo->setShow(true);
	app->redirectTo(L"main");
	return 0;
	// some operation
}


int LoginPage::confirmSignUp(View::MouseEvent& e, View::ComponentBase* com)
{
	std::wstring pw = pwInput->getText();
	int ac;
	try
	{
		ac = std::stoi(acInput->getText());
	}
	catch (...)
	{
		errorInfo->setText(L"�˺ű��������֣�");
		errorInfo->setShow(true);
		return 1;
	}
	if (!((ShopApp*)app)->shop.signUp(ac, pw))
	{
		errorInfo->setText(L"�˺��Ѵ��ڻ�����̫����");
		errorInfo->setShow(true);
		return 1;
	}
	errorInfo->setText(L"ע��ɹ���");
	errorInfo->setShow(true);
	return 0;
}


int LoginPage::toggleSignUp(View::MouseEvent& e, View::ComponentBase* com)
{
	boxTitle->setText(L"ע��");
	leftButton->setText(L" �����˺�");
	rightButton->setText(L" ע��");

	leftButton->onClickFun = std::bind(&LoginPage::toggleLogin, this, std::placeholders::_1, std::placeholders::_2);
	rightButton->onClickFun = std::bind(&LoginPage::confirmSignUp, this, std::placeholders::_1, std::placeholders::_2);

	return 0;
}

int LoginPage::toggleLogin(View::MouseEvent& e, View::ComponentBase* com)
{
	boxTitle->setText(L"��¼");
	leftButton->setText(L"  ���û�");
	rightButton->setText(L" ��¼");

	rightButton->onClickFun = std::bind(&LoginPage::confirmLogin, this, std::placeholders::_1, std::placeholders::_2);
	leftButton->onClickFun = std::bind(&LoginPage::toggleSignUp, this, std::placeholders::_1, std::placeholders::_2);

	return 0;
}
