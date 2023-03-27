
#ifndef MASTER_BENCHMARKS_B4_4_SET_DIFFERENCE_CUTOFF_H
#define MASTER_BENCHMARKS_B4_4_SET_DIFFERENCE_CUTOFF_H

#include <algorithm>
#include <vector>

namespace B4 {

    template<class ExecutionPolicy>
    inline void
    b4_4_set_difference_cutoff(ExecutionPolicy &&policy,
                               const std::vector<int> &v1,
                               const std::vector<int> &v2,
                               std::vector<int> &result) {
        std::set_difference(policy, v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
    }

}


#endif //MASTER_BENCHMARKS_B4_4_SET_DIFFERENCE_CUTOFF_H
