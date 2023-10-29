#include "MainPage.h"
#include "ShopApp.h"
using csvop::CSVData;
using namespace Custom;
void MainPage::initComponents()
{
	/*----------------New Compoments----------------------*/

	View::Line* line1 = new View::Line(outputHandle, 0, 1, true, 90);
	View::Line* line2 = new View::Line(outputHandle, 16, 2, false, 28);
	View::Line* line3 = new View::Line(outputHandle, 90, 0, false, 30);
	View::Line* line4 = new View::Line(outputHandle, 91, 0, false, 30);
	View::Line* searchLine = new View::Line(outputHandle, 27, 4, true, 30);

	TopButton* button = new TopButton(outputHandle, L" 菜单", 0, 0, 8, 1);
	TopButton* button2 = new TopButton(outputHandle, L" 刷新", 0, 0, 8, 1);
	TopButton* button3 = new TopButton(outputHandle, L" 退出登录", 0, 0, 12, 1);
	TopButton* lastPage = new TopButton(outputHandle, L" 上一页", 35, 28, 10, 1);
	TopButton* nextPage = new TopButton(outputHandle, L" 下一页", 63, 28, 10, 1);
	Button* search = new Button(outputHandle, L" 搜索", 19, 2, 8, 3);
	addButton = new Button(outputHandle, L" 添加", 80, 2, 8, 3);

	leftButton = new LeftButton(outputHandle, L"\n 购物", 0, 2, 16, 4);
	leftButton2 = new LeftButton(outputHandle, L"\n 商品信息", 0, 5, 16, 4);
	leftButton3 = new LeftButton(outputHandle, L"\n 订单信息", 0, 9, 16, 4);
	leftButton4 = new LeftButton(outputHandle, L"\n 进货信息", 0, 13, 16, 4);
	leftButton5 = new LeftButton(outputHandle, L"\n 优惠管理", 0, 13, 16, 4);
	leftButton6 = new LeftButton(outputHandle, L"\n 统计信息", 0, 13, 16, 4);

	cancalButton = new View::Button(outputHandle, L" 取消", 97, 27, 8, 3);
	saveButton = new View::Button(outputHandle, L" 保存", 111, 27, 8, 3);

	View::Text* title = new View::Text(outputHandle, L"复古商品管理OS", 0, 0, 16, 1);
	View::Text* partIndex = new View::Text(outputHandle, L"|", 54, 28, 12, 1);
	maxIndex = new View::Text(outputHandle, L"", 55, 28, 3, 1);
	curIndex = new View::InputText(outputHandle, L"", 52, 28, 2, 1);
	rightTitle = new View::Text(outputHandle, L"购物车", 93, 0, 10, 1);
	errorInfo = new View::Text(outputHandle, L"", 60, 0, 28, 1);

	searchText = new InputText(outputHandle, L"请输入商品名称/种类", 28, 3, 24, 1);

	Span* topSpan = new Span(outputHandle, 16, 0, 40, 1, -1);
	Span* editButtons = new Span(outputHandle, 97, 27, 50, 3, 6);
	Div* leftDiv = new Div(outputHandle, 0, 2, 16, 40, 0);

	InputUnit* units1[5] = {
		new InputUnit(outputHandle, L"名称", 0, 0, 25, 3),new InputUnit(outputHandle, L"型号", 0, 0, 25, 3),
		new InputUnit(outputHandle, L"种类", 0, 0, 25, 3), new InputUnit(outputHandle, L"售价", 0, 0, 25, 3),
		new InputUnit(outputHandle, L"状态", 0,0, 25, 3)
	};

	tables[0] = new Custom::Table(outputHandle, { L" 商品号",L" 名称",L" 型号",L" 种类",L" 详情", L" 价格" }, 19, 5, 69, 23);
	tables[1] = new Custom::Table(outputHandle, { L" 商品号",L" 名称",L" 型号",L" 种类",L" 售价",L"状态",L"库存" }, 19, 5, 69, 23);
	tables[2] = new Custom::Table(outputHandle, { L" 订单号",L" 总价(优惠后)",L" 订单时间",L" 优惠号" }, 19, 5, 69, 23);
	tables[3] = new Custom::Table(outputHandle, { L" 进货号",L" 商品号",L" 进价", L" 进货时间",L"进货渠道",L"进货数量" }, 19, 5, 69, 23);
	tables[4] = new Custom::Table(outputHandle, { L" 优惠号",L" 最低消",L" 优惠价",L" 优惠类型",L" 优惠详情" }, 19, 5, 69, 23);
	tables[5] = new Custom::Table(outputHandle, { L" 商品号",L" 名称",L" 型号",L" 种类",L" 销量" }, 19, 5, 69, 23);

	/*----------------Set Compoments----------------------*/
	title->setColorStyle(View::Color::PALELIGHTGREEN, View::Color::BLACK);
	rightTitle->setColorStyle(View::Color::LIGHTBLUE, View::Color::BLACK);
	errorInfo->setShow(false);
	errorInfo->setColorStyle(Color::LIGHTRED, Color::BLACK);

	//addButton->onClickFun = std::bind(&MainPage::addRow, this, std::placeholders::_1, std::placeholders::_2);
	addButton->setShow(false);

	search->onClickFun = std::bind(&MainPage::onSearch, this, std::placeholders::_1, std::placeholders::_2);
	leftButton->onClickFun = std::bind(&MainPage::onBuy, this, std::placeholders::_1, std::placeholders::_2);
	leftButton2->onClickFun = std::bind(&MainPage::onCommodity, this, std::placeholders::_1, std::placeholders::_2);
	leftButton3->onClickFun = std::bind(&MainPage::onSale, this, std::placeholders::_1, std::placeholders::_2);
	leftButton4->onClickFun = std::bind(&MainPage::onPurchase, this, std::placeholders::_1, std::placeholders::_2);
	leftButton5->onClickFun = std::bind(&MainPage::onDiscount, this, std::placeholders::_1, std::placeholders::_2);
	leftButton6->onClickFun = std::bind(&MainPage::onStatistics, this, std::placeholders::_1, std::placeholders::_2);
	button2->onClickFun = std::bind(&MainPage::onUpdate, this, std::placeholders::_1, std::placeholders::_2);
	button3->onClickFun = std::bind(&MainPage::onLogout, this, std::placeholders::_1, std::placeholders::_2);
	for (size_t i = 0; i < 6; i++)
	{
		tables[i]->onClickItem = std::bind(&MainPage::clickTableItem, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	}

	lastPage->onClickFun = std::bind(&MainPage::lastPage, this, std::placeholders::_1, std::placeholders::_2);
	nextPage->onClickFun = std::bind(&MainPage::nextPage, this, std::placeholders::_1, std::placeholders::_2);
	curIndex->textChanged = std::bind(&MainPage::setPage, this, std::placeholders::_1);
	tables[0]->setShow(false), tables[1]->setShow(false), tables[2]->setShow(false);
	tables[3]->setShow(false), tables[4]->setShow(false), tables[5]->setShow(false);
	tables[1]->setColWidth({ 8,10,10,10,10,5,5 });
	tables[4]->setColWidth({ 8,10,12,10,23 });
	/*----------------register Compoments----------------------*/
	std::vector<ComponentBase*> nullCom;

	std::vector<ComponentBase*> topSpanAllCom = { button,button2,button3 };
	std::vector<ComponentBase*> topSpanMouseCom = { button,button2,button3 };
	topSpan->registerComponents(topSpanAllCom, topSpanMouseCom, nullCom);

	std::vector<ComponentBase*> leftDivAllCom = { leftButton,leftButton2,leftButton3,leftButton4,leftButton5,leftButton6 };
	leftDiv->registerComponents(leftDivAllCom, leftDivAllCom, nullCom);

	std::vector<ComponentBase*> editButtonsAllCom = { cancalButton,saveButton };
	std::vector<ComponentBase*> editButtonsMouseCom = { cancalButton,saveButton };
	editButtons->registerComponents(editButtonsAllCom, editButtonsMouseCom, nullCom);

	std::set<ComponentBase*> allComponents = {
		topSpan,leftDiv,editButtons,search,addButton,
		line1,line2,line3,line4,searchLine,lastPage,nextPage,
		searchText,title,rightTitle,partIndex,maxIndex,errorInfo,curIndex,
		rightDiv
	};
	std::set<ComponentBase*> mouseComponents = { topSpan,leftDiv,editButtons,searchText,search,addButton,lastPage,nextPage
	,curIndex,rightDiv };
	std::set<ComponentBase*> keyComponents = { button };
	registerComponents(allComponents, mouseComponents, keyComponents);
}

void MainPage::mounted()
{
}

void MainPage::created()
{
	switchTable(0);
}

int MainPage::onAddCommodity(View::MouseEvent& e, View::ComponentBase* com)
{
	CommodityEdit* p = (CommodityEdit*)rightDiv;
	p->set({ L"", L"", L"", L"", L"", L"", L"" });
	return 0;
}

int MainPage::onAddPurchase(View::MouseEvent& e, View::ComponentBase* com)
{
	cancalButton->setShow(true);
	saveButton->setShow(true);
	rightTitle->setText(L"添加");
	PurchaseInfo* p = (PurchaseInfo*)rightDiv;
	p->set();
	return 0;
}

int MainPage::onEditDiscount(View::MouseEvent& e, View::ComponentBase* com)
{
	app->navigateTo(L"discount");
	return 0;
}

int MainPage::onLogout(View::MouseEvent& e, View::ComponentBase* com)
{
	app->redirectTo(L"login");
	return 0;
}

int MainPage::onUpdate(View::MouseEvent& e, View::ComponentBase* com)
{
	update();
	return 0;
}

int MainPage::onSearch(View::MouseEvent& e, View::ComponentBase* com)
{
	try {
		unsetError();
		csvdb::table::TableData td;
		switch (index)
		{
		case 0:td = ((ShopApp*)app)->shop.searchShelves(searchText->getText()); break;
		case 1:td = ((ShopApp*)app)->shop.searchCommodities(searchText->getText()); break;
		case 2:td = ((ShopApp*)app)->shop.searchSale(searchText->getText()); break;
		case 3:td = ((ShopApp*)app)->shop.searchPurchase(searchText->getText()); break;
		case 4:td = ((ShopApp*)app)->shop.searchDiscount(searchText->getText()); break;
		case 5:td = ((ShopApp*)app)->shop.searchStatistics(searchText->getText()); break;
		}

		if (td.length())
			curTable->setTableData(td);
		else
			setError(L"无结果！");
		return 0;
	}
	catch (...)
	{
		setError(L"出错啦！");
		return 1;
	}
}

int MainPage::onBuy(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	if (switchTable(0) == -1)
		setError(L"打开列表失败！");
	return 0;
}

int MainPage::onCommodity(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	if (switchTable(1) == -1)
		setError(L"打开列表失败！");
	return 0;
}

int MainPage::onPurchase(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	if (switchTable(3) == -1)
		setError(L"打开列表失败！");
	return 0;
}

int MainPage::onDiscount(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	if (switchTable(4) == -1)
		setError(L"打开列表失败！");
	return 0;
}

int MainPage::onSale(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	if (switchTable(2) == -1)
		setError(L"打开列表失败！");
	return 0;
}

int MainPage::onStatistics(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	if (switchTable(5) == -1)
		setError(L"打开列表失败！");
	//else
	//	curTable->setTableData(((ShopApp*)app)->shop.getShelves());
	return 0;
}

int MainPage::onPlay(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	if (switchTable(5) == -1)
		setError(L"打开游戏失败！");
	//else
	//	curTable->setTableData(((ShopApp*)app)->shop.getShelves());
	return 0;
}

int MainPage::onConfirmBuy(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	try
	{
		ShoppingCart* p = ((ShoppingCart*)rightDiv);
		bool res = ((ShopApp*)app)->shop.saveSales(p->getShop(), p->getTotalPrice(),p->getDiscountId());
		if(!res)
			setError(L"失败,商品或优惠不可用！");
		else
		{
			setError(L"\t成功下单！");
			p->clearShop();
		}
		return true;
	}
	catch (...)
	{
		setError(L"下单失败，请检查输入！");
		return 1;
	}
}

int MainPage::onSavePurchase(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	PurchaseInfo* p = ((PurchaseInfo*)rightDiv);
	int res = ((ShopApp*)app)->shop.savePurchase(p->getPurchase());
	if (res == 0)
	{
		curTable->setTableData(((ShopApp*)app)->shop.getPurchase());
		p->clearInput();
	}
	else if(res == 2)
		setError(L"失败，商品号不存在！");
	else
		setError(L"失败，请检查输入！");
	return res;
}

int MainPage::onSaveCommodity(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	CommodityEdit* p = ((CommodityEdit*)rightDiv);
	if (((ShopApp*)app)->shop.saveCommodity(p->getCommodity()))
	{
		curTable->setTableData(((ShopApp*)app)->shop.getCommodities());
		return 0;
	}
	else
	{
		setError(L"保存失败，请检查输入是否正确！");
		return 1;
	}
}

int MainPage::onRemoveCommodity(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	CommodityEdit* p = ((CommodityEdit*)rightDiv);
	if (p->getCommodityId() == INT_MAX)
	{
		return 0;
	}
	if (((ShopApp*)app)->shop.removeCommodity(p->getCommodityId()))
	{
		curTable->setTableData(((ShopApp*)app)->shop.getCommodities());
		CommodityEdit* p = (CommodityEdit*)rightDiv;
		p->set({ L"", L"", L"", L"", L"", L"", L"" });
		return 0;
	}
	else
	{
		setError(L"			删除失败！");
		return 1;
	}
}

int MainPage::onClearPurchase(View::MouseEvent& e, View::ComponentBase* com)
{
	unsetError();
	PurchaseInfo* p = ((PurchaseInfo*)rightDiv);
	p->clearInput();
	return 0;
}

int MainPage::nextPage(View::MouseEvent& e, View::ComponentBase* com)
{
	if (curTable->getMaxIndex() <= curTable->getCurIndex())
		return 1;
	curTable->setIndex(curTable->getCurIndex() + 1);
	curIndex->setText(curTable->getCurIndex());
	return 0;
}

int MainPage::lastPage(View::MouseEvent& e, View::ComponentBase* com)
{
	if (curTable->getCurIndex() <= 1)
		return 1;
	curTable->setIndex(curTable->getCurIndex() - 1);
	curIndex->setText(curTable->getCurIndex());
	return 0;
}

int MainPage::setPage(View::InputText* com)
{
	size_t t;
	try
	{
		t = std::stoi(com->getText());
	}
	catch (...)
	{
		com->setText(1);
		t = 1;
	}

	if (!curTable->setIndex(t))
	{
		com->setText(curTable->getCurIndex());
		return 1;
	}
		
	return 0;
}

int MainPage::clickTableItem(MouseEvent& e, Custom::TableItem& item, std::vector<CSVData>& data, size_t pageIndex, size_t dataIndex)
{
	switch (index)
	{
	case 0:
	{
		ShoppingCart* p = ((ShoppingCart*)rightDiv);
		p->addShop(data[0].getInt(), data[1].getStr(), data[5].getDouble());
		//p ->addShop(item->getText(0));
		break;

	}
	case 1:
	{
		CommodityEdit* p = ((CommodityEdit*)rightDiv);
		p->set(((ShopApp*)app)->shop.getCommodity(data[0].getInt()));
		//p->set(data[0].getInt(), data[1].getStr(), data[5].getDouble());
		break;
	}
	case 2:
	{
		SaleDetail* p = ((SaleDetail*)rightDiv);
		p->set(data[0].getInt(), item.getText(1), ((ShopApp*)app)->shop.getSaleDetail(data[0].getInt()));
		break;
	}
	case 3:
	{
		cancalButton->setShow(false);
		saveButton->setShow(false);
		rightTitle->setText(L"详情");
		PurchaseInfo* p = ((PurchaseInfo*)rightDiv);
		p->set(((ShopApp*)app)->shop.getPurchaseDetail(data[0].getInt()));
		break;
	}
	case 4:
	{
		DiscountInfo* p = ((DiscountInfo*)rightDiv);
		p->set(((ShopApp*)app)->shop.getDiscountInfo(data[0].getInt()));
		break;
	}
	}

	return 0;
}

int MainPage::clearShop(View::MouseEvent& e, View::ComponentBase* com)
{
	ShoppingCart* p = ((ShoppingCart*)rightDiv);
	p->clearShop();
	return 0;
}

int MainPage::switchTable(int tableIndex)
{
	if (tableIndex >= sizeof(tables) / sizeof(tables[0]))
		return -1;
	if(curTable == tables[tableIndex])
		return tableIndex;
	if (curTable != NULL)
	{
		curTable->setShow(false);
		curTable->update();
		detachComponent(curTable);
	}
	curTable = tables[tableIndex];
	curTable->setShow(true);
	addComponent(curTable);

	index = tableIndex;
	if(curButton!=NULL)
		curButton->setColorStyle(Color::WHITE,Color::BLACK);
	switch (index)
	{
	case 0:
	{
		curButton = leftButton;
		addButton->setShow(false);
		cancalButton->setShow(true);
		saveButton->setShow(true);
		
		searchText->setText(L"请输入商品名称/种类"); 
		rightTitle->setText(L"购物");
		cancalButton->setText(L" 清空");
		saveButton->setText(L" 下单");
		
		curTable->tdProcess = Shop::shelfTdProcess;
		curTable->setTableData(((ShopApp*)app)->shop.getShelves());
		cancalButton->onClickFun = std::bind(&MainPage::clearShop, this, std::placeholders::_1, std::placeholders::_2);
		saveButton->onClickFun = std::bind(&MainPage::onConfirmBuy, this, std::placeholders::_1, std::placeholders::_2);

		removeComponent(rightDiv);
		rightDiv = new ShoppingCart(outputHandle, 93, 2, 26, 24);
		addComponent(rightDiv);
		break;
	}
	case 1:
	{
		curButton = leftButton2;
		addButton->setShow(true);
		cancalButton->setShow(true);
		saveButton->setShow(true);

		searchText->setText(L"请输入商品名称/种类/状态");
		rightTitle->setText(L"编辑");
		cancalButton->setText(L" 删除");
		saveButton->setText(L" 保存");
		addButton->setText(L" 添加");

		curTable->tdProcess = Shop::commodityTdProcess;
		curTable->setTableData(((ShopApp*)app)->shop.getCommodities());
		addButton->onClickFun = std::bind(&MainPage::onAddCommodity, this, std::placeholders::_1, std::placeholders::_2);
		saveButton->onClickFun = std::bind(&MainPage::onSaveCommodity, this, std::placeholders::_1, std::placeholders::_2);
		cancalButton->onClickFun = std::bind(&MainPage::onRemoveCommodity, this, std::placeholders::_1, std::placeholders::_2);
		
		removeComponent(rightDiv);
		rightDiv = new CommodityEdit(outputHandle, 93, 2, 26, 27);
		addComponent(rightDiv);
		break;
	}
	case 2:
	{
		curButton = leftButton3;
		addButton->setShow(false);
		cancalButton->setShow(false);
		saveButton->setShow(false);

		searchText->setText(L"请输入订单号"); 
		rightTitle->setText(L"订单详情");
		
		curTable->tdProcess = Shop::saleTdProcess;
		curTable->setTableData(((ShopApp*)app)->shop.getSale());
		
		removeComponent(rightDiv);
		rightDiv = new SaleDetail(outputHandle, 93, 2, 27, 27);
		addComponent(rightDiv);
		break;
	}
	case 3:
	{
		curButton = leftButton4;
		addButton->setShow(true);
		cancalButton->setShow(false);
		saveButton->setShow(false);

		searchText->setText(L"请输入进货号/商品号");
		rightTitle->setText(L"编辑");
		cancalButton->setText(L" 清空");
		saveButton->setText(L" 保存");
		addButton->setText(L" 添加");

		addButton->onClickFun = std::bind(&MainPage::onAddPurchase, this, std::placeholders::_1, std::placeholders::_2);
		saveButton->onClickFun = std::bind(&MainPage::onSavePurchase, this, std::placeholders::_1, std::placeholders::_2);
		cancalButton->onClickFun = std::bind(&MainPage::onClearPurchase, this, std::placeholders::_1, std::placeholders::_2);
		curTable->tdProcess = Shop::purchaseTdProcess;
		curTable->setTableData(((ShopApp*)app)->shop.getPurchase());

		removeComponent(rightDiv);
		rightDiv = new PurchaseInfo(outputHandle, 93, 2, 27, 27);
		addComponent(rightDiv);
		break;
	}
	case 4:
	{
		curButton = leftButton5;
		addButton->setShow(true);
		cancalButton->setShow(true);
		saveButton->setShow(false);

		searchText->setText(L"请输入优惠号");
		rightTitle->setText(L"优惠单");
		cancalButton->setText(L" 删除");
		addButton->setText(L" 编辑");

		curTable->tdProcess = Shop::discountTdProcess;
		curTable->setTableData(((ShopApp*)app)->shop.getDiscount());
		addButton->onClickFun = std::bind(&MainPage::onEditDiscount, this, std::placeholders::_1, std::placeholders::_2);

		removeComponent(rightDiv);
		rightDiv = new DiscountInfo(outputHandle, 93, 4, 27, 24);
		addComponent(rightDiv);
		break;
	}
	case 5:
	{
		curButton = leftButton6;
		addButton->setShow(false);
		cancalButton->setShow(false);
		saveButton->setShow(false);

		searchText->setText(L"请输入日期(M/D/Y-M/D/Y)");
		rightTitle->setText(L"统计");

		curTable->tdProcess = NULL;
		curTable->setTableData(((ShopApp*)app)->shop.getStatistics());

		removeComponent(rightDiv);
		rightDiv = NULL;
		//rightDiv = new DiscountInfo(outputHandle, 93, 4, 27, 24);
		//addComponent(rightDiv);
		break;
	}
	}

	maxIndex->setText(curTable->getMaxIndex());

	if (curTable->getMaxIndex() == 0)
		curIndex->setText(0);
	else
		curIndex->setText(1);

	curButton->setColorStyle(Color::RED,Color::WHITE);
	return tableIndex;
}

void MainPage::setError(std::wstring errorInfo)
{
	this->errorInfo->setText(errorInfo);
	this->errorInfo->setShow(true);
}

void MainPage::unsetError()
{
	errorInfo->setShow(false);
}
