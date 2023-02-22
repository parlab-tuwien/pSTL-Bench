
#ifndef MASTER_BENCHMARKS_B6_2_EXCLUSIVE_SCAN_H
#define MASTER_BENCHMARKS_B6_2_EXCLUSIVE_SCAN_H

#include <numeric>
#include <vector>
#include <utility>

template<class ExecutionPolicy>
inline std::vector<int>
b6_2_exclusive_scan(ExecutionPolicy &&policy, const std::vector<int> &container) {

    std::vector<int> result(container.size());

    std::exclusive_scan(policy, container.begin(), container.end(), result.begin(), 0);

    return result;
}

#endif //MASTER_BENCHMARKS_B6_2_EXCLUSIVE_SCAN_H