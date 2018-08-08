//
//  trade_data.cpp
//  TradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 08/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

#include "trade_data.h"

TradeData::TradeData(unsigned long int ts, unsigned long int tq, unsigned long int tp) :m_timeStamp(ts),m_tradeQuantity(tq),m_tradePrice(tp)
{
}

bool TradeData::operator==(const TradeData &other) const
{ return (m_timeStamp == other.m_timeStamp
          && m_tradeQuantity == other.m_tradeQuantity
          && m_tradePrice == other.m_tradePrice);
}
