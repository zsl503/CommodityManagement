#include "Shop.h"
#include <map>
using namespace csvdb;
using namespace csvop;
using csvdb::table::FilterGetter;
using csvdb::table::Field;
using csvdb::table::TableData;
using csvdb::table::TableHeader;

Shop::Shop(Database db):db(db){}

bool Shop::login(int ac, std::wstring pw)
{
	try
	{
		TableData td = db[L"shop"];
		shopId = ac;
		return (loginStatus = (td[td[L"id"] == ac && td[L"password"] == pw].length() != 0));
	}
	catch (csvdb::CSVDbException e)
	{
		return (loginStatus = false);
	}
}

bool Shop::isLogin()
{
	return loginStatus;
}

bool Shop::signUp(int ac, std::wstring pw)
{
	try
	{
		TableData td = db[L"shop"];
		if (pw.size() < 8 || td[td[L"id"] == ac].length() != 0)
			return false;

		time_t timep;
		time(&timep);
		return db.insert(L"shop", { L"id",L"password",L"createTime" },
			{
				ac,CSVData(pw,DATATYPE::STR),CSVData(timep,DATATYPE::DATE)
			});
	}
	catch (CSVDbException e)
	{
		return (loginStatus = false);
	}
}

std::wstring Shop::price2wstr(double price)
{
	return price2wstr(std::to_wstring(price));
}

std::wstring Shop::price2wstr(std::wstring price)
{
	int pos = price.find(L'.');
	if (pos == -1)
	{
		price = price + L".00";
	}
	else if (price.length() - pos == 1)
	{
		price = price + L"0";
	}
	else if (price.length() - pos > 2)
	{
		price = price.substr(0, pos + 3);
	}
	return L"￥" + price;
}

TableData Shop::getShelves()
{
	TableData td = db[L"commodity"]; 
	// 取表commodity
	td = td[td[L"shopId"] == shopId && td[L"status"] == true && td[L"delete"] == false];
	// 筛选字段 "shopId"值为shopId 并且字段 "status"值为true 并且字段 "delete"值为false的数据
	td = td[td[L"id"] && td[L"name"] && td[L"model"] && td[L"category"] && td[L"detail"] && td[L"price"]];
	// 筛选字段 "id","name","model","category","detail","price"列的数据
	return td;
}

TableData Shop::getCommodities(bool isShort)
{
	TableData td = db[L"commodity"];
	td = td[td[L"shopId"] == shopId && td[L"delete"] == false];
	if (!isShort)
	{
		td = td[td[L"id"] && td[L"name"] && td[L"model"] && td[L"category"] && td[L"price"] && td[L"status"] && td[L"stock"]];
	}
	else
	{
		td = td[td[L"id"] && td[L"name"] && td[L"category"] && td[L"price"]];
	}
	return td;

}

TableData Shop::getSale()
{
	TableData td = db[L"sale"];
	td = td[td[L"shopId"] == shopId];
	td = td[td[L"id"] && td[L"totalPrice"] && td[L"time"] && td[L"discountId"]];	
	return td;
}

TableData Shop::getPurchase()
{
	TableData td = db[L"purchase"];
	td = td[td[L"shopId"] == shopId];
	td = td[td[L"id"] && td[L"commodityId"] && td[L"price"] && td[L"time"] && td[L"channel"] && td[L"quantity"]];
	return td;
}

TableData Shop::getDiscount()
{
	TableData td = db[L"discount"];
	td = td[td[L"shopId"] == shopId && td[L"delete"] == false];
	td = td[td[L"id"] && td[L"lowPrice"] && td[L"subPrice"] && td[L"isAll"] && td[L"detail"]];
	return td;
}

TableData Shop::getDiscountIds()
{
	TableData td = db[L"discount"];
	td = td[td[L"shopId"] == shopId && td[L"delete"] == false][td[L"id"]];
	return td;
}

