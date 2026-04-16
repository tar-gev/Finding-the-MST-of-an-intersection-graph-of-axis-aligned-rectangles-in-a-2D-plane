#include "utilities.hpp"

#include <QFont>
#include <QFontMetrics>
#include <QRectF>
#include <QColor>
#include <QPen>

namespace utilities {

void drawEdgeWeightLabel(QPainter& painter, QPointF p1, QPointF p2, double weight) {
        QPointF mid((p1.x() + p2.x()) / 2.0, (p1.y() + p2.y()) / 2.0); // Գտնում ենք կողի մեջտեղը
        QString text = QString::number(weight);                    // Թիվը վերածում ենք տեքստի

        // Տառատեսակի կարգավորում
        QFont font = painter.font();
        font.setPointSize(10);
        font.setBold(true);
        painter.setFont(font);

        // Հաշվում ենք տեքստի զբաղեցրած պիքսելային չափերը
        QFontMetrics fm(font);
        int textWidth = fm.horizontalAdvance(text);
        int textHeight = fm.height();

        // Ստեղծում ենք ֆոնի ուղղանկյունը տեքստի շուրջ
        QRectF bgRect(mid.x() - textWidth / 2.0 - 4, mid.y() - textHeight / 2.0 - 2, textWidth + 8, textHeight + 4);

        // Նկարում ենք կիսաթափանցիկ սպիտակ ֆոն կարմիր եզրագծով
        painter.setBrush(QColor(255, 255, 255, 220));
        painter.setPen(QPen(Qt::darkRed, 1));
        painter.drawRoundedRect(bgRect, 3, 3); // 3x3 կլորացված անկյուններ

        // Նկարում ենք բուն տեքստը կենտրոնում
        painter.setPen(Qt::red);
        painter.drawText(bgRect, Qt::AlignCenter, text);
}

double roundToOneDecimal(double value) noexcept {
        return std::round(value * 10.0) / 10.0;
}

} // namespace utilities
