#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "union_find.hpp"

#include <vector>
#include <unordered_map>
#include <QString>
#include <utility> // std::pair-ի համար

// Գրաֆի տվյալների կառույց և ալգորիթմներ (MST)
class Graph
{
public:
        // Կողի տվյալներ պահող ստրուկտուրա
        struct EdgeData { int u, v; double weight; };

        // Ալգորիթմի քայլի նկարագրության և ընթացիկ վիճակի ստրուկտուրա
        struct AlgorithmStep {
                QString description;          // Ինչ է կատարվում տվյալ քայլում (տեքստ)
                std::vector<EdgeData> currentMST; // ՄԿԾ-ի (MST) կողերը տվյալ պահին
        };

        void addEdge(int u, int v, double weight, bool isDirected = false);    // Ավելացնում է նոր կող գրաֆում (լռելյայն՝ չուղղորդված)
        void addVertex(int u);  // Ավելացնում է նոր, դատարկ գագաթ
        void clear() noexcept;  // Մաքրում է ամբողջ գրաֆը

        const std::unordered_map<int, std::unordered_map<int, double>>& getAdjList() const noexcept;        // Վերադարձնում է հարևանության ցուցակը (միայն կարդալու համար)
        int getVertexCount() const noexcept;        // Վերադարձնում է գագաթների ընդհանուր քանակը
        int getEdgeCount() const noexcept;      // Վերադարձնում է կողերի ընդհանուր քանակը

        std::pair<QString, std::vector<AlgorithmStep>> getAlgorithmSteps() const noexcept;      // Ընտրում և աշխատացնում է ճիշտ ալգորիթմը ՄԿԾ-ի համար (Kruskal կամ Prim)

        void printGraph() const noexcept;       // Տպում է գրաֆի կառույցը կոնսոլում դեբագի համար
private:
        std::vector<AlgorithmStep> getKruskalSteps() const noexcept;    // Kruskal-ի ալգորիթմի իրականացում՝ քայլերի գրանցումով
        std::vector<AlgorithmStep> getPrimSteps() const noexcept;       // Prim-ի ալգորիթմի իրականացում՝ քայլերի գրանցումով

private:
        // Հարևանության ցուցակ. Արտաքին քարտեզը՝ գագաթներն են, ներքինը՝ հարևանները և կշիռները
        std::unordered_map<int, std::unordered_map<int, double>> adjList;
};


#endif // GRAPH_HPP
