# Graph

Graph algorithms by Kyle Dukart

GitHub URL https://github.com/2018WI343/graph-kyledukart

Expected Mark: 70

Time spent: ~20 hours

Compiled and tested on CSS Linux Lab: Yes

> cpplint *.cpp *.h

> cppcheck --enable=all --force --inconclusive --language=c++ --std=posix --suppress=missingIncludeSystem *.cpp *.h

 int Graph::MST(const string &startLabel,
                              ^
graph.cpp:251:55: warning: unused parameter ‘visit’ [-Wunused-parameter]
                           const string &to, int weight)) const {

These parameters are for MST which is not implemented.

[graph.cpp:248] -> [graph.h:129]: (performance, inconclusive) Technically the member function 'Graph::MST' can be static.
MST is not implemented.

[graph.cpp:138] -> [graph.cpp:137]: (error) Iterator 'it' used after element has been erased.
I do not see a problem with this, unsure how to fix.

[graph.cpp:248]: (style) The function 'MST' is never used.
[graph_test.cpp:44]: (style) The function 'edgeVisitor' is never used.
[graph_test.cpp:112]: (style) The function 'testGraph0DFS' is never used.
[graph_test.cpp:174]: (style) The function 'testGraph0Dijkstra' is never used.
[graph_test.cpp:205]: (style) The function 'testGraph1' is never used.
unused testing functions

> valgrind ./bst.exe

definitely lost: 0 bytes in 0 blocks

Implemented most functions, but did not have time for MST, proper testing