TableData Shop::getStatistics()
{
	TableData td = db[L"saleDetail"];
	TableData resTd,td2 = db[L"commodity"],tmp;
	vector<vector<CSVData>> datas;

	// commodityId,pos
	map<int, int> posMap;
	td = td[td[L"shopId"] == shopId][td[L"commodityId"] && td[L"quantity"]];
	td2 = td2[td2[L"id"] && td2[L"name"] && td2[L"model"] && td2[L"category"]];
	for (size_t i = 0; i < td.length(); i++)
	{
		map<int, int>::iterator iter = posMap.find(td[i][0].getInt());
		if (iter == posMap.end())
		{
			tmp = td2[td2[L"id"] == td[i][0].getInt()];
			datas.push_back({ td[i][0],CSVData(),CSVData(),CSVData(),td[i][1] });
			posMap.insert(pair<int, int>(td[i][0].getInt(), datas.size() - 1));
			if (tmp.length() > 0)
			{
				datas.back()[1] = tmp[0][1];
				datas.back()[2] = tmp[0][2];
				datas.back()[3] = tmp[0][3];
			}
		}
		else
		{
			datas[iter->second][4] = datas[iter->second][4] + td[i][1];
		}
	}
	std::vector<Field> fields = td2.getHeader().getFields();
	fields.push_back(Field(L"sales", DATATYPE::INT,false,4));
	return TableData(table::TableHeader(fields), datas);
}

TableData Shop::searchCommodities(std::wstring cond, bool isShort)
{
	TableData td = db[L"commodity"];
	td = td[td[L"shopId"] == shopId && td[L"delete"] == false];
	if (!isShort)
	{
		td = td[td[L"id"] && td[L"name"] && td[L"model"] && td[L"category"] && td[L"price"] && td[L"status"] && td[L"stock"]];
		if (cond == L"在售" || cond == L"下架")
			td = td[td[L"name"] == cond || td[L"category"] == cond || td[L"status"] == (cond == L"在售" ? true : false)];
		else
			td = td[td[L"name"] == cond || td[L"category"] == cond];
	}
	else
	{
		td = td[td[L"id"] && td[L"name"] && td[L"category"] && td[L"price"]];
		try {
			int t = std::stoi(cond);
			td = td[td[L"name"] == cond || td[L"id"] == t || td[L"category"] == cond];
		}
		catch (...)
		{
			td = td[td[L"name"] == cond];
		}
	}
	return td;
}

TableData Shop::searchShelves(std::wstring cond)
{
	try
	{
		TableData td = db[L"commodity"];
		td = td[td[L"shopId"] == shopId && td[L"status"] == true && td[L"delete"] == false];
		td = td[td[L"id"] && td[L"name"] && td[L"model"] && td[L"category"] && td[L"detail"] && td[L"price"]];
		td = td[td[L"name"] == cond || td[L"category"] == cond];
		return td;
	}
	catch (...)
	{
		return TableData();
	}
}

TableData Shop::searchSale(std::wstring cond)
{
	try
	{
		int t = std::stoi(cond);
		TableData td = db[L"sale"];
		td = td[td[L"shopId"] == shopId];
		td = td[td[L"id"] && td[L"totalPrice"] && td[L"discountId"] && td[L"time"]];
		td = td[td[L"id"] == t];
		return td;
	}
	catch (...)
	{
		return TableData();
	}
}

TableData Shop::searchPurchase(std::wstring cond)
{
	try
	{
		int t = std::stoi(cond);
		TableData td = db[L"purchase"];
		td = td[td[L"shopId"] == shopId];
		td = td[td[L"id"] && td[L"commodityId"] && td[L"price"] && td[L"time"] && td[L"channel"] && td[L"quantity"]];
		td = td[td[L"id"] == t || td[L"commodityId"] == t];
		return td;
	}
	catch (...)
	{
		return TableData();
	}
}

