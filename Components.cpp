#include "Components.h"
#include <CSVType.h>
#include "Shop.h"
using namespace Custom;
using csvop::CSVData;
using csvop::DATATYPE;

/*-------------------TopButton----------------------------*/

void TopButton::drawBorder()
{
	COORD c = point.getCOORD();
	c.X = point.getAbsX(0);
	for (int i = 0; i < height; i++)
	{
		c.Y = point.getAbsY(i);
		SetConsoleCursorPosition(outputHandle, c);
		std::wcout << L"|";
	}

	c.X = point.getAbsX(width - 1);
	for (int i = 0; i < height; i++)
	{
		c.Y = point.getAbsY(i);
		SetConsoleCursorPosition(outputHandle, c);
		std::wcout << L"|";
	}
}


/*-------------------LeftButton----------------------------*/

Custom::LeftButton::LeftButton(HANDLE outputHandle, std::wstring text, size_t abX, size_t abY, size_t width, size_t height, Color defaultForeColor, Color defaultBackColor)
	:Button(outputHandle, text, abX, abY, width, height, defaultForeColor, defaultBackColor) {
	textAbX = 0, textAbY = 1;
	textSubHeight = 0, textSubWidth = 2;
}

void LeftButton::drawBorder()
{
	COORD c = point.getCOORD();
	//SetConsoleCursorPosition(outputHandle, c);
	//for (int i = 0; i < width; i++)
	//	std::wcout << L"-";
	c.X = point.getAbsX(0), c.Y = point.getAbsY(height - 1);
	SetConsoleCursorPosition(outputHandle, c);
	for (int i = 0; i < width; i++)
		std::wcout << L"-";
}


/*-------------------TableItem----------------------------*/

TableItem::TableItem(HANDLE outputHandle, std::vector<std::wstring> rowData,
	size_t abX, size_t abY, size_t width, size_t height, Color defaultForeColor, Color defaultBackColor)
	:Span(outputHandle, abX, abY, width, height,1, defaultForeColor, defaultBackColor), hoverBackColor(defaultBackColor)
{
	setPadding(0, 0, 0, 0);
	std::vector<unsigned short> colWidth;
	unsigned short w = ((width - rowData.size() - 1)/ rowData.size());
	if (w < 2)
		throw;
	std::vector<View::ComponentBase*> com;
	
	Text* t;
	for (size_t i = 0; i < rowData.size(); i++)
	{
		t = new Text(outputHandle, rowData[i], 0, 0, w, height);
		texts.push_back(t);
		com.push_back(t);
	}
	registerComponents(com, com, com);

}

void TableItem::setRowData(std::vector<std::wstring> rowData)
{
	if (rowData.size() != texts.size())
		throw;
	for (size_t i = 0; i < rowData.size(); i++)
	{
		texts[i]->setText(rowData[i]);
	}
}

void TableItem::setColWidth(const std::vector<unsigned short>& colWidth)
{
	if (colWidth.empty())return;
	size_t i = 0,countWidth = 0,min = colWidth.size()> texts.size()? texts.size(): colWidth.size();
	for (; i < min; i++)
	{
		texts[i]->setSize(colWidth[i],texts[i]->getHeight());
		countWidth += colWidth[i];
	}
	if (i != texts.size())
	{
		size_t len = (width - countWidth - (texts.size() - i) - 1) / (texts.size() - i);
		for (; i < texts.size(); i++)
		{
			texts[i]->setSize(len, texts[i]->getHeight());
		}

	}
	setSpace(space);
	//setChange();
}

void TableItem::drawBorder()
{
	COORD c = point.getCOORD(1,0);

	for (size_t i = 1; i < texts.size(); i++)
	{
		c.X = texts[i]->getPoint().x - 1;
		for (size_t j = 0; j < height; j++)
		{
			c.Y = point.getAbsY(j);
			SetConsoleCursorPosition(outputHandle, c);
			std::wcout << L"|";
		}
	}
}

