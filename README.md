# stl-parallel_for
parallel_for implemention using STL
c++/stl implementation of parallel_for. Intel TBB version 4/update 4 has issues with c++ v20/v23. Intel oneTBB does not have a static lib version ( could probably build github oneTBB codebase into a static lib ), therefore wrote my own parallel_for implementation. This out performed Intel and Parallel Patterns Library parallel_for implementations for my case usage, more details can be found here:

https://www.youtube.com/watch?v=PITHyAhm8lo
