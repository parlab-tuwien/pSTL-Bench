#pragma once

#include <vector>
#include <cmath>
#include <algorithm>

template<class ExecutionPolicy>
inline void b1_for_each_linear(ExecutionPolicy &policy, const std::vector<int> &input_data) {

    // quite straight forward std::for_each. The body of the lambda has no intention behind it, the goal was just
    // to have a big enough computation that takes some amount of time.
    std::for_each(policy, input_data.begin(), input_data.end(), [](const auto &entry) {
        return std::min(std::sin(entry), std::tan(entry));
    });

}