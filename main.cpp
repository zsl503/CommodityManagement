#pragma once
#include <iostream>
#include <Windows.h>
#include <ComponentBase.h>
#include <App.h>
#include <csvdb.h>

#include "Components.h"
#include "MainPage.h"
#include "LoginPage.h"
#include "DiscountPage.h"
#include "ShopApp.h"
using namespace View;
using namespace csvdb;
using namespace csvop;
using csvdb::table::Field;
bool checkTables(Database db)
{
	return db.tableExist(L"user") && db.tableExist(L"commodity") &&
		db.tableExist(L"purchase") && db.tableExist(L"sale") &&
		db.tableExist(L"saleDetail") && db.tableExist(L"discount") && db.tableExist(L"shop");
}

void initTables(csvdb::Database db)
{
	Field f1(L"id", DATATYPE::INT, true, 0);
	Field f2(L"bossName", DATATYPE::STR, false, 1);
	Field f3(L"name", DATATYPE::STR, false, 2);
	Field f4(L"password", DATATYPE::STR, false, 3);
	Field f5(L"createTime", DATATYPE::DATE, false, 4);
	if(!db.tableExist(L"shop"))
		db.createTable(L"shop", { f1 ,f2,f3,f4,f5 });

	f1 = Field(L"id", DATATYPE::INT, false, 0,L"",true,1000);
	f2 = Field(L"shopId", DATATYPE::INT, true, 1,L"shop.id");
	f3 = Field(L"name", DATATYPE::STR, true, 2);
	f4 = Field(L"model", DATATYPE::STR, true, 3);
	f5 = Field(L"category", DATATYPE::STR, true, 4);
	Field f6(L"detail", DATATYPE::STR, false, 5);
	Field f7(L"price", DATATYPE::DOUBLE, false, 6);
	Field f8(L"status", DATATYPE::BOOL, false, 7);
	Field f9(L"delete", DATATYPE::BOOL, true, 8);
	Field f10(L"stock", DATATYPE::INT, false, 9);
	if (!db.tableExist(L"commodity"))
		db.createTable(L"commodity", { f1 ,f2,f3,f4,f5,f5,f6,f7,f8,f9,f10 });

	f1 = Field(L"id", DATATYPE::INT, true, 0,L"",true,1000);
	f2 = Field(L"shopId", DATATYPE::INT, false, 1, L"shop.id");
	f3 = Field(L"commodityId", DATATYPE::INT, false, 2, L"commodity.id");
	f4 = Field(L"price", DATATYPE::DOUBLE, false, 3);
	f5 = Field(L"time", DATATYPE::DATE, false, 4);
	f6 = Field(L"channel", DATATYPE::STR, false, 5);
	f7 = Field(L"quantity", DATATYPE::INT, false, 6);
	if (!db.tableExist(L"purchase"))
		db.createTable(L"purchase", { f1 ,f2,f3,f4,f5,f6,f7 });

	f1 = Field(L"id", DATATYPE::INT, true, 0, L"", true, 1000);
	f2 = Field(L"shopId", DATATYPE::INT, false, 1, L"shop.id");
	f3 = Field(L"lowPrice", DATATYPE::DOUBLE, false, 2);
	f4 = Field(L"subPrice", DATATYPE::DOUBLE, false, 3);
	f5 = Field(L"isAll", DATATYPE::BOOL, false, 4);
	f6 = Field(L"detail", DATATYPE::STR, false, 5);
	f7 = Field(L"delete", DATATYPE::BOOL, false, 6);
	if (!db.tableExist(L"discount"))
		db.createTable(L"discount", { f1 ,f2,f3,f4,f5,f6,f7 });

	f1 = Field(L"discountId", DATATYPE::INT, true, 0, L"discount.id");
	f3 = Field(L"commodityId", DATATYPE::INT, true, 1, L"commodity.id");
	f2 = Field(L"shopId", DATATYPE::INT, true, 2, L"shop.id");
	if (!db.tableExist(L"discountDetail"))
		db.createTable(L"discountDetail", { f1 ,f2,f3 });

	f1 = Field(L"id", DATATYPE::INT, true, 0, L"", true, 1000);
	f2 = Field(L"totalPrice", DATATYPE::DOUBLE, false, 1);
	f3 = Field(L"discountId", DATATYPE::INT, false, 2, L"discount.id");
	f4 = Field(L"time", DATATYPE::TIME, false, 3);
	f5 = Field(L"shopId", DATATYPE::INT, false, 4, L"shop.id");
	if (!db.tableExist(L"sale"))
		db.createTable(L"sale", { f1,f2,f3,f4,f5 });

	f1 = Field(L"saleId", DATATYPE::INT, true, 0, L"sale.id");
	f2 = Field(L"shopId", DATATYPE::INT, true, 1, L"shop.id");
	f3 = Field(L"commodityId", DATATYPE::INT, true, 2, L"commodity.id");
	f6 = Field(L"price", DATATYPE::DOUBLE, false, 5);
	f7 = Field(L"quantity", DATATYPE::INT, false, 6);
	if (!db.tableExist(L"saleDetail"))
		db.createTable(L"saleDetail", { f1 ,f2,f3,f4,f5 ,f6,f7});
}

void begin()
{
	Database db(L"C:\\CSVDatabase\\");
	if (!checkTables(db))
		initTables(db);
	ShopApp app(db,120,30);
	std::map<std::wstring, View::PageBase*> route = {
		{L"login",new LoginPage()},
		{L"main",new MainPage()},
		{L"discount",new DiscountPage()},
	};
	app.registerPages(route);
	app.navigateTo(L"login");
}

int main()
{
	try {
		begin();
	}
	catch(DatabaseBrokenException e) {
		Database::createDatabase(L"C:\\CSVDatabase\\");
		begin();
	}
	return 0;

}