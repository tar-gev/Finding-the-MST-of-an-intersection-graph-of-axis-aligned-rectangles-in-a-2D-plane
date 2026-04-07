#include "graph_draw_widget.hpp"

#include "utilities.hpp"

#include <QPainter>

GraphDrawWidget::
GraphDrawWidget(const Graph& g, QWidget *parent)
        : QWidget(parent)
        , graph(g)
        , showOriginalEdges(true)
{}

void GraphDrawWidget::
setMST(const std::vector<Graph::EdgeData>& mst)
{
        currentMST = mst;
        update(); // Հրահանգում է նորից կանչել paintEvent-ը
}

void GraphDrawWidget::
setShowOriginalEdges(bool show) {
        showOriginalEdges = show;
        update();
}

void GraphDrawWidget::
paintEvent(QPaintEvent *event)
{
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // Հարթեցնում է գծերը (Anti-aliasing)

        auto adjList = graph.getAdjList();
        if (adjList.empty()) return;

        int w = width();
        int h = height();
        int radius = std::min(w, h) / 2 - 40; // Գագաթները շարելու շրջանագծի շառավիղը
        QPointF center(w / 2.0, h / 2.0);     // Կենտրոնական կետը

        // Հաշվում ենք յուրաքանչյուր գագաթի դիրքը շրջանագծի վրա (շրջանաձև դասավորվածություն)
        std::map<int, QPointF> nodePositions;
        int n = adjList.size();
        int i = 0;
        for (const auto& pair : adjList) {
                int node = pair.first;
                double angle = 2.0 * 3.1415926535 * i / n; // Անկյունը ռադիաններով
                double x = center.x() + radius * std::cos(angle);
                double y = center.y() + radius * std::sin(angle);
                nodePositions[node] = QPointF(x, y);
                i++;
        }

        // 1. Օրիգինալ գրաֆի նկարչություն (մոխրագույն կողերով)
        if (showOriginalEdges) {
                painter.setPen(QPen(Qt::lightGray, 2));
                std::set<std::pair<int, int>> drawnEdges; // Պահում ենք նկարված կողերը՝ կրկնությունից խուսափելու համար

                for (const auto& pair : adjList) {
                        int u = pair.first;
                        for (const auto& edge : pair.second) {
                                int v = edge.first;
                                double weight = edge.second;

                                int minNode = std::min(u, v);
                                int maxNode = std::max(u, v);

                                if (drawnEdges.find({minNode, maxNode}) == drawnEdges.end()) {
                                        drawnEdges.insert({minNode, maxNode});
                                        QPointF p1 = nodePositions[u];
                                        QPointF p2 = nodePositions[v];

                                        painter.setPen(QPen(Qt::lightGray, 2));
                                        painter.drawLine(p1, p2); // Գծում ենք կողը
                                        utilities::drawEdgeWeightLabel(painter, p1, p2, weight); // Նկարում ենք կշիռը
                                }
                        }
                }
        }

        // 2. ՄԿԾ (ակտիվ) կողերի նկարչություն (կարմիր և հաստ)
        for (const auto& edge : currentMST) {
                QPointF p1 = nodePositions[edge.u];
                QPointF p2 = nodePositions[edge.v];

                painter.setPen(QPen(Qt::red, 4)); // Հաստությունը 4
                painter.drawLine(p1, p2);
                utilities::drawEdgeWeightLabel(painter, p1, p2, edge.weight);
        }

        // 3. Գագաթների նկարչություն (կողերի վրայից, դեղին շրջաններով)
        for (const auto& pair : nodePositions) {
                int node = pair.first;
                QPointF pos = pair.second;

                painter.setBrush(Qt::yellow);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawEllipse(pos, 16, 16); // 16 շառավղով շրջան

                painter.setPen(Qt::black);
                QFont f = painter.font(); f.setPointSize(9); f.setBold(true); painter.setFont(f);
                painter.drawText(QRectF(pos.x() - 16, pos.y() - 16, 32, 32), Qt::AlignCenter, QString::number(node)); // ID-ն մեջտեղում
        }
}
