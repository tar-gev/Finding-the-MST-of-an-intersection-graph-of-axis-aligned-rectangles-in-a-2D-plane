#ifndef ALGORITHM_WINDOW_HPP
#define ALGORITHM_WINDOW_HPP

#include <QWidget>
#include "graph.hpp"

class QTextEdit;
class GraphDrawWidget;
class QPushButton;


// Ալգորիթմի անիմացիայի և լոգերի գլխավոր պատուհանը
class AlgorithmWindow : public QWidget
{
public:
        AlgorithmWindow(const Graph& g, const QString& algoName, const std::vector<Graph::AlgorithmStep>& algoSteps, QWidget *parent = nullptr);

private:
        // Կանչվում է տայմերի կողմից ամեն 1.2 վայրկյանը մեկ
        void nextStep();

private:
        QTextEdit* textEdit;
        GraphDrawWidget* drawWidget;
        QPushButton* hideBtn;
        QPushButton* showBtn;
        std::vector<Graph::AlgorithmStep> steps; // Քայլերի ամբողջական զանգվածը
        int currentStep = 0; // Ընթացիկ ինդեքսը
        QTimer* timer;
};

#endif // ALGORITHM_WINDOW_HPP
