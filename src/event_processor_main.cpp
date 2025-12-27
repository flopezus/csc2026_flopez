// CSC Latin America 2026 - Event Processor Demo
// Demonstrates OpenMP parallelization for Exercise SD E1
#include "EventProcessor.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace csc2026;

void measureScaling(int nEvents, int nParticles) {
    std::cout << "=== Scaling Test ===" << std::endl;
    std::cout << "Events: " << nEvents << ", Particles/event: " << nParticles << std::endl;
    std::cout << std::endl;
    
    auto events = generateSampleEvents(nEvents, nParticles);
    
    std::cout << std::setw(10) << "Threads" 
              << std::setw(15) << "Time (ms)"
              << std::setw(15) << "Speedup"
              << std::setw(15) << "Efficiency"
              << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    double baselineTime = 0;
    
    for (int nThreads : {1, 2, 4, 8}) {
        #ifdef _OPENMP
        omp_set_num_threads(nThreads);
        #endif
        
        EventProcessor processor;
        
        auto start = std::chrono::high_resolution_clock::now();
        processor.processEvents(events);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double timeMs = duration.count() / 1000.0;
        
        if (nThreads == 1) {
            baselineTime = timeMs;
        }
        
        double speedup = baselineTime / timeMs;
        double efficiency = speedup / nThreads * 100.0;
        
        std::cout << std::setw(10) << nThreads
                  << std::setw(15) << std::fixed << std::setprecision(2) << timeMs
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup << "x"
                  << std::setw(14) << std::fixed << std::setprecision(1) << efficiency << "%"
                  << std::endl;
    }
    
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "CSC 2026 - Event Processor Scaling Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    
    #ifdef _OPENMP
    std::cout << "OpenMP enabled, max threads: " << omp_get_max_threads() << std::endl;
    #else
    std::cout << "OpenMP not available" << std::endl;
    #endif
    
    std::cout << std::endl;
    
    // Small test
    measureScaling(1000, 100);
    
    // Larger test
    measureScaling(10000, 100);
    
    return 0;
}
