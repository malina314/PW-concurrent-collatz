#ifndef SHAREDRESULTS_HPP
#define SHAREDRESULTS_HPP

#include "lib/infint/InfInt.h"

#include <mutex>

class SharedResults
{
    std::map<InfInt, uint64_t> results;
    std::mutex m;

public:
    uint64_t getResult(const InfInt &n) {
        std::lock_guard<std::mutex> lock(m);
        auto it = results.find(n);

        if (it == results.end()) {
            uint64_t r = calcCollatz(n);
            results[n] = r;
            return r;
        }

        return it->second;
    }
};

#endif