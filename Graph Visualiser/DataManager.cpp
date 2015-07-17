// handle loading data into the application
#include "stdafx.h"
#include "libxl.h" // library for accessing excel data

using namespace libxl;
namespace DataManager{

	void LoadData(){
		Book* TormData = xlCreateBook();
		std::cout << "Book created" << std::endl;
		if (TormData->load(L"tormdata.xls")){
			std::cout << "data loaded" << std::endl;
			Sheet* sheet = TormData->getSheet(0);
			int row = 4;
			int col = 2;
			
			std::cout << sheet->readNum(row,col) << std::endl;
			

		}
	}
}