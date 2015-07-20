// handle loading data into the application
#include "stdafx.h"
#include "libxl.h" // library for accessing excel data

using namespace libxl;
namespace DataManager{

	void LoadData(){
		double allData[30][35]; // memory reservoir to hold the spreadsheet
		
		Book* TormData = xlCreateBook();
		std::cout << "Book created" << std::endl; // loads tormdata.xls 
		if (TormData->load(L"tormdata.xls")){ // not sure what the 'L' is for
			std::cout << "data loaded" << std::endl; // confirms data can be loaded
			Sheet* sheet = TormData->getSheet(0); // get the first sheet of the document

			for (int j = 2; j < 2 + 3; j++){ // for every column in the spreadsheet
				for (int i = 4; i < 4 + 30; i++){ // get the full column of data
					int value = (int)sheet->readNum(i, j);
					std::cout << value << std::endl;
				}
			}
			/*const wchar_t* leString = sheet->readStr(2, 2);
			std::cout << *leString << std::endl;*/
		}
	}
}