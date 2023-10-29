#pragma once
#include "App.h"
#include "Shop.h"
#include "csvdb.h"
class ShopApp :
	public View::App
{
public:
	Shop shop;
	ShopApp(csvdb::Database db, int width, int height) : shop(db), App(width, height){}
};