void TableItem::beginDraw()
{
	SetConsoleTextAttribute(outputHandle, int(nowForeColor) + int(nowBackColor) * 0x10);
	drawBackground();
	drawBorder();
	SetConsoleTextAttribute(outputHandle, int(defaultForeColor) + int(defaultBackColor) * 10);
	Span::beginDraw();
}

void TableItem::setHoverStyle(View::Color foreColor, View::Color backColor)
{
	hoverBackColor = backColor;
	hoverBackColor = foreColor;
}

void TableItem::setClickStyle(View::Color foreColor, View::Color backColor)
{
	clickBackColor = backColor;
	clickForeColor = foreColor;
}

std::wstring Custom::TableItem::getText(int index)
{
	return texts[index]->getText();
}

std::wstring Custom::TableItem::setText(int index, std::wstring text)
{
	std::wstring old = texts[index]->getText();
	texts[index]->setText(text);
	return old;
}

void TableItem::setColorStyle(View::Color foreColor, View::Color backColor)
{
	ComponentBase::setColorStyle(foreColor, backColor);
	for (auto i : allComponents)
		i->setColorStyle(foreColor, backColor);
	setChange();
}

ComponentBase* TableItem::onMouseEvent(MouseEvent& e)
{
	if (isPointIn(e.point))
	{
		switch (e.type)
		{
		case MouseEventType::LCLICK:
		case MouseEventType::LDOUBLECLICK:
			isHover = false;
			if (!isOnclick)
			{
				isOnclick = true;
				for (auto i : allComponents)
					i->setColorStyle(clickForeColor, clickBackColor);
				nowBackColor = clickBackColor;
				nowForeColor = clickForeColor;
				setChange();
			}
			mouseEventFun = NULL;
			break;
		default:
			if (!isHover)
			{
				for (auto i : allComponents)
					i->setColorStyle(hoverForeColor, hoverBackColor);
				nowBackColor = hoverBackColor;
				nowForeColor = hoverForeColor;
				isHover = true;
				setChange();
			}
			if (isOnclick)
			{
				mouseEventFun = onClickFun;
				isOnclick = false;
			}
			else
				mouseEventFun = NULL;
		}
	}
	else
	{
		switch (e.type)
		{
		case MouseEventType::LCLICK:
		case MouseEventType::LDOUBLECLICK:
			break;
		default:
			if (isHover || isOnclick)
			{
				for (auto i : allComponents)
					i->setColorStyle(foreColor, backColor);
				nowBackColor = backColor;
				nowForeColor = foreColor;
				isHover = false;
				isOnclick = false;
				setChange();
			}
		}
		mouseEventFun = NULL;
	}
	if (mouseEventFun && mouseEventFun(e, this))
	{
		setChange();
		return this;
	}
	return NULL;
}


/*-------------------Table----------------------------*/

void Custom::Table::setColWidth(const std::vector<unsigned short>& colWidth)
{
	int total = 0;
	for (size_t i = 0; i < colWidth.size(); i++)
	{
		total += colWidth[i];
	}
	if (total > width)
		return;
	this->colWidth = colWidth;
	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->setColWidth(colWidth);
	}
	if(titleItem!=NULL)
		titleItem->setColWidth(colWidth);
}

void Custom::Table::setTableData(csvdb::table::TableData td)
{
	clearItems();
	this->datas.clear();
	if (!tdProcess)
		tdProcess = std::bind(&Custom::Table::defaultProcess, this, std::placeholders::_1);
	this->datas = tdProcess(td);
	this->td = td;
	index = 0;
	setIndex(1);
}

void Custom::Table::clearData()
{
	setTableData(csvdb::table::TableData());
}

