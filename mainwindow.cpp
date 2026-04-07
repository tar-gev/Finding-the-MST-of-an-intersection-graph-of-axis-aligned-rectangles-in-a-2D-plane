#include "mainwindow.hpp"

#include "coordinate_plane.hpp"

#include <QVBoxLayout>
#include <QPushButton>

MainWindow::
MainWindow(QWidget *parent)
        : QWidget(parent)
{
        setWindowTitle("Սահող Գծի Ալգորիթմ (Event Queue + Active Set)");
        resize(800, 600);

        QVBoxLayout *layout = new QVBoxLayout(this); // Ստեղծում ենք ուղղահայաց դասավորություն
        CoordinatePlane *plane = new CoordinatePlane(this); // Ստեղծում ենք նկարչական դաշտը
        QPushButton *sweepButton = new QPushButton("Սկսել Սահող Գիծը", this); // Ստեղծում ենք գլխավոր կոճակը
        sweepButton->setFont(QFont("Arial", 12, QFont::Bold));
        sweepButton->setCursor(Qt::PointingHandCursor); // Փոխում ենք մկնիկի ցուցիչը

        layout->addWidget(plane);       // Ավելացնում ենք դաշտը վերևում
        layout->addWidget(sweepButton); // Ավելացնում ենք կոճակը ներքևում

        // Կապում ենք կոճակի "սեղմում" իրադարձությունը CoordinatePlane դասի startSweepLine ֆունկցիայի հետ
        connect(sweepButton, &QPushButton::clicked, plane, &CoordinatePlane::startSweepLine);
}