TableData Shop::searchDiscount(std::wstring cond)
{
	try
	{
		int t = std::stoi(cond);
		TableData td = db[L"discount"];
		td = td[td[L"shopId"] == shopId && td[L"id"] == t && td[L"delete"] == false];
		td = td[td[L"id"] && td[L"lowPrice"] && td[L"subPrice"] && td[L"isAll"] && td[L"detail"]];
		return td;
	}
	catch (...)
	{
		return TableData();
	}
}

TableData Shop::searchStatistics(std::wstring cond)
{
	int pos = cond.find(L"-");
	if (pos == wstring::npos)
		return TableData();
	wstring res[2] = { cond.substr(0, pos),cond.substr(pos + 1) };

	TableData td = db[L"saleDetail"], resTd, td2 = db[L"commodity"], td3 = db[L"sale"], tmp;
	vector<vector<CSVData>> datas;
	set<int> idSet;
	map<int, int> posMap;

	td3 = td3[td3[L"shopId"] == shopId && td3[L"time"] <= CSVData(res[1],DATATYPE::DATE) && td3[L"time"] >= CSVData(res[0], DATATYPE::DATE)][td3[L"id"]];
	if (td3.length()==0)
		return tmp;
	for (size_t i = 0; i < td3.length(); i++)
	{
		idSet.insert(td3[i][0].getInt());
	}

	td = td[td[L"shopId"] == shopId][td[L"saleId"] && td[L"commodityId"] && td[L"quantity"]];
	td2 = td2[td2[L"id"] && td2[L"name"] && td2[L"model"] && td2[L"category"]];
	for (size_t i = 0; i < td.length(); i++)
	{
		set<int>::iterator setIter = idSet.find(td[i][0].getInt());
		if (setIter == idSet.end())
			continue;

		map<int, int>::iterator iter = posMap.find(td[i][1].getInt());
		if (iter == posMap.end())
		{
			tmp = td2[td2[L"id"] == td[i][1].getInt()];
			datas.push_back({ td[i][1],CSVData(),CSVData(),CSVData(),td[i][2] });
			posMap.insert(pair<int, int>(td[i][1].getInt(), datas.size() - 1));
			if (tmp.length() > 0)
			{
				datas.back()[1] = tmp[0][1];
				datas.back()[2] = tmp[0][2];
				datas.back()[3] = tmp[0][3];
			}
		}
		else
		{
			datas[iter->second][4] = datas[iter->second][4] + td[i][2];
		}
	}
	std::vector<Field> fields = td2.getHeader().getFields();
	fields.push_back(Field(L"sales", DATATYPE::INT, false, 4));
	return TableData(table::TableHeader(fields), datas);
}

std::vector<std::wstring> Shop::getCommodity(int commodityId)
{
	TableData td = db[L"commodity"];
	td = td[td[L"shopId"] == shopId && td[L"delete"] == false && td[L"id"] == commodityId];
	td = td[td[L"id"] && td[L"name"] && td[L"model"] && td[L"category"] && td[L"detail"] && td[L"price"] && td[L"status"] && td[L"stock"]];
	if (td.length() == 0 || td[0].size() < 7)
		return std::vector<std::wstring>();
	else
	{
		return std::vector<std::wstring>{
			td[0][0].toString(), td[0][1].toString(), td[0][2].toString(), td[0][3].toString(),
			td[0][4].toString(), price2wstr(td[0][5].toString()).substr(1), td[0][6].toString(),
			td[0][7].toString()
		};
	}
}

std::vector<std::vector<std::wstring>> Shop::getSaleDetail(int saleId)
{
	TableData td = db[L"saleDetail"],td2 = db[L"commodity"],tmpTd;
	td = td[td[L"saleId"] == saleId];
	td = td[td[L"commodityId"] && td[L"price"] && td[L"quantity"]];

	td2 = td2[td2[L"shopId"] == shopId][td2[L"id"] && td2[L"name"]];
	int id;
	std::wstring name;
	std::vector<std::vector<std::wstring>> res;
	for (size_t i = 0; i < td.length(); i++)
	{
		tmpTd = td2[td2[L"id"] == td[i][0].getInt()];
		if (tmpTd.length() == 0)
			continue;
		std::vector<std::wstring> line = {
			td[i][0].toString(),tmpTd[0][1].getStr(),
			price2wstr(td[i][1].getDouble()),td[i][2].toString()
		};
		res.push_back(line);
	}
	return res;
}

