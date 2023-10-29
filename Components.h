#pragma once
#include <iostream>
#include <Windows.h>
#include <ComponentBase.h>
#include <csvdb.h>
#include <map>

namespace Custom
{
	using namespace View;
	class TopButton :public Button
	{
	public:
		TopButton(HANDLE outputHandle, std::wstring text, size_t abX, size_t abY, size_t width = 10, size_t height = 1
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			:Button(outputHandle, text, abX, abY, width, height, defaultForeColor, defaultBackColor)
		{
			textAbX = 1, textAbY = 0;
			textSubHeight = 0, textSubWidth = 2;
		}
		TopButton(HANDLE outputHandle, std::wstring text, Point point, size_t width = 10, size_t height = 1
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: TopButton(outputHandle, text, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}
	protected:

		void drawBorder();
	};

	class LeftButton :public Button
	{
	public:
		LeftButton(HANDLE outputHandle, std::wstring text, size_t abX, size_t abY, size_t width = 10, size_t height = 1
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);

		LeftButton(HANDLE outputHandle, std::wstring text, Point point, size_t width = 10, size_t height = 1
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: LeftButton(outputHandle, text, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}
	protected:

		void drawBorder();
	};

	class TableItem :public Span
	{
	public:
		TableItem(HANDLE outputHandle, std::vector<std::wstring> rowData,size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);

		TableItem(HANDLE outputHandle, std::vector<std::wstring> rowData, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: TableItem(outputHandle, rowData, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}
		void setRowData(std::vector<std::wstring> rowData);
		void setColWidth(const std::vector<unsigned short>& colWidth);

		ComponentBase* onMouseEvent(MouseEvent&);
		void setColorStyle(Color, Color);
		void setHoverStyle(Color, Color);
		void setClickStyle(Color, Color);
		std::wstring getText(int);
		std::wstring setText(int,std::wstring);
		std::function<int(MouseEvent&, ComponentBase*)> onClickFun;
	private:
		Color hoverBackColor;
		Color hoverForeColor = Color::LIGHTYELLOW;
		Color clickBackColor = Color::WHITE;
		Color clickForeColor = Color::LIGHTRED;
		void beginDraw();
		void drawBorder();
		std::vector<View::Text*> texts;
		bool isHover = false;
		bool isOnclick = false;
	};

	class Table :public Div
	{
	public:
		Table(HANDLE outputHandle, std::vector<std::wstring> colNames, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);

		Table(HANDLE outputHandle,std::vector<std::wstring> colNames,Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: Table(outputHandle, colNames, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}
		virtual void setColWidth(const std::vector<unsigned short>& colWidth);
		virtual void setTableData(csvdb::table::TableData);
		void clearData();
		void update();
		size_t getCurIndex();
		size_t getMaxIndex();
		csvdb::table::TableData getTableData();
		bool setIndex(size_t);
		View::ComponentBase* onMouseEvent(MouseEvent& e);
		std::function<int(MouseEvent&, TableItem&, std::vector<csvop::CSVData>&, size_t, size_t)> onClickItem = NULL;
		std::function<std::vector<std::vector<std::wstring>>(csvdb::table::TableData&)> tdProcess = NULL;
	protected:
		void clearItems();
		void drawBorder();
		void beginDraw();
		TableItem* titleItem = NULL;
		std::vector<TableItem*> items;
		csvdb::table::TableData td;
	private:
		std::vector<unsigned short> colWidth;
		std::vector<std::vector<std::wstring>> defaultProcess(csvdb::table::TableData& td);
		int clickItem(View::MouseEvent& e, View::ComponentBase* com);
		size_t maxRow;
		size_t index;
		std::vector<std::vector<std::wstring>> datas;
	};

	class Box :public LayoutBase
	{
	public:
		Box(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			:LayoutBase(outputHandle, abX, abY, width, height, defaultForeColor, defaultBackColor){}
		Box(HANDLE outputHandle, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: Box(outputHandle, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

	protected:
		void beginDraw();

	private:
		std::vector<std::wstring> colNames;
	};

	class InputUnit :public Div
	{
	public:
		InputUnit(HANDLE outputHandle, std::wstring title, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);
		InputUnit(HANDLE outputHandle, std::wstring title, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: InputUnit(outputHandle, title, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

		void setTitle(std::wstring);

		std::wstring getTitle();

		void setValue(std::wstring);

		std::wstring getValue();

	private:
		View::Text* title = NULL;
		View::InputText* input = NULL;
	};

	class CommodityEdit :public Div
	{
	public:
		CommodityEdit(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);
		
		CommodityEdit(HANDLE outputHandle, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: CommodityEdit(outputHandle, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

		void set(vector<std::wstring> commodity);

		vector<std::wstring> getCommodity();

		int getCommodityId();

	private:
		int commodityId = INT_MAX;
		Text* commodityIdText = NULL;
		InputUnit *nameInputUnit = NULL, *modelInputUnit = NULL, *categoryInputUnit = NULL, *detailInputUnit = NULL, *priceInputUnit = NULL, *statusInputUnit = NULL;
	};

	class ShoppingCartItem :public Span
	{
	public:
		ShoppingCartItem(HANDLE outputHandle,int commodityId, std::wstring name, double price, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);
		
		ShoppingCartItem(HANDLE outputHandle, int commodityId, std::wstring name, double price, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: ShoppingCartItem(outputHandle, commodityId, name, price, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

		void addCount();

		void subCount();

		int getCount();

		double getTotalPrice();

		int getCommodityId();

	private:
		int addItemCount(View::MouseEvent& e, View::ComponentBase* com);
		int subItemCount(View::MouseEvent& e, View::ComponentBase* com);
		std::wstring name;
		double price;
		int commodityId;
		size_t count = 1;
		Text* countText = NULL;
		Text* priceText = NULL;
	};

	class ShoppingCart :public Div
	{
	public:
		ShoppingCart(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);
		
		ShoppingCart(HANDLE outputHandle, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: ShoppingCart(outputHandle,point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

		void addShop(int commodityId, std::wstring name,double price);

		void delShop(int commodityId);

		void clearShop();

		double getTotalPrice();

		int getDiscountId();

		vector < vector<csvop::CSVData>> getShop();

	private:
		std::map<int, ShoppingCartItem*> items;
		Text* totalPrice = NULL;
		InputText* discountId = NULL;
		Div* itemDiv = NULL;
		Div* showDiv = NULL;
	};

	class SaleDetailItem :public Span
	{
	public:
		SaleDetailItem(HANDLE outputHandle, vector<std::wstring> data, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);

		SaleDetailItem(HANDLE outputHandle, vector<std::wstring> data, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: SaleDetailItem(outputHandle, data, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

		void set(vector<std::wstring> commodity);
	private:
		Text *commodityIdText = NULL, *nameText = NULL, *priceText = NULL, *quantityText = NULL;
	};

	class SaleDetail :public Div
	{
	public:
		SaleDetail(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
			,Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			:Div(outputHandle, abX, abY, width, height, 0, defaultForeColor, defaultBackColor){}

		SaleDetail(HANDLE outputHandle, Point point, size_t width, size_t height
			,Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: SaleDetail(outputHandle, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

		void set(int saleId, std::wstring price, vector<vector<std::wstring>> datas);
	
	private:
		Text* saleIdText = NULL;
	};

	class PurchaseInfo :public Div
	{
	public:
		PurchaseInfo(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
		: Div(outputHandle, abX, abY, width, height, 1, defaultForeColor, defaultBackColor) {}

		PurchaseInfo(HANDLE outputHandle, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: PurchaseInfo(outputHandle, point.x, point.y, width, height, defaultForeColor, defaultBackColor){}

		void set(vector<std::wstring> purchase);

		void set();

		void clearInput();

		vector<std::wstring> getPurchase();

	private:
		InputUnit* commodityIdUnit = NULL, *quantityUnit = NULL,
			* priceUnit = NULL, * timeUnit = NULL, * channalUnit = NULL;
	};

	class DiscountInfo :public Div
	{
	public:
		DiscountInfo(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);

		DiscountInfo(HANDLE outputHandle, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: DiscountInfo(outputHandle, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

		void set(vector<std::wstring> discount);

		int getId();

	private:
		Text* texts[5]={NULL,NULL,NULL,NULL,NULL};
		std::vector<std::wstring> contents = {
			L"优惠号：",L"最抵消：",L"优惠价：",L"优惠类型：",L"优惠详情："
		};
		int id;
	};

	class AdvList :public Table
	{
	public:
		AdvList(HANDLE outputHandle, std::wstring, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);

		std::wstring setText(int, std::wstring);

		void setColWidth(const std::vector<unsigned short>& colWidth);
	protected:
		void drawBorder();
	};

	class CommodityList :public AdvList
	{
	public:
		CommodityList(HANDLE outputHandle, std::wstring, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);

		void setTableData(csvdb::table::TableData);

		void addCommodity(int commodityId, std::wstring name, double price);

		void delCommodity(int commodityId);

	};

	class DiscountEdit :public Div
	{
	public:
		DiscountEdit(HANDLE outputHandle, size_t abX, size_t abY, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK);

		DiscountEdit(HANDLE outputHandle, Point point, size_t width, size_t height
			, Color defaultForeColor = Color::WHITE, Color defaultBackColor = Color::BLACK)
			: DiscountEdit(outputHandle, point.x, point.y, width, height, defaultForeColor, defaultBackColor) {}

		void set(vector<std::wstring> discount);

		void set();

		vector<std::wstring> getDiscount();

	private:
		InputUnit* texts[5] = { NULL,NULL,NULL,NULL,NULL };
		int id;
	};
}
