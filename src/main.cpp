#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

#ifdef _WIN32
// Windows specific headers
#include <windows.h>
#define HAVE_STRUCT_TIMESPEC
#endif

#include <pthread.h>
#include "../include/crawler.hpp"
#include "../include/utils.hpp"

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -i, --input <file>     Input file containing URLs (default: data/urls.txt)" << std::endl;
    std::cout << "  -o, --output <file>    Output file for results (default: data/results.csv)" << std::endl;
    std::cout << "  -t, --threads <num>    Number of threads to use (default: 4)" << std::endl;
    std::cout << "  -e, --extended         Collect extended information (links, images, headings, etc.)" << std::endl;
    std::cout << "  -h, --help             Display this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        std::string inputFile = "data/urls.txt";
        std::string outputFile = "data/results.csv";
        int numThreads = 4;
        bool extendedInfo = false;

        std::cout << "Starting Parallel Web Crawler on Windows..." << std::endl;

        // Parse command line arguments
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help") {
                printUsage(argv[0]);
                return 0;
            } else if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
                inputFile = argv[++i];
            } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
                outputFile = argv[++i];
            } else if ((arg == "-t" || arg == "--threads") && i + 1 < argc) {
                numThreads = std::stoi(argv[++i]);
            } else if (arg == "-e" || arg == "--extended") {
                extendedInfo = true;
            } else {
                std::cerr << "Unknown argument: " << arg << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        }

        std::cout << "Parallel Web Crawler" << std::endl;
        std::cout << "Input file: " << inputFile << std::endl;
        std::cout << "Output file: " << outputFile << std::endl;
        std::cout << "Number of threads: " << numThreads << std::endl;
        std::cout << "Extended info: " << (extendedInfo ? "yes" : "no") << std::endl;

        // Read URLs from input file
        std::cout << "Reading URLs from file: " << inputFile << std::endl;
        std::vector<std::string> urls = Utils::readUrlsFromFile(inputFile);
        if (urls.empty()) {
            std::cerr << "No URLs found in input file or file couldn't be opened." << std::endl;
            return 1;
        }

        std::cout << "Loaded " << urls.size() << " URLs:" << std::endl;
        for (const auto& url : urls) {
            std::cout << " - " << url << std::endl;
        }

        std::cout << "Initializing crawler..." << std::endl;
        
        // Start the timer
        auto startTime = std::chrono::high_resolution_clock::now();

        // Create and run the crawler with extended info option
        Crawler crawler(urls, outputFile, numThreads, extendedInfo);
        
        std::cout << "Starting crawler run..." << std::endl;
        crawler.run();
        
        std::cout << "Crawler run completed!" << std::endl;

        // Stop the timer and calculate elapsed time
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        std::cout << "Crawling completed in " << duration.count() / 1000.0 << " seconds." << std::endl;
        std::cout << "Results saved to " << outputFile << std::endl;

        if (extendedInfo) {
            std::cout << "Extended information was collected and saved to additional files." << std::endl;
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception caught!" << std::endl;
        return 1;
    }
} 