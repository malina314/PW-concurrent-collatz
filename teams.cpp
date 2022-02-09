#include <utility>
#include <deque>
#include <future>
#include <vector>

#include "teams.hpp"
#include "contest.hpp"
#include "collatz.hpp"

void callCalcCollatz(const InfInt &n, uint64_t &res) {
    res = calcCollatz(n);
}

ContestResult TeamNewThreads::runContestImpl(ContestInput const & contestInput)
{
    ContestResult r;
    r.resize(contestInput.size());
    uint64_t idx = 0;

    if (this->getSharedResults()) {
        //TODO
    } else {
        auto size = this->getSize();
        std::vector<std::thread> threads(size);
        size_t thread_idx = 0;
        for (const InfInt &singleInput : contestInput) {
            if (threads[thread_idx].joinable()) {
                threads[thread_idx].join();
            }
            threads[thread_idx] = this->createThread(callCalcCollatz, singleInput, std::ref(r[idx]));
            idx++;
            thread_idx = (thread_idx + 1) % size;
        }

        for (auto &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    return r;
}

ContestResult TeamConstThreads::runContestImpl(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}

ContestResult TeamPool::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}

ContestResult TeamNewProcesses::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}

ContestResult TeamConstProcesses::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}

ContestResult TeamAsync::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}