std::vector<std::vector<std::wstring>> Custom::Table::defaultProcess(csvdb::table::TableData& td)
{
	std::vector<std::vector<std::wstring>> res;
	for (size_t i = 0; i < td.length(); i++)
	{
		std::vector<std::wstring> tmp;
		for (size_t j = 0; j < td[i].size(); j++)
		{
			tmp.push_back(td[i][j].toString());
		}
		res.push_back(tmp);
	}
	return res;
}

void Custom::Table::update()
{
	if (isChange&&getShow())
	{
		draw();
		isChange = false;
	}
	else
		Div::update();
}

size_t Custom::Table::getCurIndex()
{
	return index;
}

size_t Custom::Table::getMaxIndex()
{
	return (datas.size() / maxRow) + (datas.size() % maxRow ? 1 : 0);
}

csvdb::table::TableData Custom::Table::getTableData()
{
	return td;
}

bool Custom::Table::setIndex(size_t index)
{
	if (index == this->index || index > getMaxIndex())return false;
	clearItems();
	for (size_t i = (index - 1) * maxRow; i < index * maxRow && i < datas.size(); i++)
	{
		TableItem* t = new TableItem(outputHandle, datas[i], 0, 0, width - 2, 1);
		if (!colWidth.empty())
			t->setColWidth(colWidth);
		t->onClickFun = std::bind(&Table::clickItem, this, std::placeholders::_1, std::placeholders::_2);
		items.push_back(t);
		addComponent(t);
		mouseComponents.push_back(t);
	}
	this->index = index;
	setChange();
	return true;
}

View::ComponentBase* Custom::Table::onMouseEvent(MouseEvent& e)
{
	return Div::onMouseEvent(e);
}

int Custom::Table::clickItem(View::MouseEvent& e, View::ComponentBase* com)
{
	TableItem* item = (TableItem*)com;
	if (onClickItem)
	{
		std::vector<TableItem*>::iterator iter = std::find(items.begin(), items.end(), item);
		if (iter != items.end())
		{
			size_t pageIndex = distance(items.begin(), iter),tdIndex =  (getCurIndex() - 1) * maxRow + pageIndex;
			onClickItem(e, **iter,td[tdIndex], pageIndex, tdIndex);
			return 0;
		}
	}
	return 1;
}

void Custom::Table::clearItems()
{
	for (auto i : items)
	{
		removeComponent(i);
	}
	items.clear();
}

void Custom::Table::drawBorder()
{
	ComponentBase::drawBorder();
	COORD c = point.getCOORD();
	c.X = point.x + 1,c.Y = titleItem->getPoint().getAbsY(titleItem->getHeight());
	SetConsoleCursorPosition(outputHandle, c);
	for (size_t i = 1; i < width - 1; i++)
		std::wcout << L"-";
}

void Custom::Table::beginDraw()
{
	drawBorder();
	Div::beginDraw();
}

Custom::Table::Table(HANDLE outputHandle, std::vector<std::wstring> colNames, size_t abX, size_t abY, size_t width, size_t height
	, Color defaultForeColor, Color defaultBackColor)
	:Div(outputHandle, abX, abY, width, height, 1, defaultForeColor, defaultBackColor)
{
	setPadding(1, 1, 0, 1);
	maxRow = ((height - 1) / 2) - 1;
	index = 1;
	titleItem = new TableItem(outputHandle, colNames, 0, 0, width - 2, 1);
	addComponent(titleItem);
}


/*-------------------Box----------------------------*/

void Box::beginDraw()
{
	ComponentBase::drawBorder();
	LayoutBase::beginDraw();
}


/*-------------------InputUnit----------------------------*/

Custom::InputUnit::InputUnit(HANDLE outputHandle, std::wstring title, size_t abX, size_t abY, size_t width, size_t height, Color defaultForeColor, Color defaultBackColor)
	:Div(outputHandle, abX, abY, width, height,0,defaultForeColor, defaultBackColor)
{
	this->title = new Text(outputHandle, title, 0, 0, width, 1);
	this->input = new InputText(outputHandle, L"", 0, 0, width, 1);
	Line* line = new Line(outputHandle,90,3,true, width);

	std::vector<ComponentBase*> allCom = { this->title ,this->input ,line };
	registerComponents(allCom, allCom, allCom);
}

