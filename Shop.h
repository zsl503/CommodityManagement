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
	csvdb::table::TableData getShelves();	// ��ȡ�����б�
	csvdb::table::TableData getCommodities(bool isShort = false);	// ��ȡ��Ʒ�б�
	csvdb::table::TableData getSale();	// ��ȡ����
	csvdb::table::TableData getPurchase();	// ��ȡ������Ϣ
	csvdb::table::TableData getDiscount();	// ��ȡ�ۿ���Ϣ
	csvdb::table::TableData getDiscountIds();
	csvdb::table::TableData getStatistics();

	csvdb::table::TableData searchShelves(std::wstring);	// ����������Ʒ
	csvdb::table::TableData searchCommodities(std::wstring,bool isShort = false);	// ������Ʒ
	csvdb::table::TableData searchSale(std::wstring);	// ��������
	csvdb::table::TableData searchPurchase(std::wstring);	// ����������
	csvdb::table::TableData searchDiscount(std::wstring);	// �����Ż�
	csvdb::table::TableData searchStatistics(std::wstring);	// ����ͳ������

	csvdb::table::TableData getDiscountDetail(int discountId);	// ��ȡ�ۿ�ϸ��
	std::vector<std::wstring> getCommodity(int commodityId);	// ��ȡ��Ʒ����
	std::vector<std::vector<std::wstring>> getSaleDetail(int commodityId);	// ��ȡ��������
	std::vector<std::wstring> getPurchaseDetail(int purchaseId);	// ��ȡ��������
	std::vector<std::wstring> getDiscountInfo(int discountId, bool edit = false);	// ��ȡ�Żݻ�����Ϣ
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