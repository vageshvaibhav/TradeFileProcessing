//
//  trade_file_handler.cpp
//  TradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 08/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

#include "../../TradeProcessingApplication/TradeProcessingApplication/trade_file_handler.h"

TradeFileHandler::TradeFileHandler(std::string filePath,std::string inputfileName,std::string outputFileName,bool processLargeFilesInChunks) :
m_processLargeFilesInChunks(processLargeFilesInChunks)
{
    if(filePath.empty())
    {
        boost::filesystem::path full_path(boost::filesystem::system_complete("./"));
        filePath = full_path.string();
        std::cout<<"Default File Path ="<< filePath<<"\n"<<std::flush;
    }
    m_inputFilePath = boost::filesystem::path(filePath);
    m_directoryPathString = m_inputFilePath.string();
    if (exists(m_inputFilePath) && is_directory(m_inputFilePath))
    {
        m_inputFilePath += inputfileName;
    }
    
    m_outputFilePath = boost::filesystem::path(filePath);
    if (exists(m_outputFilePath) && is_directory(m_outputFilePath))
    {
        m_outputFilePath += outputFileName;
    }
    
    
}


TradeFileHandler::~TradeFileHandler()
{
    if(m_inputTradeStream.is_open())
    {
        m_inputTradeStream.close();
    }
    if(m_outputTradeStream.is_open())
    {
        m_outputTradeStream.close();
    }
}


