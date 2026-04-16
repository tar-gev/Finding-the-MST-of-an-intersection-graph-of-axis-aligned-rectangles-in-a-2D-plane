#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <QPainter>
#include <QPointF>
#include <QString>
#include <cmath>

namespace utilities {

        // Իրադարձության տեսակներ Սահող Գծի ալգորիթմի համար
        enum EventType {
                LEFT_EDGE = 0, // Ձախ եզր՝ նշանակում է ուղղանկյունը մտնում է ակտիվ գոտի
                RIGHT_EDGE = 1 // Աջ եզր՝ նշանակում է ուղղանկյունը լքում է ակտիվ գոտին
        };

        // Սահող Գծի իրադարձության ստրուկտուրա
        struct SweepEvent {
                double x;       // Իրադարձության X կոորդինատը
                EventType type; // Իրադարձության տեսակը (ձախ թե աջ)
                int rectId;     // Ուղղանկյան նույնացուցիչը (ID) զանգվածում

                // Սորտավորման կանոն իրադարձությունների հերթի համար
                bool operator<(const SweepEvent& other) const {
                        if (x == other.x)
                                return type < other.type; // Եթե X-երը համընկնում են, նախ մշակել ձախ եզրերը (0 < 1)
                        return x < other.x; // Հիմնականում սորտավորել ըստ X կոորդինատի աճման
                }
        };

        void drawEdgeWeightLabel(QPainter& painter, QPointF p1, QPointF p2, double weight);     // Օժանդակ ֆունկցիա՝ կողի կշիռը գեղեցիկ ֆոնով նկարելու համար
        double roundToOneDecimal(double value) noexcept;        // Թվի կլորացում մինչև տասնորդական մաս

} // namespace utilities

#endif // UTILITIES_HPP
