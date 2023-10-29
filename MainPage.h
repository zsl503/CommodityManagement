#pragma once
#include "ComponentBase.h"
#include "Components.h"
#include "App.h"

class MainPage : public View::PageBase
{
public:
	MainPage(){}
	
	void initComponents();
	void mounted();
	void created();
	
	int onAddCommodity(View::MouseEvent& e, View::ComponentBase* com);	// 按下添加商品按钮
	int onAddPurchase(View::MouseEvent& e, View::ComponentBase* com);	// 按下添加进货信息按钮
	int onEditDiscount(View::MouseEvent& e, View::ComponentBase* com);	// 按下编辑优惠按钮
	int onLogout(View::MouseEvent& e, View::ComponentBase* com);	// 按下退出登录按钮
		
	int onUpdate(View::MouseEvent& e, View::ComponentBase* com);	// 按下更新按钮
	int onBuy(View::MouseEvent& e, View::ComponentBase* com);	// 按下购物按钮
	int onCommodity(View::MouseEvent& e, View::ComponentBase* com);	// 按下商品管理按钮
	int onSale(View::MouseEvent& e, View::ComponentBase* com);	// 按下订单管理按钮
	int onPurchase(View::MouseEvent& e, View::ComponentBase* com);	// 按下进货管理按钮
	int onDiscount(View::MouseEvent& e, View::ComponentBase* com);	// 按下优惠管理按钮
	int onStatistics(View::MouseEvent& e, View::ComponentBase* com);
	int onPlay(View::MouseEvent& e, View::ComponentBase* com);
	int onSearch(View::MouseEvent& e, View::ComponentBase* com);

	int onConfirmBuy(View::MouseEvent& e, View::ComponentBase* com);
	int onSaveCommodity(View::MouseEvent& e, View::ComponentBase* com);	//按下保存商品信息按钮
	int onSavePurchase(View::MouseEvent& e, View::ComponentBase* com);

	int onRemoveCommodity(View::MouseEvent& e, View::ComponentBase* com);
	int onClearPurchase(View::MouseEvent& e, View::ComponentBase* com);

	int nextPage(View::MouseEvent& e, View::ComponentBase* com);	//按下下一页翻页按钮
	int lastPage(View::MouseEvent& e, View::ComponentBase* com);	//按下上一页翻页按钮

	int setPage(View::InputText* com);	// 输入页码信息后执行跳转

	int clickTableItem(View::MouseEvent&, Custom::TableItem&, std::vector<csvop::CSVData>&, size_t, size_t);	// 按下表中的项目
	int clearShop(View::MouseEvent&, View::ComponentBase*);	// 按下清除购物车按钮

private:
	Custom::LeftButton* leftButton = NULL;
	Custom::LeftButton* leftButton2 = NULL;
	Custom::LeftButton* leftButton3 = NULL;
	Custom::LeftButton* leftButton4 = NULL;
	Custom::LeftButton* leftButton5 = NULL;
	Custom::LeftButton* leftButton6 = NULL;
	Custom::LeftButton* curButton = NULL;
	View::InputText* searchText = NULL;
	View::Text* maxIndex = NULL;
	View::InputText* curIndex;
	View::Text* errorInfo = NULL;
	View::Div* rightDiv = NULL;
	Custom::Table* tables[6];
	Custom::Table* curTable = NULL;
	View::Button* cancalButton = NULL;
	View::Button* saveButton = NULL;
	View::Button* addButton = NULL;
	View::Text* rightTitle = NULL;
	int switchTable(int tableIndex);	// 切换表类型
	void setError(std::wstring errorInfo);	// 显示错误提示
	void unsetError();	// 隐藏错误提示
	int index = 0;
};