bool TradeFileHandler::ParseInputTradesFile()
{
    auto directoryPath = boost::filesystem::path(m_directoryPathString);
    if (!(exists(directoryPath) || is_directory(directoryPath)) || !(exists(m_inputFilePath)))
    {
        return false;
    }
    std::vector<std::string> tradesVector;
    m_inputTradeStream.open(m_inputFilePath.string());
    if(m_inputTradeStream.is_open())
    {
        tradesVector.clear();
        std::string tradeRow;
        while(std::getline(m_inputTradeStream, tradeRow))
        {
            
            boost::split(tradesVector,tradeRow, boost::is_any_of(","));
            if(tradesVector.size() == TradeDataFields::MaxItemsInTradeRow) //Format of the file is <TimeStamp>,<Symbol>,<Quantity>,<Price>
            {
                if (m_processLargeFilesInChunks)
                {
                    //Create a file per symbol so that large trade data file is sub divided in to
                    //smaller files having trade information for only one symbol
                    std::ofstream perSymbolFile;
                    std::string perSymbolFileName (m_directoryPathString + tradesVector[TradeDataFields::Symbol]);
                    perSymbolFile.open(perSymbolFileName,std::ios_base::app);
                    perSymbolFile << tradeRow<<"\n";
                    m_individualSymbolFileNameSet.insert(tradesVector[TradeDataFields::Symbol]);
                    perSymbolFile.close();
                }
                else
                {
                    auto iter = m_parsedData.find(tradesVector[TradeDataFields::Symbol]);
                    if(iter != m_parsedData.end())
                    {
                        auto tradeInsertIter = iter->second.insert(TradeData(stoul(tradesVector[TradeDataFields::TimeStamp]),stoul(tradesVector[TradeDataFields::Quantity]),stoul(tradesVector[TradeDataFields::Price])));
                        if(!tradeInsertIter.second )
                        {
                            std::cout<<"Could not insert Trade data to map for symbol = "<< tradesVector[TradeDataFields::Symbol]<<"timestamp=" <<tradesVector[TradeDataFields::TimeStamp];
                        }
                    }
                    else
                    {
                        std::set<TradeData> temp{TradeData(stoul(tradesVector[TradeDataFields::TimeStamp]),stoul(tradesVector[TradeDataFields::Quantity]),stoul(tradesVector[TradeDataFields::Price]))};
                        m_parsedData[tradesVector[TradeDataFields::Symbol]] = temp;
                    }
                }
            }
            
            else
            {
                std::cerr<< "Invalid number of fields in Trade Row,number of fields= " << tradesVector.size() << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Could not open file, filename =" << m_inputFilePath <<std::endl<<std::flush;
    }
    return true;
}

void TradeFileHandler::WriteProcessedTradeFileToOutFile()
{
    m_outputTradeStream.open(m_outputFilePath.string());
    if(m_outputTradeStream.is_open())
    {
        std::cout <<"outputting <symbol>,<MaxTimeGap>,<Volume>,<WeightedAveragePrice>,<MaxPrice> for parsedData size =" <<m_parsedData.size() <<"\n";
        for (auto &v : m_parsedData)
        {
            m_outputTradeStream << v.first <<","<< maxTimeGap(v.second) <<","<<totalVolumeTraded(v.second)<<","<<averagWeightedPrice(v.second)<<"," << maxPrice(v.second)<<"\n";
        }
    }
    m_outputTradeStream.close();
}


bool TradeFileHandler::CreateProcessedSymbolFile(std::string symbolFileName)
{
    std::set<TradeData,std::less<TradeData>> parsedData;
    std::vector<std::string> tradesVector;
    std::ifstream inputTradeStream;
    std::ofstream outputTradeStream;
    std::string symbol;
    inputTradeStream.open(symbolFileName);
    if(inputTradeStream.is_open())
    {
        tradesVector.clear();
        std::string tradeRow;
        while(std::getline(inputTradeStream, tradeRow))
        {
            
            boost::split(tradesVector,tradeRow, boost::is_any_of(","));
            if(tradesVector.size() == TradeDataFields::MaxItemsInTradeRow) //Format of the file is <TimeStamp>,<Symbol>,<Quantity>,<Price>
            {
                symbol = tradesVector[TradeDataFields::Symbol]; //This file has rows only for one symbol
                auto tradeInsertIter = parsedData.insert(TradeData(stoul(tradesVector[TradeDataFields::TimeStamp]),stoul(tradesVector[TradeDataFields::Quantity]),stoul(tradesVector[TradeDataFields::Price])));
                if(!tradeInsertIter.second )
                {
                    std::cout<<"Could not insert Symbol's Trade data in set for symbol  = "<< tradesVector[TradeDataFields::Symbol]<<"timestamp=" <<tradesVector[TradeDataFields::TimeStamp]<<"\n";
                }
                
            }
            else
            {
                std::cerr<< "Invalid number of fields in Trade Row,number of fields= " << tradesVector.size() << std::endl;
            }
        }
    }
    
    inputTradeStream.close();
    outputTradeStream.open(symbolFileName+"_processed");
    if(outputTradeStream.is_open())
    {
        //<symbol>,<MaxTimeGap>,<Volume>,<WeightedAveragePrice>,<MaxPrice>
        outputTradeStream << symbol <<","<< maxTimeGap(parsedData) <<","<<totalVolumeTraded(parsedData)<<","<<averagWeightedPrice(parsedData)<<"," << maxPrice(parsedData)<<"\n";
    }
    return true;
}


void TradeFileHandler::SplitTradeFilePerSymbolAsync()
{
    for( auto& symbolFile : m_individualSymbolFileNameSet)
    {
        std::string tempSymbolFile(m_directoryPathString+symbolFile);
        //Launch each Symbol file handler asynchronously
        auto fut = std::async(std::launch::async, &TradeFileHandler::CreateProcessedSymbolFile,this, tempSymbolFile);
        m_asyncSymbolFutures.emplace_back(std::make_pair(symbolFile,fut.share()));
    }
    
}


void TradeFileHandler::WriteMergedSymbolFileToOutFile()
{
    m_outputTradeStream.open(m_outputFilePath.string());
    for(auto& item : m_asyncSymbolFutures)
    {
        std::ifstream tempObject;
        if(item.second.get()) //Validate the future we got from Async Task
        {
            tempObject.open(m_directoryPathString+item.first+"_processed");
            m_outputTradeStream << tempObject.rdbuf();
        }
        tempObject.close();
        remove((m_directoryPathString+item.first).c_str());
        remove((m_directoryPathString+item.first+"_processed").c_str());
    }
    m_outputTradeStream.close();
    std::cout<<"Calculated output successfully written to output file"<<"\n";
}
