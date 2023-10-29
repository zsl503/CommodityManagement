#include "DiscountPage.h"
#include "ShopApp.h"
using namespace Custom;
using csvop::CSVData;
void DiscountPage::initComponents()
{
	/*----------------New Compoments----------------------*/

	View::Line* line1 = new View::Line(outputHandle, 0, 1, true, 90);
	View::Line* line2 = new View::Line(outputHandle, 14, 2, false, 28);
	View::Line* line3 = new View::Line(outputHandle, 62, 2, false, 28);
	View::Line* line4 = new View::Line(outputHandle, 90, 0, false, 30);
	View::Line* line5 = new View::Line(outputHandle, 91, 0, false, 30);
	View::Line* searchLine = new View::Line(outputHandle, 24, 4, true, 18);

	TopButton* button = new TopButton(outputHandle, L" 返回", 0, 0, 8, 1);
	TopButton* button2 = new TopButton(outputHandle, L" 刷新", 0, 0, 8, 1);
	Span* topSpan = new Span(outputHandle, 0, 0, 40, 1, -1);
	View::Text* rightTitle = new View::Text(outputHandle, L"编辑", 93, 0, 10, 1);
	//View::Text* title = new View::Text(outputHandle, L"复古商品管理OS", 0, 0, 16, 1);
	Button* search = new Button(outputHandle, L" 搜索", 16, 2, 8, 3);
	modeButton = new Button(outputHandle, L"   +", 52, 2, 9, 3);
	TopButton* lastComPage = new TopButton(outputHandle, L" 上一页", 25, 28, 10, 1);
	TopButton* nextComPage = new TopButton(outputHandle, L" 下一页", 44, 28, 10, 1);

	TopButton* lastDisPage = new TopButton(outputHandle, L"上页", 0, 0, 6, 1);
	TopButton* nextDisPage = new TopButton(outputHandle, L"下页", 7, 0, 6, 1);
	TopButton* addDis = new TopButton(outputHandle, L"添加", 3, 2, 6, 1);
	LayoutBase* layout = new LayoutBase(outputHandle, Point(0, 27), 14, 3);

	TopButton* lastDetailPage = new TopButton(outputHandle, L"上页", 0, 0, 6, 1);
	TopButton* nextDetailPage = new TopButton(outputHandle, L"下页", 12, 0, 6, 1);
	clearButton = new TopButton(outputHandle, L"清空", 6, 2, 6, 1);
	LayoutBase* layout2 = new LayoutBase(outputHandle, Point(68, 27), 18, 3);

	Button* delButton = new Button(outputHandle, L" 删除", 97, 27, 8, 3);
	Button* saveButton = new Button(outputHandle, L" 保存", 111, 27, 8, 3);

	View::Text* partIndex = new View::Text(outputHandle, L"|", 40, 28, 12, 1);
	maxIndex = new View::Text(outputHandle, L"1", 41, 28, 3, 1);
	curIndex = new View::InputText(outputHandle, L"1", 38, 28, 2, 1);
	searchText = new InputText(outputHandle, L"输入编号/名称", 24, 3, 18, 1);

	leftList = new AdvList(outputHandle, L" 优惠编号", 0, 2, 14, 27);
	rightList = new CommodityList(outputHandle, L"         商品列表", 63, 2, 27, 27);
	rightList->setColWidth({ 4,10,9 });
	table = new Custom::Table(outputHandle, { L" 编号",L" 名称",L" 类型", L" 价格" }, 16, 5, 45, 23);
	table->setColWidth({ 7 });
	rightUnits = new Custom::DiscountEdit(outputHandle, Point(94, 2), 25, 25);

	/*----------------Set Compoments----------------------*/

	lastComPage->onClickFun = std::bind(&DiscountPage::lastComPage, this, std::placeholders::_1, std::placeholders::_2);
	nextComPage->onClickFun = std::bind(&DiscountPage::nextComPage, this, std::placeholders::_1, std::placeholders::_2);

	lastDetailPage->onClickFun = std::bind(&DiscountPage::lastDetailPage, this, std::placeholders::_1, std::placeholders::_2);
	nextDetailPage->onClickFun = std::bind(&DiscountPage::nextDetailPage, this, std::placeholders::_1, std::placeholders::_2);

	lastDisPage->onClickFun = std::bind(&DiscountPage::lastDisPage, this, std::placeholders::_1, std::placeholders::_2);
	nextDisPage->onClickFun = std::bind(&DiscountPage::nextDisPage, this, std::placeholders::_1, std::placeholders::_2);


	addDis->onClickFun = std::bind(&DiscountPage::onAdd, this, std::placeholders::_1, std::placeholders::_2);
	button->onClickFun = std::bind(&DiscountPage::onBack, this, std::placeholders::_1, std::placeholders::_2);
	button2->onClickFun = std::bind(&DiscountPage::onUpdate, this, std::placeholders::_1, std::placeholders::_2);
	saveButton->onClickFun = std::bind(&DiscountPage::onSave, this, std::placeholders::_1, std::placeholders::_2);
	modeButton->onClickFun = std::bind(&DiscountPage::onMode, this, std::placeholders::_1, std::placeholders::_2);
	clearButton->onClickFun = std::bind(&DiscountPage::onClear, this, std::placeholders::_1, std::placeholders::_2);
	search->onClickFun = std::bind(&DiscountPage::onSearch, this, std::placeholders::_1, std::placeholders::_2);
	leftList->onClickItem = std::bind(&DiscountPage::clickListItem, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	table->onClickItem = std::bind(&DiscountPage::clickTableItem, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	table->tdProcess = Shop::commodityTdProcess;
	rightList->tdProcess = Shop::commodityListTdProcess;
	rightTitle->setColorStyle(View::Color::LIGHTBLUE, View::Color::BLACK);

	/*----------------register Compoments----------------------*/
	std::vector<ComponentBase*> nullCom;
	std::vector<ComponentBase*> layoutCom = { addDis,lastDisPage,nextDisPage };
	layout->registerComponents(layoutCom, layoutCom, nullCom);

	std::vector<ComponentBase*> layout2Com = { clearButton,lastDetailPage,nextDetailPage };
	layout2->registerComponents(layout2Com, layout2Com, nullCom);

	std::vector<ComponentBase*> topSpanAllCom = { button,button2 };
	std::vector<ComponentBase*> topSpanMouseCom = { button,button2 };
	topSpan->registerComponents(topSpanAllCom, topSpanMouseCom, nullCom);


	std::set<ComponentBase*> allComponents = {
	topSpan,search,modeButton,lastComPage,nextComPage,rightTitle,
	line1,line2,line3,line4,line5,searchLine,layout,layout2,delButton,saveButton,
	table,partIndex,maxIndex,curIndex,leftList,rightList,rightUnits,searchText
	};
	std::set<ComponentBase*> mouseComponents = { rightUnits,topSpan,search,modeButton,leftList,layout,curIndex,
	lastComPage,nextComPage,layout2,searchText,delButton,saveButton,table };
	std::set<ComponentBase*> keyComponents = {};
	registerComponents(allComponents, mouseComponents, keyComponents);
}

void DiscountPage::mounted()
{
	maxIndex->setText(table->getMaxIndex());
}

void DiscountPage::created()
{
	leftList->setTableData(((ShopApp*)app)->shop.getDiscountIds());
	table->setTableData(((ShopApp*)app)->shop.getCommodities(true));
	rightUnits->set();
	rightList->clearData();

	rightList->setTableData(((ShopApp*)app)->shop.getDiscountDetail(-1));
}

int DiscountPage::onUpdate(View::MouseEvent& e, View::ComponentBase* com)
{
	update();
	return 0;
}

int DiscountPage::onBack(View::MouseEvent& e, View::ComponentBase* com)
{
	app->navigateBack();
	return 0;
}

int DiscountPage::onSearch(View::MouseEvent& e, View::ComponentBase* com)
{
	table->setTableData(((ShopApp*)app)->shop.searchCommodities(searchText->getText(), true));
	return 0;
}

int DiscountPage::onSave(View::MouseEvent& e, View::ComponentBase* com)
{
	int res = ((ShopApp*)app)->shop.saveDiscount(rightUnits->getDiscount(), rightList->getTableData());
	switch (res)
	{
	case 1:break;
	case 2:break;
	}
	leftList->setTableData(((ShopApp*)app)->shop.getDiscountIds());
	return 0;
}

int DiscountPage::onAdd(View::MouseEvent& e, View::ComponentBase* com)
{
	rightUnits->set();
	rightList->clearData();
	return 0;
}

int DiscountPage::onMode(View::MouseEvent& e, View::ComponentBase* com)
{
	if (isAdd)
	{
		modeButton->setText(L"   -");
		isAdd = false;
	}
	else
	{
		modeButton->setText(L"   +");
		isAdd = true;
	}
	return 0;
}

int DiscountPage::onClear(View::MouseEvent& e, View::ComponentBase* com)
{
	rightList->clearData();
	return 0;
}

int DiscountPage::nextComPage(View::MouseEvent& e, View::ComponentBase* com)
{
	if (table->getMaxIndex() <= table->getCurIndex())
		return 1;
	table->setIndex(table->getCurIndex() + 1);
	curIndex->setText(table->getCurIndex());
	return 0;
}

int DiscountPage::lastComPage(View::MouseEvent& e, View::ComponentBase* com)
{
	if (table->getCurIndex() <= 1)
		return 1;
	table->setIndex(table->getCurIndex() - 1);
	curIndex->setText(table->getCurIndex());
	return 0;
}

int DiscountPage::nextDisPage(View::MouseEvent& e, View::ComponentBase* com)
{
	if (leftList->getMaxIndex() <= leftList->getCurIndex())
		return 1;
	leftList->setIndex(leftList->getCurIndex() + 1);
	return 0;
}

int DiscountPage::lastDisPage(View::MouseEvent& e, View::ComponentBase* com)
{
	if (leftList->getCurIndex() <= 1)
		return 1;
	leftList->setIndex(leftList->getCurIndex() - 1);
	return 0;
}

int DiscountPage::nextDetailPage(View::MouseEvent& e, View::ComponentBase* com)
{
	if (rightList->getMaxIndex() <= rightList->getCurIndex())
		return 1;
	rightList->setIndex(rightList->getCurIndex() + 1);
	return 0;
}

int DiscountPage::lastDetailPage(View::MouseEvent& e, View::ComponentBase* com)
{
	if (rightList->getCurIndex() <= 1)
		return 1;
	rightList->setIndex(rightList->getCurIndex() - 1);
	return 0;
}

int DiscountPage::clickListItem(MouseEvent& e, Custom::TableItem& item, std::vector<CSVData>& data, size_t pageIndex, size_t dataIndex)
{
	rightList->setTableData(((ShopApp*)app)->shop.getDiscountDetail(data[0].getInt()));
	rightUnits->set(((ShopApp*)app)->shop.getDiscountInfo(data[0].getInt()));

	//rightList->
	return 0;
}

int DiscountPage::clickTableItem(MouseEvent& e, Custom::TableItem& item, std::vector<CSVData>& data, size_t pageIndex, size_t dataIndex)
{
	int index = rightList->getCurIndex();
	if (isAdd)
		rightList->addCommodity(data[0].getInt(), data[1].getStr(), data[3].getDouble());
	else
		rightList->delCommodity(data[0].getInt());
	if (index == 0 || index > rightList->getMaxIndex())
		index = 1;
	rightList->setIndex(index);
	return 0;
}