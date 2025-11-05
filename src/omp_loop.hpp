#ifndef OMP_LOOP_HPP
#define OMP_LOOP_HPP

#include <omp.h>
#include <functional>
#include <cstddef>

class OmpLoop {
public:
    static int nbThread;
    static int granularity;

    static void setNbThread(int n) { nbThread = (n > 0 ? n : 1); }
    static void setGranularity(int g) { granularity = (g > 0 ? g : 1); }

    // parfor with size_t indices and a lambda taking size_t
    static void parfor(std::size_t start, std::size_t end, const std::function<void(std::size_t)>& f) {
        // OpenMP requires a signed loop variable; use long long
        #pragma omp parallel for num_threads(nbThread) schedule(static, granularity)
        for (long long ii = static_cast<long long>(start); ii < static_cast<long long>(end); ++ii) {
            f(static_cast<std::size_t>(ii));
        }
    }
};

// defaults
int OmpLoop::nbThread = 1;
int OmpLoop::granularity = 1;

#endif // OMP_LOOP_HPP
