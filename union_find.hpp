#ifndef UNION_FIND_HPP
#define UNION_FIND_HPP

#include <unordered_map>

class UnionFind {
private:
        std::unordered_map<int, int> parent;

public:
        void add(int v) noexcept;       // Ավելացնում է նոր գագաթ (make_set), եթե այն դեռ չկա
        int find(int i) noexcept;       // Գտնում է արմատը՝ կիրառելով ուղու սեղմում (path compression)

        // Միավորում է երկու բազմությունները (union)
        // Վերադարձնում է true, եթե հաջողվեց միավորել (այսինքն՝ տարբեր բազմություններ էին)
        bool unite(int i, int j) noexcept;
};

#endif // UNION_FIND_HPP