void Custom::InputUnit::setTitle(std::wstring title)
{
	this->title->setText(title);
}

std::wstring Custom::InputUnit::getTitle()
{
	return this->title->getText();
}

void Custom::InputUnit::setValue(std::wstring value)
{
	this->input->setText(value);
}

std::wstring Custom::InputUnit::getValue()
{
	return this->input->getText();
}


/*-------------------CommodityEdit----------------------------*/

CommodityEdit::CommodityEdit(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
	, Color defaultForeColor, Color defaultBackColor)
	:Div(outputHandle, abX, abY, width, height, 1, defaultForeColor, defaultBackColor)
{
	commodityIdText = new Text(outputHandle, L"编号：自动生成", 0, 0, width, 1);
	nameInputUnit = new InputUnit(outputHandle, L"名称", 0, 0, width, 3);
	modelInputUnit = new InputUnit(outputHandle, L"型号", 0, 0, width, 3);
	categoryInputUnit = new InputUnit(outputHandle, L"种类", 0, 0, width, 3);
	detailInputUnit = new InputUnit(outputHandle, L"描述", 0, 0, width, 3);
	priceInputUnit = new InputUnit(outputHandle, L"售价", 0, 0, width, 3);
	statusInputUnit = new InputUnit(outputHandle, L"状态", 0, 0, width, 3);
	std::vector<ComponentBase*> all = {
		commodityIdText,nameInputUnit,modelInputUnit,categoryInputUnit,detailInputUnit,
		priceInputUnit,statusInputUnit
	};
	std::vector<ComponentBase*> mouse = {
		nameInputUnit,modelInputUnit,categoryInputUnit,detailInputUnit,
		priceInputUnit,statusInputUnit
	};
	registerComponents(all, mouse, mouse);
}

void CommodityEdit::set(vector<std::wstring> commodity)
{
	for (int i = commodity.size(); i < 8; i++)
			commodity.push_back(L"");
	commodityId = INT_MAX;
	if (commodity[0] != L"")
	{
		try {
			commodityId = stoi(commodity[0]);
			commodityIdText->setText(L"编号：" + commodity[0] + L"\t库存：" + commodity[7]);
		}
		catch (...) {
			commodityIdText->setText(L"编号：自动生成");
		}
	}
	else
		commodityIdText->setText(L"编号：自动生成");
	nameInputUnit->setValue(commodity[1]);
	modelInputUnit->setValue(commodity[2]);
	categoryInputUnit->setValue(commodity[3]);
	detailInputUnit->setValue(commodity[4]);

	priceInputUnit->setValue(commodity[5]);
	statusInputUnit->setValue(commodity[6]);
}

vector<std::wstring> CommodityEdit::getCommodity()
{
	vector<std::wstring> res = {
		commodityId == INT_MAX ? L"" : std::to_wstring(commodityId) ,
		nameInputUnit->getValue(),
		modelInputUnit->getValue(),
		categoryInputUnit->getValue(),
		detailInputUnit->getValue(),
		priceInputUnit->getValue(),
		statusInputUnit->getValue(),
	};
	return res;
}

int CommodityEdit::getCommodityId()
{
	return commodityId;
}


/*-------------------ShoppingCartItem----------------------------*/

Custom::ShoppingCartItem::ShoppingCartItem(HANDLE outputHandle, int commodityId, std::wstring name, double price, size_t abX, size_t abY, size_t width, size_t height, Color defaultForeColor, Color defaultBackColor)
	:Span(outputHandle, abX, abY, width, height, 1, defaultForeColor, defaultBackColor), price(price)
{
	Text* nameText = new Text(outputHandle, name, 0, 0, 11, height);
	countText = new Text(outputHandle, (L"x" + std::to_wstring(count)), 0, 0, 3, height);
	priceText = new Text(outputHandle,Shop::price2wstr(price), 0, 0, 9, height);
	std::vector<ComponentBase*> allCom = { nameText,countText,priceText };
	registerComponents(allCom, allCom, allCom);
	this->commodityId = commodityId;
}

