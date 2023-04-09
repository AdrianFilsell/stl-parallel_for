# stl-parallel_for
parallel_for implemention using STL
c++/stl implementation of parallel_for. Intel TBB version 4/update 4 has issues with c++ v20/v23. Looked into using oneTBB but wanted a static lib version which was not available ( could probably build github oneTBB codebase into a static lib ). Therefore wrote my own parallel_for implementation, this out performed Intel and Parallel Patterns Library parallel_for implementations, more details can be found here:

https://www.youtube.com/watch?v=PITHyAhm8lo
