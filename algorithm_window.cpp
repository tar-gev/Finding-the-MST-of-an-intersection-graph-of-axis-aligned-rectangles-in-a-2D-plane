#include "algorithm_window.hpp"

#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
#include "graph_draw_widget.hpp"


AlgorithmWindow::
AlgorithmWindow(const Graph& g, const QString& algoName, const std::vector<Graph::AlgorithmStep>& algoSteps, QWidget *parent)
        : QWidget(parent)
        , steps(algoSteps)
{

        setWindowTitle("Ալգորիթմի վիզուալիզացիա - " + algoName);
        resize(1000, 600); // Պատուհանի չափերը

        QHBoxLayout* mainLayout = new QHBoxLayout(this); // Հորիզոնական բաժանում (ձախ և աջ)

        // ՁԱԽ ՄԱՍ. Լոգերի վահանակ
        textEdit = new QTextEdit(this);
        textEdit->setReadOnly(true); // Միայն կարդալու համար
        textEdit->setFont(QFont("Arial", 11));

        // ԱՋ ՄԱՍ. Գրաֆ և կոճակներ
        QVBoxLayout* rightLayout = new QVBoxLayout(); // Ուղղահայաց՝ նկար և կոճակներ տակը
        drawWidget = new GraphDrawWidget(g, this);

        // Կոճակների վահանակ տակը
        QHBoxLayout* btnLayout = new QHBoxLayout();
        hideBtn = new QPushButton("Թաքցնել չընտրված կողերը", this);
        showBtn = new QPushButton("Ցուցադրել բոլոր կողերը (Reset)", this);

        hideBtn->setFont(QFont("Arial", 10, QFont::Bold));
        showBtn->setFont(QFont("Arial", 10));

        // Սկզբնական վիճակում կոճակներն անջատված են (անիմացիայի ժամանակ սեղմել չի կարելի)
        hideBtn->setEnabled(false);
        showBtn->setEnabled(false);

        btnLayout->addWidget(hideBtn);
        btnLayout->addWidget(showBtn);

        rightLayout->addWidget(drawWidget, 1); // 1-ը հարաբերական չափն է (վերցնում է մաքս. տարածք)
        rightLayout->addLayout(btnLayout);     // Տակից կոճակները

        // Միացնում ենք ձախ և աջ մասերը (1:2 հարաբերակցությամբ)
        mainLayout->addWidget(textEdit, 1);
        mainLayout->addLayout(rightLayout, 2);

        // Կոճակների ազդանշանների (signal) միացում (slot)
        connect(hideBtn, &QPushButton::clicked, this, [this]() {
                drawWidget->setShowOriginalEdges(false); // Թաքցնում է
                hideBtn->setEnabled(false);              // Անջատում է այս կոճակը
                showBtn->setEnabled(true);               // Միացնում է մյուսը
        });

        connect(showBtn, &QPushButton::clicked, this, [this]() {
                drawWidget->setShowOriginalEdges(true);  // Ցույց է տալիս բոլորը
                showBtn->setEnabled(false);
                hideBtn->setEnabled(true);
        });

        // Տայմերի կարգավորում՝ անիմացիան քայլ-քայլ կատարելու համար
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &AlgorithmWindow::nextStep);
        timer->start(1200);  // 1.2 վայրկյան յուրաքանչյուր քայլի տևողությունը
}

void AlgorithmWindow::
nextStep()
{
        if (currentStep < steps.size()) {
                textEdit->append(steps[currentStep].description);  // Գրում ենք լոգը
                drawWidget->setMST(steps[currentStep].currentMST); // Թարմացնում ենք վիզուալը
                currentStep++; // Անցում հաջորդ քայլին
        } else {
                timer->stop(); // Եթե վերջացրեցինք, կանգնեցնում ենք
                hideBtn->setEnabled(true); // Ակտիվացնում ենք կոճակը
                textEdit->append("<br><span style='color:blue;'><b>✅ Ավարտված է:</b></span>");
        }
}