int Custom::ShoppingCartItem::getCommodityId()
{
	return commodityId;
}

int Custom::ShoppingCartItem::addItemCount(View::MouseEvent& e, View::ComponentBase* com)
{
	addCount();
	return 0;
}

int Custom::ShoppingCartItem::subItemCount(View::MouseEvent& e, View::ComponentBase* com)
{
	subCount();
	return 0;
}

void Custom::ShoppingCartItem::addCount()
{
	count++;
	countText->setText((L"x" + std::to_wstring(count)));
}

void Custom::ShoppingCartItem::subCount()
{
	count--;
	countText->setText((L"x" + std::to_wstring(count)));
}

int Custom::ShoppingCartItem::getCount()
{
	return count;
}

double Custom::ShoppingCartItem::getTotalPrice()
{
	return price*count;
}

/*-------------------ShoppingCart----------------------------*/

Custom::ShoppingCart::ShoppingCart(HANDLE outputHandle, size_t abX, size_t abY, 
	size_t width, size_t height, Color defaultForeColor, Color defaultBackColor)
	:Div(outputHandle, abX, abY, width, height, 0, defaultForeColor, defaultBackColor)
{
	itemDiv = new Div(outputHandle, abX, abY, width, height - 3, 0, defaultForeColor, defaultBackColor);
	showDiv = new Div(outputHandle, abX, abY, width, 3, 0, defaultForeColor, defaultBackColor);
	Span* tmp = new Span(outputHandle, abX, abY, width, 1, 0, defaultForeColor, defaultBackColor);
	totalPrice = new Text(outputHandle, L"总价：" + Shop::price2wstr(0), 0, 0, width, 2);
	Text* text = new Text(outputHandle, L"优惠号：", 0, 0, 9, 1);
	discountId = new InputText(outputHandle, L"请输入优惠号...", 0, 0, 15, 1);

	tmp->addComponent(text);

	std::vector<ComponentBase*> tmpAllCom = {
		discountId
	};
	std::vector<ComponentBase*> tmpEventCom = {
		discountId
	};
	tmp->registerComponents(tmpAllCom, tmpEventCom, tmpEventCom);

	std::vector<ComponentBase*> showDivAllCom = {
		totalPrice ,tmp
	};
	std::vector<ComponentBase*> showDivEventCom = {
		tmp
	};
	showDiv->registerComponents(showDivAllCom, showDivEventCom, showDivEventCom);

	std::vector<ComponentBase*> allCom = {
		itemDiv ,showDiv
	};
	std::vector<ComponentBase*> eventCom = {
		showDiv
	};
	registerComponents(allCom, eventCom, eventCom);
}

void Custom::ShoppingCart::addShop(int commodityId, std::wstring name, double price)
{
	std::map<int, ShoppingCartItem*>::iterator iter = items.find(commodityId);
	if (iter != items.end())
	{
		(iter->second)->addCount();
		totalPrice->setText(L"总价：" + Shop::price2wstr(getTotalPrice()));
		return;
	}
	ShoppingCartItem* item = new ShoppingCartItem(outputHandle, commodityId, name, price, 0, 0, width, 2);
	items.insert({ commodityId, item });
	itemDiv->addComponent(item);
	totalPrice->setText(L"总价：" + Shop::price2wstr(getTotalPrice()));
	return;
}

