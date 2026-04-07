#ifndef COORDINATE_PLANE_HPP
#define COORDINATE_PLANE_HPP

#include "rectangle.hpp"
#include "point.hpp"
#include "graph.hpp"
#include "utilities.hpp"

#include <QWidget>

// ԳԼԽԱՎՈՐ ՄԱՍ. Սահող գծի վիզուալիզացիայի դաշտը (Sweep Line logic)
class CoordinatePlane : public QWidget
{
public:
        CoordinatePlane(QWidget *parent = nullptr);

        void startSweepLine() noexcept;         // Կանչվում է "Սկսել սահող գիծը" կոճակը սեղմելիս
protected:
        void paintEvent(QPaintEvent *event) override;           // Դաշտի նկարչության ֆունկցիան

        // Մկնիկի Իրադարձությունների Մշակում
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

private:
        Point screenToMath(const QPoint& screenPos);            // Փոխակերպում է Qt-ի (էկրանի) կոորդինատները (0,0-ն ձախ վերևն է) դեպի մաթեմատիկական (0,0-ն մեջտեղում է, Y-ը վերև է աճում)

        Rectangle normalizeRect(const Rectangle& r);            // Երաշխավորում է, որ bottomLeft-ը իրոք ներքևի ձախ կետն է (ավելի փոքր x և y), իսկ topRight-ը՝ վերևի աջը

        void drawRect(QPainter& painter, const Rectangle& r);           // Նկարում է ուղղանկյունը օգտագործելով QPainter

private:
        std::vector<Rectangle> rectangles; // Պահում է էկրանին գծված բոլոր ուղղանկյունները
        Rectangle currentRect;             // Պահում է ժամանակավոր ուղղանկյունը գծման պրոցեսում
        bool isDrawingRect = false;        // Դրոշակ՝ արդյոք հիմա մկնիկով գծում ենք

        // EVENT QUEUE (Սահող Գծի) տրամաբանության փոփոխականները
        std::vector<utilities::SweepEvent> events; // Իրադարձությունների հերթը (ձախ և աջ եզրեր)
        std::unordered_set<int> activeSet;        // Ակտիվ Բազմություն (Active Set - C++-ում հիմնված է Red-Black Tree-ի վրա, ապահովում է լոգարիթմական ժամանակ)
        size_t eventIndex = 0;          // Որ իրադարձությանն ենք հասել հերթում

        bool isSweeping = false; // Դրոշակ՝ սահող գիծը աշխատում է, թե ոչ
        double sweepX = 0.0;     // Գծի ընթացիկ X դիրքը վիզուալիզացիայի համար
        double minX = 0.0;       // Դաշտի ամենաձախ եզրը (որտեղից պետք է սկսել սահել)
        double maxX = 0.0;       // Դաշտի ամենաաջ եզրը (որտեղ պետք է ավարտել)
        QTimer* timer = nullptr; // Տայմեր անիմացիայի համար

        Graph graph; // Կառուցվող գրաֆը օբյեկտը
};


#endif // COORDINATE_PLANE_HPP