TableData Shop::getDiscountDetail(int discountId)
{
	TableData td = db[L"discountDetail"], td2 = db[L"commodity"], tmpTd;
	td = td[td[L"discountId"] == discountId][td[L"commodityId"]];

	td2 = td2[td2[L"shopId"] == shopId][td2[L"id"] && td2[L"name"] && td2[L"price"]];
	int id;
	std::wstring name;
	std::vector<std::vector<CSVData>> datas;
	for (size_t i = 0; i < td.length(); i++)
	{
		tmpTd = td2[td2[L"id"] == td[i][0].getInt()];
		if (tmpTd.length() == 0)
			continue;
		datas.push_back({
			tmpTd[0][0],tmpTd[0][1],tmpTd[0][2]
			});
	}
	return TableData(td2.getHeader(), datas);
}

std::vector<std::wstring> Shop::getPurchaseDetail(int purchaseId)
{
	TableData td = db[L"purchase"], td2 = db[L"commodity"], tmpTd;
	td = td[td[L"shopId"] == shopId && td[L"id"] == purchaseId];
	td = td[td[L"id"] && td[L"commodityId"] && td[L"price"] && td[L"time"] && td[L"channel"] && td[L"quantity"]];
	
	std::vector<std::wstring> res;

	if (td.length() == 0)
		return res;

	td2 = td2[td2[L"shopId"] == shopId && td2[L"id"] == td[0][1].getInt()][td2[L"name"]];
	std::wstring name = td2.length() ? td2[0][0].getStr() : L"";
	return std::vector<std::wstring>{
			td[0][0].toString(), td[0][1].toString(), name,
			price2wstr(td[0][2].getDouble()),td[0][3].toString(), td[0][4].toString(),
			td[0][5].toString()
	};
}

std::vector<std::wstring> Shop::getDiscountInfo(int discountId,bool edit)
{
	TableData td = db[L"discount"];
	td = td[td[L"shopId"] == shopId && td[L"id"] == discountId];
	td = td[td[L"id"] && td[L"lowPrice"] && td[L"subPrice"] && td[L"isAll"] && td[L"detail"]];

	std::vector<std::wstring> res;
	if (td.length() == 0)
		return res;
	if(edit)
		return std::vector<std::wstring>{
			td[0][0].toString(), price2wstr(td[0][1].getDouble()), price2wstr(td[0][2].getDouble()),
			td[0][3].getBool() ? L"满减" : L"组合优惠", td[0][4].toString()
		};
	else
		return std::vector<std::wstring>{
		td[0][0].toString(), price2wstr(td[0][1].getDouble()), price2wstr(td[0][2].getDouble()),
			td[0][3].toString(), td[0][4].toString()
	};
}

std::vector<std::vector<std::wstring>> Shop::commodityTdProcess(TableData& td)
{
	std::vector<std::vector<std::wstring>> res;
	if (td.length() == 0)
		return res;
	if (td[0].size() == 4)
	{
		for (size_t i = 0; i < td.length(); i++)
		{
			std::vector<std::wstring> tmp;
			for (size_t j = 0; j < td[i].size(); j++)
			{
				tmp.push_back(td[i][j].toString());

			}
			tmp[3] = price2wstr(tmp[3]);
			res.push_back(tmp);
		}
	}
	else
	{
		for (size_t i = 0; i < td.length(); i++)
		{
			std::vector<std::wstring> tmp;
			for (size_t j = 0; j < td[i].size(); j++)
			{
				tmp.push_back(td[i][j].toString());

			}
			tmp[4] = price2wstr(tmp[4]);
			tmp[5] = td[i][5].getBool() ? L"在售":L"下架";
			res.push_back(tmp);
		}
	}

	return res;
}
 
