//
//  application_utilities.cpp
//  TradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 08/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

#include "application_utilities.h"

unsigned long totalVolumeTraded(const std::set<TradeData> symbolTradeData)
{
    unsigned long totalQuantity = 0;
    std::for_each(symbolTradeData.begin(), symbolTradeData.end(), [&totalQuantity](const TradeData &td)
                  {
                      totalQuantity+= td.m_tradeQuantity;
                  });
    return totalQuantity;
}


unsigned long averagWeightedPrice(const std::set<TradeData> symbolTradeData)
{
    unsigned long weightedSum = 0;
    std::for_each(symbolTradeData.begin(), symbolTradeData.end(), [&weightedSum](const TradeData& td){
        weightedSum += td.m_tradePrice*td.m_tradeQuantity;
    });
    
    unsigned long totalQuantity = 0;
    std::for_each(symbolTradeData.begin(), symbolTradeData.end(), [&totalQuantity](const TradeData &td)
                  {
                      totalQuantity+= td.m_tradeQuantity;
                  });
    return (weightedSum/totalQuantity);
}

unsigned long maxPrice(const std::set<TradeData> symbolTradeData)
{
    auto iter = std::max_element(symbolTradeData.begin(), symbolTradeData.end(), [](const TradeData& td1,const TradeData& td2)->bool {
        return td1.m_tradePrice < td2.m_tradePrice;
    });
    return iter->m_tradePrice;
}


unsigned long maxTimeGap( const std::set<TradeData> symbolTradeData)
{
    std::vector<unsigned long int> temp,tempResult;
    std::for_each(symbolTradeData.begin(),symbolTradeData.end(),[&temp](const TradeData& td)
                  {
                      temp.push_back(td.m_timeStamp);
                  });
    
    std::adjacent_difference(temp.begin(), temp.end(), std::back_inserter(tempResult));
    if(symbolTradeData.size() == 1 || symbolTradeData.size() == 0)
    {
        return 0; //if only 1/0 trade is in the file then the time gap is 0.
    }
    else
    {
        return *std::max_element(tempResult.begin()+1, tempResult.end());
    }
}
