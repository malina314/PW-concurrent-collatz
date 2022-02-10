#ifndef SHAREDRESULTS_HPP
#define SHAREDRESULTS_HPP

#include "lib/infint/InfInt.h"

#include <mutex>

class SharedResults
{
    static constexpr int N = 100'000;

    std::map<InfInt, uint64_t> results;
    std::mutex map_mutex, array_mutex;
    uint64_t *small_results;

public:
    SharedResults() : results(), map_mutex(), array_mutex() {
        small_results = new uint64_t[N]();
    }

    uint64_t getResult(const InfInt &n) {
        if (n < InfInt(N)) {
            int small_n = n.toInt();

            if (small_results[small_n] != 0) {
                return small_results[small_n];
            }

            std::lock_guard<std::mutex> lock(array_mutex);

            small_results[small_n] = calcCollatz(n);

            return small_results[small_n];
        }

        std::lock_guard<std::mutex> lock(map_mutex);
        auto it = results.find(n);

        if (it == results.end()) {
            uint64_t r = calcCollatz(n);
            results[n] = r;
            return r;
        }

        return it->second;
    }

    ~SharedResults() {
        delete [] small_results;
    }
};

#endif