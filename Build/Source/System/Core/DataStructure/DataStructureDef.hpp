#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <algorithm>

namespace Engine5
{
    template <typename T>
    using Vector = std::vector;

    template <typename T>
    using List = std::list;

    template <typename T>
    using HashMap = std::unordered_map;

    template <typename T>
    using MHashMap = std::unordered_multimap;

    template <typename T>
    using TreeMap = std::map;

    template <typename T>
    using MMap = std::multimap;

    template <typename T>
    using Set = std::set;

    template <typename T>
    using MSet = std::multiset;

    template <typename T>
    using HashSet = std::unordered_set;

    template <typename T>
    using MHashSet = std::unordered_multiset;
}