void Custom::ShoppingCart::delShop(int commodityId)
{
	std::map<int, ShoppingCartItem*>::iterator iter = items.find(commodityId);
	if (iter != items.begin())
	{
		itemDiv->removeComponent((iter->second));
		totalPrice->setText(L"总价：" + Shop::price2wstr(getTotalPrice()));
		return;
	}
}

double Custom::ShoppingCart::getTotalPrice()
{
	double price = 0;
	for (auto i : items)
	{
		price += i.second->getTotalPrice();
	}
	return price;
}

void Custom::ShoppingCart::clearShop()
{
	itemDiv->clear();
	items.clear();
	discountId->setText(L"请输入优惠号...");
	totalPrice->setText(L"总价：" + Shop::price2wstr(0));
}

int Custom::ShoppingCart::getDiscountId()
{
	try
	{
		if (discountId == NULL || discountId->getText() == L"请输入优惠号...")
			return INT_MAX;
		else
			return stoi(discountId->getText());
	}
	catch (...) 
	{
		return INT_MAX;
	}

}

vector<vector<CSVData>> Custom::ShoppingCart::getShop()
{
	vector < vector<CSVData>> shop;
	for (auto i : items)
	{
		shop.push_back({
			CSVData(i.first),
			CSVData(i.second->getTotalPrice()),
			CSVData(i.second->getCount())
			});
	}
	return shop;
}

/*-------------------ShoppingCart----------------------------*/

SaleDetailItem::SaleDetailItem(HANDLE outputHandle, vector<std::wstring> data, size_t abX, size_t abY, size_t width, size_t height
	, Color defaultForeColor, Color defaultBackColor)
	:Span(outputHandle, abX, abY, width, height, 1, defaultForeColor, defaultBackColor)
{
	commodityIdText = new Text(outputHandle, L"", 0, 0, 4, height);
	nameText = new Text(outputHandle, L"", 0, 0, 8, height);
	priceText = new Text(outputHandle, L"", 0, 0, 8, height);
	quantityText = new Text(outputHandle, L"", 0, 0, 3, height);
	std::vector<ComponentBase*> allCom = { commodityIdText,nameText,priceText,quantityText };
	registerComponents(allCom, allCom, allCom);
	set(data);
}

void SaleDetailItem::set(vector<std::wstring> data)
{
	for (int i = data.size(); i < 4; i++)
		data.push_back(L"");
	if (data[0] != L"")
		commodityIdText->setText(data[0]);
	else
		commodityIdText->setText(L"无");
	if (data[1] != L"")
		nameText->setText(data[1]);
	else
		nameText->setText(L"无");
	if (data[2] != L"")
		priceText->setText(data[2]);
	else
		priceText->setText(L"无");
	if (data[3] != L"")
		quantityText->setText(L"x" + data[3]);
	else
		quantityText->setText(L"无");
}


/*-------------------SaleDetail----------------------------*/

void SaleDetail::set(int saleId, std::wstring price, vector<vector<std::wstring>> datas)
{
	clear();
	addComponent(new Text(outputHandle, L"订单编号：" + to_wstring(saleId), 0, 0, width, 2));
	addComponent(new Text(outputHandle, L"订单总价：" + price, 0, 0, width, 2));
	addComponent(new Text(outputHandle, L"编号   名称    价格   数量", 0, 0, width, 2));
	if(datas.size()==0)
		addComponent(new Text(outputHandle, L"真奇怪，没有数据呢~", 0, 0, width, 2));
	else
		for (auto i : datas)
		{
			addComponent(new SaleDetailItem(outputHandle, i, 0, 0, width, 2));
		}
}


/*-------------------PurchaseInfo-------------------*/

