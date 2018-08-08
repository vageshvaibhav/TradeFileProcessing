//
//  testAppWithSmallInput.cpp
//  TestTradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 08/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

//#include "../../TradeProcessingApplication/TradeProcessingApplication/application_utilities.h"
//#include "../../TradeProcessingApplication/TradeProcessingApplication/trade_data.h"
#include "../../TradeProcessingApplication/TradeProcessingApplication/trade_file_handler.h"
#include<fstream>
#include<string>
#include "catch.h"

TEST_CASE( "Verify Application with splitting large file in small chunks for inputs input_test.csv against small Output" ) {
    //TimeStamp,Quantity,Price
    TradeFileHandler tfh("","input_test.csv","output_test.csv",true); //processLargeFileInChunks=true
    tfh.ParseInputTradesFile();
    tfh.SplitTradeFilePerSymbolAsync();
    tfh.WriteMergedSymbolFileToOutFile();
    std::string tradeRow;
    boost::filesystem::path full_path(boost::filesystem::system_complete("./"));
    full_path+= "output_test.csv";
    std::ifstream file;
    file.open(full_path.string());
    if(file.is_open())
        std::getline(file, tradeRow);
    REQUIRE( tradeRow == "aaa,5787,40,1161,1222" );
    std::getline(file, tradeRow);
    REQUIRE( tradeRow == "aab,6103,69,810,907" );
    std::getline(file, tradeRow);
    REQUIRE( tradeRow == "aac,3081,41,559,638" );
    file.close();
}



