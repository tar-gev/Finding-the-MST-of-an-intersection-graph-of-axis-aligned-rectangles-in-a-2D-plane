#ifndef GRAPH_VISUALIZER_HPP
#define GRAPH_VISUALIZER_HPP

#include "graph.hpp"

#include <QWidget>

// Օրիգինալ (սկզբնական) գրաֆի պատուհան, որտեղ կա "Գտնել ՄԿԾ" կոճակը
class GraphVisualizer : public QWidget
{
public:
        GraphVisualizer(const Graph& g, QWidget *parent = nullptr);

private:
        Graph graph;
};

#endif // GRAPH_VISUALIZER_HPP
