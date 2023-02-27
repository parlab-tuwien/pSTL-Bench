
#ifndef MASTER_BENCHMARKS_B9_2_TRANSFORM_OLD_IOTA_H
#define MASTER_BENCHMARKS_B9_2_TRANSFORM_OLD_IOTA_H

#include <vector>
#include <algorithm>
#include <numeric>

namespace B9 {

    template<class ExecutionPolicy>
    inline void
    b9_2_transform_old_iota(ExecutionPolicy &&policy,
                            const std::vector<int> &input_data,
                            std::vector<int> &res) {

        std::vector<std::size_t> view(input_data.size());
        std::iota(view.begin(), view.end(), 0); // sadly this cannot be done with parallel stl

        std::transform(policy, view.begin(), view.end(), res.begin(), [&](const auto &index) {
            return input_data[index] + 10;
        });

    }

}

#endif //MASTER_BENCHMARKS_B9_2_TRANSFORM_OLD_IOTA_H