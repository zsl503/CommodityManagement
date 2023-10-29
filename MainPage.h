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
	
	int onAddCommodity(View::MouseEvent& e, View::ComponentBase* com);	// ���������Ʒ��ť
	int onAddPurchase(View::MouseEvent& e, View::ComponentBase* com);	// ������ӽ�����Ϣ��ť
	int onEditDiscount(View::MouseEvent& e, View::ComponentBase* com);	// ���±༭�Żݰ�ť
	int onLogout(View::MouseEvent& e, View::ComponentBase* com);	// �����˳���¼��ť
		
	int onUpdate(View::MouseEvent& e, View::ComponentBase* com);	// ���¸��°�ť
	int onBuy(View::MouseEvent& e, View::ComponentBase* com);	// ���¹��ﰴť
	int onCommodity(View::MouseEvent& e, View::ComponentBase* com);	// ������Ʒ����ť
	int onSale(View::MouseEvent& e, View::ComponentBase* com);	// ���¶�������ť
	int onPurchase(View::MouseEvent& e, View::ComponentBase* com);	// ���½�������ť
	int onDiscount(View::MouseEvent& e, View::ComponentBase* com);	// �����Żݹ���ť
	int onStatistics(View::MouseEvent& e, View::ComponentBase* com);
	int onPlay(View::MouseEvent& e, View::ComponentBase* com);
	int onSearch(View::MouseEvent& e, View::ComponentBase* com);

	int onConfirmBuy(View::MouseEvent& e, View::ComponentBase* com);
	int onSaveCommodity(View::MouseEvent& e, View::ComponentBase* com);	//���±�����Ʒ��Ϣ��ť
	int onSavePurchase(View::MouseEvent& e, View::ComponentBase* com);

	int onRemoveCommodity(View::MouseEvent& e, View::ComponentBase* com);
	int onClearPurchase(View::MouseEvent& e, View::ComponentBase* com);

	int nextPage(View::MouseEvent& e, View::ComponentBase* com);	//������һҳ��ҳ��ť
	int lastPage(View::MouseEvent& e, View::ComponentBase* com);	//������һҳ��ҳ��ť

	int setPage(View::InputText* com);	// ����ҳ����Ϣ��ִ����ת

	int clickTableItem(View::MouseEvent&, Custom::TableItem&, std::vector<csvop::CSVData>&, size_t, size_t);	// ���±��е���Ŀ
	int clearShop(View::MouseEvent&, View::ComponentBase*);	// ����������ﳵ��ť

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
	int switchTable(int tableIndex);	// �л�������
	void setError(std::wstring errorInfo);	// ��ʾ������ʾ
	void unsetError();	// ���ش�����ʾ
	int index = 0;
};