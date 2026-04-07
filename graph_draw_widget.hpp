#ifndef GRAPH_DRAW_WIDGET_HPP
#define GRAPH_DRAW_WIDGET_HPP

#include "graph.hpp"

#include <vector>
#include <QWidget>

// Վիջեթ միայն գրաֆի վիզուալիզացիայի համար (օգտագործվում է AlgorithmWindow-ում և GraphVisualizer-ում)
class GraphDrawWidget : public QWidget
{
public:
        GraphDrawWidget(const Graph& g, QWidget *parent = nullptr);             // Կոնստրուկտոր՝ ընդունում է գրաֆը և պահպանում

        void setMST(const std::vector<Graph::EdgeData>& mst);           // Սահմանում է ընթացիկ ՄԿԾ կողերը (անիմացիայի ժամանակ թարմացվելու համար)
        void setShowOriginalEdges(bool show);           // Միացնում/Անջատում է օրիգինալ գրաֆի մոխրագույն կողերի ցուցադրումը

protected:
        void paintEvent(QPaintEvent *event) override;           // QWidget-ի նկարչության գլխավոր իրադարձությունը
private:
        Graph graph;                             // Նկարվող գրաֆը
        std::vector<Graph::EdgeData> currentMST; // Անիմացիայի ընթացիկ վիճակը (ակտիվ կողերը)
        bool showOriginalEdges;                  // Դրոշակ՝ բոլոր կողերը ցույց տալու համար
};

#endif // GRAPH_DRAW_WIDGET_HPP
