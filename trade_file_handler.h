//
//  trade_file_handler.h
//  TradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 06/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

#ifndef trade_file_handler_h
#define trade_file_handler_h
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <future>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "application_utilities.h"
#include <cstdio>

class TradeFileHandler
{
    
private:
    std::ifstream m_inputTradeStream;
    std::ofstream m_outputTradeStream;
    std::string m_directoryPathString;
    boost::filesystem::path m_inputFilePath;
    boost::filesystem::path m_outputFilePath;
    std::map<std::string,std::set<TradeData,std::less<TradeData>>> m_parsedData;
    std::set<std::string> m_individualSymbolFileNameSet;
    bool m_processLargeFilesInChunks;
    std::vector<std::pair<std::string,std::shared_future<bool>>> m_asyncSymbolFutures;
    
public:
    
    TradeFileHandler(std::string filePath="",std::string inputfileName="input.csv",std::string outputFileName ="output.csv",bool processLargeFilesInChunks = false);
    ~TradeFileHandler();
    
    //output:ParseInputTradesFile: Parses and load input trades file in memory or split large file to small sorted files per symbol
    //Input: TradeFileHandler initialized with filepaths and filenames along with split files processing preference
    bool ParseInputTradesFile();
    
    //output:WriteProcessedTradeFileToOutFile: Writes the processed symbol's trade information to output file
    //Input: TradeFileHandler with sorted and processed trades data loaded in memory/map
    void WriteProcessedTradeFileToOutFile();
    
    //output:CreateProcessedSymbolFile: Intermediate function to process and write sorted processed symbol data on temp file(per symbol)
    //Input: temporary sorted file per symbol
    bool CreateProcessedSymbolFile(std::string symbolFileName);
    
    //output:SplitTradeFilePerSymbolAsync: Processed trade data files for all symbols asynchronously
    //Input: All Unprocessed small files with single symbol data
    void SplitTradeFilePerSymbolAsync();
    
    //output:WriteMergedSymbolFileToOutFile: Merges and writes sorted processed trade data per symbol
    //Input: Small temporary processed files per symbol
    void WriteMergedSymbolFileToOutFile();
    
};

#endif /* trade_file_handler_h */
