#pragma once
#include "Components.h"
#include "App.h"
class DiscountPage :
    public View::PageBase
{
public:
	DiscountPage() {}
	void initComponents();
	void mounted();
	void created();
	int onUpdate(View::MouseEvent& e, View::ComponentBase* com);
	int onBack(View::MouseEvent& e, View::ComponentBase* com);
	int onSearch(View::MouseEvent& e, View::ComponentBase* com);
	int onAdd(View::MouseEvent& e, View::ComponentBase* com);
	int onMode(View::MouseEvent& e, View::ComponentBase* com);
	int onClear(View::MouseEvent& e, View::ComponentBase* com);
	int onSave(View::MouseEvent& e, View::ComponentBase* com);

	int clickListItem(View::MouseEvent& e, Custom::TableItem& item, std::vector<csvop::CSVData>& data, size_t pageIndex, size_t dataIndex);
	int clickTableItem(View::MouseEvent& e, Custom::TableItem& item, std::vector<csvop::CSVData>& data, size_t pageIndex, size_t dataIndex);


	int nextComPage(View::MouseEvent& e, View::ComponentBase* com);
	int lastComPage(View::MouseEvent& e, View::ComponentBase* com);
	int nextDisPage(View::MouseEvent& e, View::ComponentBase* com);
	int lastDisPage(View::MouseEvent& e, View::ComponentBase* com);

	int nextDetailPage(View::MouseEvent& e, View::ComponentBase* com);
	int lastDetailPage(View::MouseEvent& e, View::ComponentBase* com);
private:
	Custom::Table* table = NULL;
	View::Text* maxIndex = NULL;
	View::InputText* curIndex = NULL;
	View::InputText* searchText = NULL;
	Custom::AdvList* leftList = NULL;
	Custom::CommodityList* rightList = NULL;
	Custom::DiscountEdit* rightUnits = NULL;
	Custom::TopButton* clearButton = NULL;
	View::Button* modeButton = NULL;
	bool isAdd = true;
};

