//
//  trade_data.h
//  TradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 05/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

#ifndef trade_data_h
#define trade_data_h
#include <string>
struct TradeData
{
    TradeData(unsigned long int ts, unsigned long int tq, unsigned long int tp);
    bool operator==(const TradeData &other) const;
    unsigned long int m_timeStamp;
    unsigned long int m_tradeQuantity;
    unsigned long int m_tradePrice; // Taking unsigned as trade price is always > 0 as mentioned in assumptions section of instructions.txt
};

//Using enum for Trade row fields i.e. <TimeStamp>,<Symbol>,<Quantity>,<Price>
// MaxItemsInTradeRow for validating number of fields in a row
enum TradeDataFields
{
    TimeStamp=0,
    Symbol=1,
    Quantity=2,
    Price=3,
    MaxItemsInTradeRow=4
};

//Comparator specialization for Trade data to be used in std::set/map
namespace std
{
    template<>
    struct less<TradeData>
    {
        bool operator()(const TradeData& lhs,const TradeData& rhs)
        {
            return ((lhs.m_timeStamp < rhs.m_timeStamp) || ((lhs.m_timeStamp == rhs.m_timeStamp) && (lhs.m_tradePrice < rhs.m_tradePrice)));
        }
    };
    
    template<>
    struct hash<TradeData>
    {
        std::size_t operator() (const TradeData& td)
        {
            using std::hash;
            
            return((hash<unsigned long int>()(td.m_timeStamp)) ^ (hash<unsigned long int>()(td.m_tradePrice)) ^ (hash<unsigned long int>()(td.m_tradeQuantity)) );
        }
    };
}



#endif /* trade_data_h */
