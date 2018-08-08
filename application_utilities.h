//
//  application_utilities.h
//  TradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 06/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

#ifndef application_utilities_h
#define application_utilities_h
#include<future>
#include<set>
#include<vector>
#include<numeric>
#include "trade_data.h"

//output:totalVolumeTraded: Calculates total volume traded per instrument/symbol
//Input: Set with sorted trade data for a symbol
unsigned long totalVolumeTraded(const std::set<TradeData> symbolTradeData);

//output:averagWeightedPrice: Calculates average weighted price per instrument/symbol
//Input: Set with sorted trade data for a symbol
unsigned long averagWeightedPrice(const std::set<TradeData> symbolTradeData);

//output:maxPrice: Calculates Max price received per instrument/symbol
//Input: Set with sorted trade data for a symbol
unsigned long maxPrice(const std::set<TradeData> symbolTradeData);

//output:maxTimeGap: Calculates Maximum time gap in subsequent trades per instrument/symbol
//Input: Set with sorted trade data for a symbol
unsigned long maxTimeGap( const std::set<TradeData> symbolTradeData);



#endif /* application_utilities_h */
