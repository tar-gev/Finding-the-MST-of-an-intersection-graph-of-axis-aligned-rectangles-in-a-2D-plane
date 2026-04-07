#include "graph.hpp"
#include <QDebug>
#include <queue>

void Graph::addEdge(int u, int v, double weight, bool isDirected)
{
        adjList[u].insert({v, weight}); // ավելացնել u-ից v
        if (!isDirected) {
                adjList[v].insert({u, weight}); // եթե չուղղորդված է, ավելացնել նաև v-ից u
        }
}

void Graph::addVertex(int u)
{
        if (!adjList.contains(u)) {
                adjList[u] = {}; // ստեղծել դատարկ հարևանության ցուցակ
        }
}

void Graph::clear() noexcept
{
        adjList.clear();
}

void Graph::printGraph() const noexcept
{
        for (const auto& pair_item : adjList) {
                int node = pair_item.first;
                const auto& neighbors = pair_item.second;
                qDebug() << "vertex " << node << " -> ";
                for (const auto& edge : neighbors) {
                        int neighborNode = edge.first;
                        double edgeWeight = edge.second;
                        qDebug() << "[" << neighborNode << ", weight: " << edgeWeight << "] ";
                }
                qDebug() << "\n";
        }
}

const std::unordered_map<int, std::unordered_map<int, double>>&
Graph::getAdjList() const noexcept
{
        return adjList;
}

int Graph::getVertexCount() const noexcept
{
        return adjList.size();
}

int Graph::getEdgeCount() const noexcept
{
        int count = 0;
        for (const auto& pair : adjList) count += pair.second.size();
        return count / 2; // բաժանում ենք 2-ի, քանի որ ամեն կող պահված է 2 անգամ
}

std::pair<QString, std::vector<Graph::AlgorithmStep>>
Graph::getAlgorithmSteps() const noexcept
{
        int V = getVertexCount();
        int E = getEdgeCount();

        if (V <= 1) return {"Դատարկ", {}};

        // Եթե կողերի քանակը փոքր է V*(V-1)/8-ից՝ Կրուսկալ, հակառակ դեպքում՝ Պրիմ
        double threshold = (V * (V - 1)) / 8.0;

        if (E < threshold) {
                return {"Kruskal-ի Ալգորիթմ", getKruskalSteps()};
        } else {
                return {"Prim-ի Ալգորիթմ", getPrimSteps()};
        }
}

std::vector<Graph::AlgorithmStep>
Graph::getKruskalSteps() const noexcept
{
        std::vector<AlgorithmStep> steps; // Պահում է անիմացիայի քայլերը
        std::vector<EdgeData> edges;      // Բոլոր կողերի ցուցակը
        std::vector<EdgeData> currentMST; // Ակտիվ ՄԿԾ

        steps.push_back({"<b>[Քայլ 1]</b> Հավաքում ենք գրաֆի բոլոր կողերը...", currentMST});

        // Քաղում ենք բոլոր եզակի կողերը գրաֆից
        for (const auto& u_pair : adjList) {
                for (const auto& v_pair : u_pair.second) {
                        if (u_pair.first < v_pair.first) { // ապահովել եզակիություն
                                edges.push_back({u_pair.first, v_pair.first, v_pair.second});
                        }
                }
        }

        steps.push_back({QString("<b>[Քայլ 2]</b> Սկսում ենք սորտավորումը ըստ կշռի..."), currentMST});
        // Սորտավորում ենք կողերը ըստ կշռի աճման (Greedy ռազմավարություն)
        std::sort(edges.begin(), edges.end(), [](const EdgeData& a, const EdgeData& b) {
                return a.weight < b.weight;
        });

        // Disjoint Set Union (DSU) նախապատրաստում
        UnionFind dsu;
        for (const auto& pair : adjList) {
                dsu.add(pair.first); // Ավելացնում ենք գագաթները DSU-ի մեջ
        }

        steps.push_back({"<b>[Քայլ 3]</b> Սկսում ենք ընտրությունը...<br>", currentMST});

        // Անցնում ենք սորտավորված կողերով
        for (const auto& edge : edges) {
                QString log = QString("Դիտարկում ենք կողը՝ <b>%1 - %2</b> (կշիռ: %3).<br>").arg(edge.u).arg(edge.v).arg(edge.weight);

                if (dsu.unite(edge.u, edge.v)) {
                        currentMST.push_back(edge);
                        log += "<span style='color:green;'>&nbsp;&nbsp;➔ Ցիկլ չի առաջանում: Ավելացվում է ՄԿԾ-ի մեջ:</span><br>";
                } else {
                        log += "<span style='color:red;'>&nbsp;&nbsp;➔ Առաջացնում է ցիկլ: Անտեսվում է:</span><br>";
                }
                steps.push_back({log, currentMST}); // Պահպանում ենք քայլը վիզուալիզացիայի համար
        }
        steps.push_back({"<br><b>Ալգորիթմն ավարտվեց:</b>", currentMST});
        return steps;
}