std::vector<std::vector<std::wstring>> Shop::saleTdProcess(TableData& td)
{
	std::vector<std::vector<std::wstring>> res;
	for (size_t i = 0; i < td.length(); i++)
	{
		std::vector<std::wstring> tmp;
		for (size_t j = 0; j < td[i].size(); j++)
		{
			tmp.push_back(td[i][j].toString());

		}
		tmp[1] = Shop::price2wstr(tmp[1]);
		if (td[i][3].isNull())
			tmp[3] = L"无优惠";
		res.push_back(tmp);
	}
	return res;
}

std::vector<std::vector<std::wstring>> Shop::purchaseTdProcess(TableData& td)
{
	std::vector<std::vector<std::wstring>> res;
	for (size_t i = 0; i < td.length(); i++)
	{
		std::vector<std::wstring> tmp;
		for (size_t j = 0; j < td[i].size(); j++)
		{
			tmp.push_back(td[i][j].toString());

		}
		tmp[2] = price2wstr(tmp[2]);
		res.push_back(tmp);
	}
	return res;
}

std::vector<std::vector<std::wstring>> Shop::discountTdProcess(TableData& td)
{
	std::vector<std::vector<std::wstring>> res;
	for (size_t i = 0; i < td.length(); i++)
	{
		std::vector<std::wstring> tmp;
		for (size_t j = 0; j < td[i].size(); j++)
		{
			tmp.push_back(td[i][j].toString());
		}
		
		tmp[1] = price2wstr(tmp[1]);
		tmp[2] = price2wstr(tmp[2]);
		tmp[3] = td[i][3].getBool()? L"满减" : L"组合优惠";
		res.push_back(tmp);
	}
	return res;
}

int Shop::useDiscount(const vector<vector<CSVData>>& datas, int discountId, double totalPrice)
{
	TableData td = db[L"discount"];
	td = td[td[L"id"] == discountId && td[L"shopId"] == shopId && td[L"delete"] == false][td[L"lowPrice"] && td[L"subPrice"] && td[L"isAll"]];
	if (td.length() == 0 || td[0][0].getDouble()>totalPrice)
		return -1;
	TableData td2 = db[L"discountDetail"];
	td2 = td2[td2[L"discountId"] == discountId && td2[L"shopId"] == shopId][td2[L"commodityId"]];
	std::map<int, bool> res;
	for (size_t i = 0; i < td2.length(); i++)
	{
		res.insert(pair<int, bool>(td2[i][0].getInt(), false));
	}
	if (td[0][2].getBool())
	{
		for (size_t i = 0; i < datas.size(); i++)
		{
			std::map<int, bool>::iterator iter = res.find(datas[i][0].getInt());
			if (iter != res.end())
				iter->second = true;
		}
		for(auto i:res)
			if(!i.second)
				return -1;
	}
	else
	{
		bool hasOne = false;
		for (size_t i = 0; i < datas.size(); i++)
		{
			std::map<int, bool>::iterator iter = res.find(datas[i][0].getInt());
			if (iter != res.end())
			{
				hasOne = true;
				break;
			}
		}
		if(!hasOne)
			return -1;
	}
	totalPrice -= td[0][1].getDouble();
	return totalPrice > 0 ? totalPrice : 0;
}

std::vector<std::vector<std::wstring>> Shop::shelfTdProcess(TableData& td)
{
	std::vector<std::vector<std::wstring>> res;
	for (size_t i = 0; i < td.length(); i++)
	{
		std::vector<std::wstring> tmp;
		for (size_t j = 0; j < td[i].size(); j++)
		{
			tmp.push_back(td[i][j].toString());
		}
		tmp[5] = Shop::price2wstr(tmp[5]);
		res.push_back(tmp);
	}
	return res;
}