void PurchaseInfo::set(vector<std::wstring> purchase)
{
	clear();
	for (int i = purchase.size(); i < 6; i++)
		purchase.push_back(L"");
	std::vector<ComponentBase*> allCom = {
			new Text(outputHandle, L"进货号：" + purchase[0], 0, 0, width, 2),
			new Text(outputHandle, L"商品号：" + purchase[1], 0, 0, width, 2),
			new Text(outputHandle, L"商品名称：" + purchase[2], 0, 0, width, 2),
			new Text(outputHandle, L"进价：" + purchase[3], 0, 0, width, 2),
			new Text(outputHandle, L"进货时间：" + purchase[4], 0, 0, width, 2),
			new Text(outputHandle, L"进货渠道：" + purchase[5], 0, 0, width, 2),
			new Text(outputHandle, L"进货数量：" + purchase[6], 0, 0, width, 2)
	}, nullCom;
	registerComponents(allCom, nullCom, nullCom);
}

void Custom::PurchaseInfo::clearInput()
{
	commodityIdUnit->setValue(L"");
	quantityUnit->setValue(L"");
	priceUnit->setValue(L"");
	timeUnit->setValue(L"");
	channalUnit->setValue(L"");
}

void PurchaseInfo::set()
{
	clear();
	std::vector<ComponentBase*> allCom = {
			new Text(outputHandle, L"进货号：自动生成", 0, 0, width, 1),
			commodityIdUnit = new InputUnit(outputHandle, L"商品号", 0, 0, width, 3),
			priceUnit = new InputUnit(outputHandle, L"进价", 0, 0, width, 3),
			timeUnit = new InputUnit(outputHandle, L"进货时间", 0, 0, width, 3),
			channalUnit = new InputUnit(outputHandle, L"进货渠道", 0, 0, width, 3),
			quantityUnit = new InputUnit(outputHandle, L"进货数量", 0, 0, width, 3)
	};
	registerComponents(allCom, allCom, allCom);
}

vector<std::wstring> PurchaseInfo::getPurchase()
{
	return {
		commodityIdUnit->getValue(),
		priceUnit->getValue(),
		timeUnit->getValue(),
		channalUnit->getValue(),
		quantityUnit->getValue(),
	};
}

/*-------------------DiscountInfo-------------------*/

DiscountInfo::DiscountInfo(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
	, Color defaultForeColor, Color defaultBackColor)
	: Div(outputHandle, abX, abY, width, height, 1, defaultForeColor, defaultBackColor) 
{
	std::vector<ComponentBase*> allCom = {
		texts[0] = new Text(outputHandle, L"", 0, 0, width, 2),
		texts[1] = new Text(outputHandle, L"", 0, 0, width, 2),
		texts[2] = new Text(outputHandle, L"", 0, 0, width, 2),
		texts[3] = new Text(outputHandle, L"", 0, 0, width, 2),
		texts[4] = new Text(outputHandle, L"", 0, 0, width, 2)
	},nullCom;
	registerComponents(allCom, nullCom, nullCom);
}

void DiscountInfo::set(vector<std::wstring> discount)
{
	try {
		id = std::stoi(discount[0]);
	}
	catch(...) {
		id = -1;
	}
	for (int i = discount.size(); i < 5; i++)
		discount.push_back(L"");
	for (size_t i = 0; i < 5; i++)
	{
		texts[i]->setText(contents[i] + discount[i]);
	}
}

int DiscountInfo::getId()
{
	return id;
}


/*-------------------AdvList-------------------*/
AdvList::AdvList(HANDLE outputHandle, std::wstring title, size_t abX, size_t abY, size_t width, size_t height
	, Color defaultForeColor, Color defaultBackColor)
	:Table(outputHandle, std::vector<std::wstring>{title},abX,abY, width, height, defaultForeColor,defaultBackColor)
{
	setPadding(0, 1, 0, 1);
}

void Custom::AdvList::drawBorder()
{
	COORD c = point.getCOORD();
	c.X = point.x, c.Y = titleItem->getPoint().getAbsY(titleItem->getHeight());
	SetConsoleCursorPosition(outputHandle, c);
	for (size_t i = 0; i < width; i++)
		std::wcout << L"-";
}

