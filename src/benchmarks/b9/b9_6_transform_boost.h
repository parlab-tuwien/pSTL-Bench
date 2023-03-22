
#ifndef MASTER_BENCHMARKS_B9_6_TRANSFORM_BOOST_H
#define MASTER_BENCHMARKS_B9_6_TRANSFORM_BOOST_H

#ifndef SKIP_BOOST

#include <boost/iterator/counting_iterator.hpp>
#include <vector>
#include <algorithm>

namespace B9 {

    template<class ExecutionPolicy>
    inline void
    b9_6_transform_boost(ExecutionPolicy &&policy,
                         const std::vector<int> &input_data,
                         std::vector<int> &res) {

        std::transform(policy,
                       boost::counting_iterator<int>(0),
                       boost::counting_iterator<int>(input_data.size()),
                       res.begin(), [&](const auto &index) {
                    return input_data[index] + 10;
                });

    }

}

#endif

#endif //MASTER_BENCHMARKS_B9_6_TRANSFORM_BOOST_H
