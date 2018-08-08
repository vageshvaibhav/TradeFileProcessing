//
//  testUtilFunction.cpp
//  TestTradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 08/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

#include "../../TradeProcessingApplication/TradeProcessingApplication/application_utilities.h"
#include "catch.h"
#include <set>

TEST_CASE( "Verify utility functions for Trade attributes", "[Set of Trade data]" ) {
    //TimeStamp,Quantity,Price
    std::set<TradeData> s {TradeData(500,2,3),TradeData(600,5,7),TradeData(1100,8,9)};
    
    REQUIRE( totalVolumeTraded(s) == 15 );
    REQUIRE( averagWeightedPrice(s) == 7 );
    REQUIRE( maxPrice(s) == 9 );
    REQUIRE( maxTimeGap(s) == 500 );
    
    SECTION( "Adding one more trade to verify change in Trade Attributes" ) {
        s.insert(TradeData(1900,10,11));
        
        REQUIRE( totalVolumeTraded(s) == 25 );
        REQUIRE( averagWeightedPrice(s) == 8 );
        REQUIRE( maxPrice(s) == 11 );
        REQUIRE( maxTimeGap(s) == 800 );
    }

}