std::vector<Graph::AlgorithmStep>
Graph::getPrimSteps() const noexcept
{
        std::vector<AlgorithmStep> steps;
        std::vector<EdgeData> currentMST;
        std::unordered_map<int, bool> visited; // Այցելված գագաթներ
        for (const auto& pair : adjList) visited[pair.first] = false;

        steps.push_back({"<b>[Քայլ 1]</b> Սկզբնավորում ենք PQ (Առաջնահերթության հերթ)...", currentMST});

        // Priority Queue՝ տարրերը պահվում են սորտավորված ըստ կշռի դեպի պակաս
        using P = std::pair<double, std::pair<int, int>>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

        // Քանի որ գրաֆը կարող է լինել անջատ (Forest), փորձում ենք բոլոր չայցելված գագաթներից
        for (const auto& start_node : adjList) {
                int start = start_node.first;
                if (visited[start]) continue;

                visited[start] = true; // Նշում ենք սկզբնական գագաթը որպես այցելված
                QString startLog = QString("<b>[Նոր Կոմպոնենտ]</b> Սկսում ենք <b>%1</b> գագաթից...").arg(start);
                steps.push_back({startLog, currentMST});

                // Սկզբնական գագաթի հարևաններին լցնում ենք հերթի մեջ
                for (const auto& edge : start_node.second) {
                        pq.push({edge.second, {start, edge.first}});
                }

                // Քանի դեռ հերթը դատարկ չէ
                while (!pq.empty()) {
                        auto top = pq.top(); // վերցնում ենք ամենափոքր կշռով կողը
                        pq.pop();

                        double weight = top.first;
                        int u = top.second.first;
                        int v = top.second.second;

                        QString log = QString("Հերթից հանում ենք ամենափոքրը՝ <b>%1 - %2</b> (կշիռ: %3).<br>").arg(u).arg(v).arg(weight);

                        if (visited[v]) { // Եթե գագաթն արդեն ՄԿԾ-ում է՝ անտեսում ենք (ցիկլից խուսափելու համար)
                                log += "<span style='color:red;'>&nbsp;&nbsp;➔ Մյուս գագաթն արդեն այցելված է:</span><br>";
                        } else {
                                visited[v] = true; // Նշում ենք որպես այցելված
                                currentMST.push_back({u, v, weight}); // Ավելացնում ենք ՄԿԾ-ի մեջ
                                log += "<span style='color:green;'>&nbsp;&nbsp;➔ Գագաթը նոր է: Ավելացվում է ՄԿԾ:</span><br>";

                                // Նոր գագաթի չայցելված հարևաններին լցնում ենք հերթի մեջ
                                for (const auto& edge : adjList.at(v)) {
                                        if (!visited[edge.first]) {
                                                pq.push({edge.second, {v, edge.first}});
                                        }
                                }
                        }
                        steps.push_back({log, currentMST});
                }
        }
        steps.push_back({"<br><b>Ալգորիթմն ավարտվեց:</b>", currentMST});
        return steps;
}