std::wstring Custom::AdvList::setText(int index, std::wstring text)
{
	if (index > items.size())
		return L"";

	return items[index]->setText(0, text);
}

void Custom::AdvList::setColWidth(const std::vector<unsigned short>& colWidth)
{
	TableItem* tmp = titleItem;
	titleItem = NULL;
	Table::setColWidth(colWidth);
	titleItem = tmp;
}

/*-------------------CommodityList-------------------*/

CommodityList::CommodityList(HANDLE outputHandle, std::wstring title, size_t abX, size_t abY, size_t width, size_t height
	, Color defaultForeColor, Color defaultBackColor)
	:AdvList(outputHandle, title, abX, abY, width, height, defaultForeColor, defaultBackColor)
{}

void CommodityList::setTableData(csvdb::table::TableData datas)
{
	int num = datas.getHeader().getFields().size();
	if (datas.length() == 0 && num == 0)
	{
		td.clear();
		AdvList::setTableData(td);
		return;
	}
	else if (num < 3)
		return;
	AdvList::setTableData(datas);
}

void CommodityList::addCommodity(int commodityId, std::wstring name, double price)
{
	if (td[td[L"id"] == commodityId].length() != 0)
		return;
	td.push({ CSVData(commodityId),CSVData(name,DATATYPE::STR),CSVData(price)});
	setTableData(td);
}

void CommodityList::delCommodity(int commodityId)
{
	td.get(td[L"id"] != commodityId);
	setTableData(td);
}

/*-------------------DiscountEdit-------------------*/

DiscountEdit::DiscountEdit(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
	, Color defaultForeColor, Color defaultBackColor)
	: Div(outputHandle, abX, abY, width, height, 2, defaultForeColor, defaultBackColor)
{
	std::vector<ComponentBase*> allCom = {
		texts[0] = new InputUnit(outputHandle, L"优惠号", 0, 0, width, 3),
		texts[1] = new InputUnit(outputHandle, L"最抵消", 0, 0, width, 3),
		texts[2] = new InputUnit(outputHandle, L"优惠价", 0, 0, width, 3),
		texts[3] = new InputUnit(outputHandle, L"选中商品必选", 0, 0, width, 3),
		texts[4] = new InputUnit(outputHandle, L"优惠详情", 0, 0, width, 3)
	}, nullCom;
	registerComponents(allCom, allCom, nullCom);
}

void DiscountEdit::set(vector<std::wstring> discount)
{
	try {
		id = std::stoi(discount[0]);
	}
	catch (...) {
		id = -1;
	}
	std::vector<ComponentBase*>::iterator iter = std::find(mouseComponents.begin(), mouseComponents.end(), texts[0]);
	if (iter == mouseComponents.end())
		mouseComponents.push_back(texts[0]);
	for (int i = discount.size(); i < 5; i++)
		discount.push_back(L"");
	discount[1] = discount[1].substr(1);
	discount[2] = discount[2].substr(1);
	for (size_t i = 0; i < 5; i++)
	{
		texts[i]->setValue(discount[i]);
	}
}

void DiscountEdit::set()
{
	texts[0]->setValue(L"自动生成");
	id = -1;
	std::vector<ComponentBase*>::iterator iter = std::find(mouseComponents.begin(), mouseComponents.end(), texts[0]);
	if (iter != mouseComponents.end())
		mouseComponents.erase(iter);
	for (size_t i = 1; i < 5; i++)
	{
		texts[i]->setValue(L"");
	}
}

vector<std::wstring> DiscountEdit::getDiscount()
{
	vector<std::wstring> res;
	if(texts[0] == NULL || texts[0]->getValue() == L"自动生成")
		res.push_back(L"");
	else
		res.push_back(texts[0]->getValue());
	for (size_t i = 1; i < 5; i++)
	{
		if (texts[i] != NULL)
			res.push_back(texts[i]->getValue());
		else
			res.push_back(L"");
	}
	return res;
}

