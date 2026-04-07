#include "graph_visualizer.hpp"

#include "graph_draw_widget.hpp"
#include "algorithm_window.hpp"

#include <QVBoxLayout>
#include <QPushButton>


GraphVisualizer::
GraphVisualizer(const Graph& g, QWidget *parent)
        : QWidget(parent)
        , graph(g)
{
        setWindowTitle("Կառուցված Գրաֆ (Սկզբնական)");
        resize(600, 600);

        QVBoxLayout *layout = new QVBoxLayout(this);

        GraphDrawWidget* originalDraw = new GraphDrawWidget(g, this); // Նկարում է գրաֆը
        layout->addWidget(originalDraw, 1);

        QPushButton *mstButton = new QPushButton("Մինիմալ կմախքային ծառ գտնել", this); // Ալգորիթմը սկսող կոճակ
        mstButton->setFont(QFont("Arial", 11, QFont::Bold));
        layout->addWidget(mstButton, 0, Qt::AlignCenter);

        connect(mstButton, &QPushButton::clicked, this, [this]() {
                // Ստանում ենք քայլերը Graph օբյեկտից
                auto result = graph.getAlgorithmSteps();
                QString algoName = result.first;
                auto steps = result.second;

                // Բացում ենք նոր պատուհանը (Անիմացիոն պատուհանը)
                AlgorithmWindow* algoWindow = new AlgorithmWindow(graph, algoName, steps);
                algoWindow->setAttribute(Qt::WA_DeleteOnClose); // Հիշողությունը ազատվում է փակելիս
                algoWindow->show();
        });
}
