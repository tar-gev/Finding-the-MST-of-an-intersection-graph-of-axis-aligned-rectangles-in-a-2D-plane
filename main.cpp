#include "mainwindow.hpp"

#include <QApplication>   // Qt հավելվածի գլխավոր դասը

// C++ ծրագրի մուտքի կետը
int main(int argc, char *argv[]) {
        QApplication app(argc, argv); // Ստեղծում է Qt հավելվածի օբյեկտը

        MainWindow window; // Ստեղծում է գլխավոր պատուհանը
        window.show();     // Ցուցադրում է էկրանին

        return app.exec(); // Սկսում է Qt-ի իրադարձությունների մշակման ցիկլը (Event Loop)
}
