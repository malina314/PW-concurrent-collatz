#include <utility>
#include <deque>
#include <future>
#include <vector>

#include "teams.hpp"
#include "contest.hpp"
#include "collatz.hpp"

#include "lib/infint/InfInt.h"

void callCalcCollatz(const InfInt &n, uint64_t &res) {
    res = calcCollatz(n);
}

void callCalcCollatzX(const InfInt &n, uint64_t &res, std::shared_ptr<SharedResults> sharedResults) {
    res = sharedResults->getResult(n);
}

void callManyCalcCollatz(std::vector<InfInt> &tasks, std::vector<uint64_t> &res) {
    for (int i = 0; i < tasks.size(); ++i) {
        res[i] = calcCollatz(tasks[i]);
    }
}

void callManyCalcCollatzX(std::vector<InfInt> &tasks,
                          std::vector<uint64_t> &res,
                          std::shared_ptr<SharedResults> sharedResults) {
    for (int i = 0; i < tasks.size(); ++i) {
        res[i] = sharedResults->getResult(tasks[i]);
    }
}

ContestResult TeamNewThreads::runContestImpl(ContestInput const & contestInput)
{
    ContestResult r;
    r.resize(contestInput.size());
    uint64_t idx = 0;
    auto size = this->getSize();
    std::vector<std::thread> threads(size);
    size_t thread_idx = 0;

    if (this->getSharedResults()) {
        for (const InfInt &singleInput : contestInput) {
            if (threads[thread_idx].joinable()) {
                threads[thread_idx].join();
            }
            threads[thread_idx] = this->createThread(callCalcCollatzX,
                                                     singleInput,
                                                     std::ref(r[idx]),
                                                     this->getSharedResults());
            idx++;
            thread_idx = (thread_idx + 1) % size;
        }
    } else {
        for (const InfInt &singleInput : contestInput) {
            if (threads[thread_idx].joinable()) {
                threads[thread_idx].join();
            }
            threads[thread_idx] = this->createThread(callCalcCollatz, singleInput, std::ref(r[idx]));
            idx++;
            thread_idx = (thread_idx + 1) % size;
        }
    }

    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return r;
}

ContestResult TeamConstThreads::runContestImpl(ContestInput const & contestInput)
{
    ContestResult r;
    r.resize(contestInput.size());
    auto size = this->getSize();
    std::vector<std::thread> threads(size);
    std::vector<std::vector<InfInt>> tasks_for_threads(size);
    std::vector<std::vector<uint64_t>> results(size);
    size_t tasks_idx = 0;

    for (const InfInt &singleInput : contestInput) {
        tasks_for_threads[tasks_idx % size].push_back(std::ref(singleInput));
        tasks_idx++;
    }

    if (this->getSharedResults()) {
        for (int i = 0; i < size; ++i) {
            results[i].resize(tasks_for_threads[i].size());
            threads[i] = this->createThread(callManyCalcCollatzX,
                                            std::ref(tasks_for_threads[i]),
                                            std::ref(results[i]),
                                            this->getSharedResults());
        }
    } else {
        for (int i = 0; i < size; ++i) {
            results[i].resize(tasks_for_threads[i].size());
            threads[i] = this->createThread(callManyCalcCollatz,
                                            std::ref(tasks_for_threads[i]),
                                            std::ref(results[i]));
        }
    }

    for (auto &t : threads) {
        t.join();
    }

    for (int i = 0; i < contestInput.size(); ++i) {
        r[i] = results[i % size][i / size];
    }

    return r;
}

ContestResult TeamPool::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    r.resize(contestInput.size());

    if (this->getSharedResults()) {
        //TODO
    } else {
        std::vector<std::future<uint64_t>> futures(contestInput.size());
        size_t idx = 0;
        for (const InfInt &singleInput : contestInput) {
            futures[idx] = this->pool.push(calcCollatz, singleInput);
            idx++;
        }

        for (idx = 0; idx < contestInput.size(); ++idx) {
            r[idx] = futures[idx].get();
        }
    }

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
    r.resize(contestInput.size());

    if (this->getSharedResults()) {
        //TODO
    } else {
        std::vector<std::future<uint64_t>> futures(contestInput.size());
        size_t idx = 0;
        for (const InfInt &singleInput : contestInput) {
            futures[idx] = std::async(calcCollatz, singleInput);
            idx++;
        }

        for (idx = 0; idx < contestInput.size(); ++idx) {
            r[idx] = futures[idx].get();
        }
    }

    return r;
}