std::vector<std::vector<std::wstring>> Shop::commodityListTdProcess(TableData& td)
{
	std::vector<std::vector<std::wstring>> res;
	for (size_t i = 0; i < td.length(); i++)
	{
		std::vector<std::wstring> tmp;
		for (size_t j = 0; j < td[i].size(); j++)
		{
			tmp.push_back(td[i][j].toString());
		}
		tmp[2] = Shop::price2wstr(tmp[2]);
		res.push_back(tmp);
	}
	return res;
}

bool Shop::saveSales(const vector<vector<CSVData>>& datas, double totalPrice,int discountId)
{
	vector<CSVData> index;
	TableData td = db[L"commodity"], d, tp;
	tp = td;
	td = td[td[L"shopId"] == shopId && td[L"delete"] == false][td[L"id"] && td[L"stock"]];
	for (auto i : datas)
	{
		d = td[td[L"id"] == i[0].getInt()];
		int stock = (d[0][1] - i[2]).getInt(), quantity = i[2].getInt();
		if (stock < 0)
		{
			return false;
		}
	}
	time_t timep;
	bool res;
	time(&timep);
	if (discountId != INT_MAX)
	{
		totalPrice = useDiscount(datas, discountId,totalPrice);
		if (totalPrice < 0 )
			return false;
		res = db.insert(L"sale", { L"totalPrice",L"discountId",L"time",L"shopId" },
			{
				CSVData(totalPrice),CSVData(discountId),CSVData(timep,DATATYPE::DATE),CSVData(shopId)
			}, &index);
	}
		
	else
		res = db.insert(L"sale", { L"totalPrice",L"time",L"shopId" },
				{
					CSVData(totalPrice),CSVData(timep,DATATYPE::DATE),CSVData(shopId)
				}, &index);

	if (index.size() < 2 || !res)
		return false;

	for (auto i : datas)
	{		
		d = td[td[L"id"] == i[0].getInt()];
		int stock = (d[0][1] - i[2]).getInt(), quantity = i[2].getInt();
		if (stock < 0)
		{
			stock = 0;
			quantity = d[0][1].getInt();
		}
		db.insert(L"saleDetail", { L"saleId",L"shopId",L"commodityId",L"price",L"quantity" },
				{
					index[1],CSVData(shopId),i[0],i[1],quantity
				});

		db.update(L"commodity",
			{ L"stock" },
			{
				stock
			},
			tp[L"id"] == i[0].getInt() && tp[L"shopId"] == shopId && tp[L"delete"] == false
		);
	}
	return true;
}

bool Shop::saveCommodity(const std::vector<std::wstring>& datas)
{
	try
	{
		if (datas[0] == L"")
		{
			if (datas[1] == L"")
				return false;
			return db.insert(L"commodity", { L"shopId",L"name",L"model",L"category",L"detail",L"price",L"status",L"delete",L"stock" },
				{
					CSVData(shopId),CSVData(datas[1],DATATYPE::STR),
					CSVData(datas[2],DATATYPE::STR),CSVData(datas[3],DATATYPE::STR),
					CSVData(datas[4],DATATYPE::STR),CSVData(datas[5],DATATYPE::DOUBLE),
					CSVData(datas[6],DATATYPE::BOOL),CSVData(false),CSVData(0)
				});
		}
		else
		{
			TableData td = db[L"commodity"];
			int res = db.update(
				L"commodity",
				{ L"name",L"model",L"category",L"detail",L"price",L"status" },
				{
					CSVData(datas[1],DATATYPE::STR),CSVData(datas[2],DATATYPE::STR),CSVData(datas[3],DATATYPE::STR),
					CSVData(datas[4],DATATYPE::STR),CSVData(datas[5],DATATYPE::DOUBLE),CSVData(datas[6],DATATYPE::BOOL)
				},
				td[L"id"] == stoi(datas[0]) && td[L"shopId"] == shopId
			);
			if (res != -1)
				return true;
			else
				return false;
		}
	}
	catch (...)
	{
		return false;
	}
	
}

