#include "union_find.hpp"

void UnionFind::add(int v) noexcept
{
        if (!parent.contains(v)) {
                parent[v] = v;
        }
}

int UnionFind::find(int i) noexcept
{
        if (parent[i] == i) {
                return i;
        }
        return parent[i] = find(parent[i]);     // path compression
}

bool UnionFind::unite(int i, int j) noexcept
{
        int root_i = find(i);
        int root_j = find(j);

        if (root_i != root_j) {
                parent[root_i] = root_j;
                return true;
        }
        return false;
}
