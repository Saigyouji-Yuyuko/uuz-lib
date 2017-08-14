cmake_minimum_required(VERSION 3.8)
project(Project2)

set(CMAKE_CXX_STANDARD 17)


set(SOURCE_FILES
        Project2/Container/allocator.h
        Project2/Container/array.h
        Project2/Container/container.h
        Project2/Container/deque.h
        Project2/Container/forward_list.h
        Project2/Container/list.h
        Project2/Container/map.h
        Project2/Container/pair.h
        Project2/Container/priority_queue.h
        Project2/Container/rbtree.h
        Project2/Container/set.h
        Project2/Container/string.h
        Project2/Container/vector.h
        Project2/Release/main.i
        Project2/test/debug.h
        Project2/test/test.h
        Project2/algorithm.h
        Project2/iterator.h
        Project2/main.cpp
        Project2/prepare.h
        Project2/ptr.h)

add_executable(Project2 ${SOURCE_FILES})