//
//  main.cpp
//  TradeProcessingApplication
//
//  Created by Vagesh Vaibhav on 05/08/18.
//  Copyright © 2018 Vagesh Vaibhav. All rights reserved.
//

#include <boost/program_options.hpp>
#include <exception>
#include "trade_data.h"
#include "application_utilities.h"
#include "trade_file_handler.h"


int main(int argc, const char * argv[]) {
 
    boost::program_options::options_description desc("Allowed options");
    boost::program_options::variables_map vm;
    bool processLargeFileInChunks;
    std::string filePath,inputFileName,outputFileName;
    try
    {
        desc.add_options()
        ("help,h", "help message")
        ("processLargeFileInChunks,p", boost::program_options::value<bool>(&processLargeFileInChunks)->default_value(false), "Instructs to Process large Trades file in chunks based on per instrument data, default false")
        ("filePath,f", boost::program_options::value<std::string>(&filePath)->default_value(""), "Path to the Trades file")
        ("inputFileName,i", boost::program_options::value<std::string>(&inputFileName)->default_value("input.csv"), "Input file Name")
        ("outputFileName,o", boost::program_options::value<std::string>(&outputFileName)->default_value("output.csv"), "Output file Name")
        ;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);
    }
    catch(...)
    {
        std::cerr << "Failed to parse command line args" << std::endl;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    auto path = boost::filesystem::path(filePath);
    if(!filePath.empty() && !(exists(path) || is_directory(path)))
    {
        std::cerr << "Invalid file path provided, returning,"<<std::endl;
        return 1;
    }

    
    TradeFileHandler tfh(filePath,inputFileName,outputFileName,processLargeFileInChunks);
    auto success = tfh.ParseInputTradesFile(); //Parse Trades file into memory or small files
    if(!success)
    {
        return 1;
    }
    
    if(!processLargeFileInChunks)
    {
        tfh.WriteProcessedTradeFileToOutFile();
    }
    else
    {
        tfh.SplitTradeFilePerSymbolAsync();
        tfh.WriteMergedSymbolFileToOutFile();
    }

    return 0;
}
