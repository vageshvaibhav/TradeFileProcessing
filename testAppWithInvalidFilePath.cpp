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

TEST_CASE( "Verify Application return values with path or input file that does not exist or invalid" ) {
    //TimeStamp,Quantity,Price
    TradeFileHandler tfh("/abc","random_file.csv","output_test.csv",false); //processLargeFileInChunks=false
    bool parsingSuccess = tfh.ParseInputTradesFile();
    REQUIRE( parsingSuccess == false );
    //Now Test with default path
    TradeFileHandler tfh1("","input_test.csv","output_test.csv",false); //processLargeFileInChunks=false
    parsingSuccess = tfh1.ParseInputTradesFile();
    REQUIRE( parsingSuccess == true );
    
}



