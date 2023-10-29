#pragma once
#include <string>
#include <csvdb.h>
#include <map>

class Shop
{
public:
	Shop(csvdb::Database db);
	bool login(int ac, std::wstring pw);
	bool isLogin();
	bool signUp(int ac, std::wstring pw);
	csvdb::table::TableData getShelves();	// 获取在售列表
	csvdb::table::TableData getCommodities(bool isShort = false);	// 获取商品列表
	csvdb::table::TableData getSale();	// 获取订单
	csvdb::table::TableData getPurchase();	// 获取订货信息
	csvdb::table::TableData getDiscount();	// 获取折扣信息
	csvdb::table::TableData getDiscountIds();
	csvdb::table::TableData getStatistics();

	csvdb::table::TableData searchShelves(std::wstring);	// 搜索在售商品
	csvdb::table::TableData searchCommodities(std::wstring,bool isShort = false);	// 搜索商品
	csvdb::table::TableData searchSale(std::wstring);	// 搜索订单
	csvdb::table::TableData searchPurchase(std::wstring);	// 搜索进货单
	csvdb::table::TableData searchDiscount(std::wstring);	// 搜索优惠
	csvdb::table::TableData searchStatistics(std::wstring);	// 搜索统计数据

	csvdb::table::TableData getDiscountDetail(int discountId);	// 获取折扣细节
	std::vector<std::wstring> getCommodity(int commodityId);	// 获取商品详情
	std::vector<std::vector<std::wstring>> getSaleDetail(int commodityId);	// 获取订单详情
	std::vector<std::wstring> getPurchaseDetail(int purchaseId);	// 获取进货详情
	std::vector<std::wstring> getDiscountInfo(int discountId, bool edit = false);	// 获取优惠基础信息
	bool order();
	int useDiscount(const vector<vector<csvop::CSVData>>& datas, int discountId, double totalPrice);

	static std::vector<std::vector<std::wstring>> shelfTdProcess(csvdb::table::TableData& td);
	static std::vector<std::vector<std::wstring>> commodityTdProcess(csvdb::table::TableData& td);
	static std::vector<std::vector<std::wstring>> saleTdProcess(csvdb::table::TableData& td);
	static std::vector<std::vector<std::wstring>> purchaseTdProcess(csvdb::table::TableData& td);
	static std::vector<std::vector<std::wstring>> discountTdProcess(csvdb::table::TableData& td);
	static std::vector<std::vector<std::wstring>> commodityListTdProcess(csvdb::table::TableData& td);

	bool saveSales(const vector<vector<csvop::CSVData>>& datas,double,int=INT_MAX);
	bool saveCommodity(const std::vector<std::wstring>&);
	int savePurchase(const std::vector<std::wstring>&);
	int saveDiscount(const std::vector<std::wstring>&,const csvdb::table::TableData&);

	bool removeCommodity(int id);
	bool removeDiscount(int id);

	static std::wstring price2wstr(double price);
	static std::wstring price2wstr(std::wstring price);
private:
	bool loginStatus;
	int account;
	std::wstring shopName;
	csvdb::Database db;
	int shopId = 201901;
};