int Shop::savePurchase(const std::vector<std::wstring>& datas)
{
	try
	{
		TableData td = db[L"commodity"], d;
		d = td[td[L"id"] == stoi(datas[0]) && td[L"shopId"] == shopId && td[L"delete"] == false];
		if (d.length() == 0)
			return 2;

		if (db.insert(L"purchase", { L"shopId",L"commodityId",L"price",L"time",L"channel",L"quantity" },
				{
					CSVData(shopId),CSVData(datas[0],DATATYPE::INT),
					CSVData(datas[1],DATATYPE::DOUBLE),CSVData(datas[2],DATATYPE::DATE),
					CSVData(datas[3],DATATYPE::STR),CSVData(datas[4],DATATYPE::INT)
				}))
		{
			d = d[d[L"stock"]];
			if (d.length() == 0)
				return 2;
			int res = db.update(
				L"commodity",
				{ L"stock" },
				{
					CSVData(datas[4],DATATYPE::INT) + d[0][0]
				},
				td[L"id"] == stoi(datas[0]) && td[L"shopId"] == shopId && td[L"delete"] == false
			);
			if (res > 0)
				return 0;
			else
				return 3;
		}
		return 4;
	}
	catch (...)
	{
		return 1;
	}

}

int Shop::saveDiscount(const std::vector<std::wstring>& datas, const TableData& items)
{
	int id;
	try
	{
		if (datas[0] == L"")
		{
			vector<CSVData> index;

			bool res = db.insert(L"discount", { L"shopId",L"lowPrice" ,L"subPrice" ,L"isAll" ,L"detail" ,L"delete" }, 
				{
					shopId,CSVData(datas[1],DATATYPE::DOUBLE),CSVData(datas[2],DATATYPE::DOUBLE),
					CSVData(datas[3],DATATYPE::BOOL),CSVData(datas[4],DATATYPE::STR),false
				}, &index);

			if (index.size() < 4 || !res)
				return false;

			id = index[1].getInt();
		}
		else
		{
			id = stoi(datas[0]);
			bool res = db.drop(L"discountDetail", FilterGetter(db.getTableHeader(L"discountDetail"), L"discountId") == id);
			if (!res)
				return false;
		}

		for (size_t i = 0; i < items.length(); i++)
		{
			db.insert(L"discountDetail", { L"discountId",L"commodityId",L"shopId" },
				{
					id,const_cast<TableData&>(items)[i][0],shopId
				});
		}
		return db.update(L"discount", { L"id",L"shopId",L"lowPrice" ,L"subPrice" ,L"isAll" ,L"detail" ,L"delete" },
				{
					id,shopId,CSVData(datas[1],DATATYPE::DOUBLE),CSVData(datas[2],DATATYPE::DOUBLE),
					CSVData(datas[3],DATATYPE::BOOL),CSVData(datas[4],DATATYPE::STR),false
				},
			FilterGetter(db.getTableHeader(L"discount"), L"id") == id && 
			FilterGetter(db.getTableHeader(L"discount"), L"shopId") == shopId);
	}
	catch (InvalidValueException e)
	{
		return 2;
	}
	catch (...)
	{
		return 1;
	}

}

bool Shop::removeCommodity(int id)
{
	try
	{
		if (id == INT_MAX)
			return true;
		else
		{
			TableData td = db[L"commodity"];
			return db.update(
				L"commodity",
				{ L"delete" },
				{CSVData(true)},
				td[L"id"] == id && td[L"shopId"] == shopId
			);
		}
	}
	catch (...)
	{
		return false;
	}
}

bool Shop::removeDiscount(int id)
{
	try
	{
		if (id == INT_MAX)
			return true;
		else
		{
			TableData td = db[L"discount"];
			return db.update(
				L"discount",
				{ L"delete" },
				{ CSVData(true) },
				td[L"id"] == id && td[L"shopId"] == shopId
			);
		}
	}
	catch (...)
	{
		return false;
	}
}