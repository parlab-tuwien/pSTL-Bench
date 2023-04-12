
#ifndef MASTER_BENCHMARKS_B4_1_MERGE_CUTOFF_H
#define MASTER_BENCHMARKS_B4_1_MERGE_CUTOFF_H

#include <algorithm>
#include <vector>

namespace B4 {

    template<class ExecutionPolicy,
            typename base_policy = typename suite::base_type<ExecutionPolicy>>
    inline void
    b4_1_merge_cutoff(ExecutionPolicy &&policy,
                      const suite::int_vec<base_policy> &v1,
                      const suite::int_vec<base_policy> &v2,
                      suite::int_vec<base_policy> &result) {
        std::merge(policy, v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
    }

}


#endif //MASTER_BENCHMARKS_B4_1_MERGE_CUTOFF_H
