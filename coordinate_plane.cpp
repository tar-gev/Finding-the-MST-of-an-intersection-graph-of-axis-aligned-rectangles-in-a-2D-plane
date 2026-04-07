#include "coordinate_plane.hpp"

#include "utilities.hpp"
#include "graph_visualizer.hpp"

#include <QTimer>
#include <QMouseEvent>

CoordinatePlane::
CoordinatePlane(QWidget *parent)
        : QWidget(parent)
{
        setMinimumSize(800, 500); // Վիջեթի մինիմալ չափեր
}

void CoordinatePlane::
startSweepLine() noexcept
{
        if (rectangles.empty()) return; // Եթե ուղղանկյուններ չկան, ոչինչ չանել

        graph.clear();
        events.clear();
        activeSet.clear();

        minX = rectangles[0].bottomLeft.x;
        maxX = rectangles[0].topRight.x;

        // 1. Ստեղծում ենք Իրադարձությունների հերթը (Event Queue)
        for (size_t i = 0; i < rectangles.size(); ++i) {
                // Որոշում ենք գծի շարժման մինիմալ և մաքսիմալ կոորդինատները
                minX = std::min(minX, rectangles[i].bottomLeft.x);
                maxX = std::max(maxX, rectangles[i].topRight.x);

                // Ամեն ուղղանկյան համար ստեղծում ենք 2 իրադարձություն (ձախ և աջ եզրեր)
                events.push_back({rectangles[i].bottomLeft.x, utilities::LEFT_EDGE, static_cast<int>(i)});
                events.push_back({rectangles[i].topRight.x, utilities::RIGHT_EDGE, static_cast<int>(i)});
        }

        minX -= 50; // Սկսում ենք մի քիչ ավելի ձախից (վիզուալ գեղեցկության համար)
        maxX += 50; // Ավարտում ենք մի քիչ ավելի աջ

        // 2. Սորտավորում ենք իրադարձությունները (O(N log N) ժամանակային բարդություն)
        std::sort(events.begin(), events.end());

        sweepX = minX;   // Գծի սկզբնական X դիրքը
        eventIndex = 0;  // Որ իրադարձությունն ենք հիմա սպասում
        isSweeping = true; // Ակտիվացնում ենք անիմացիան

        // Ստեղծում ենք տայմերը վիզուալ անիմացիայի և ալգորիթմի համաժամանակյա աշխատանքի համար
        if (!timer) {
                timer = new QTimer(this);
                connect(timer, &QTimer::timeout, this, [this]() {
                        // ՎԻԶՈՒԱԼ ՄԱՍ: Գիծը առաջ է գնում սահուն (ամեն կադրին 2.0 պիքսելով)
                        sweepX += 2.0;

                        // ՏՐԱՄԱԲԱՆՈՒԹՅՈՒՆ. Մշակում ենք այն իրադարձությունները, որոնց վրայով գիծն արդեն անցել է
                        // Սա երաշխավորում է, որ ալգորիթմը աշխատում է Event-ների հիման վրա, բայց վիզուալը սահուն է
                        while (eventIndex < events.size() && events[eventIndex].x <= sweepX) {
                                utilities::SweepEvent ev = events[eventIndex];
                                int currentId = ev.rectId;
                                Rectangle currentRect = rectangles[currentId];

                                if (ev.type == utilities::LEFT_EDGE) {
                                        // Նոր ուղղանկյուն հանդիպեց: Ավելացնում ենք որպես գագաթ գրաֆում:
                                        graph.addVertex(currentId);

                                        // Որոնում ենք հատումներ ՄԻԱՅՆ Ակտիվ Բազմությունում (Active Set)
                                        // C++-ի std::unordered_set-ը ապահովում է Active Set-ի ~O(1) մուտքագրում/հեռացում
                                        for (int activeId : activeSet) {
                                                Rectangle activeRect = rectangles[activeId];
                                                // // անկյունագծերի հատման կետերի հեռավորությամբ
                                                // // Ստուգում ենք հատումը ըստ Y առանցքի (X հատումը արդեն երաշխավորված է Active Set-ով)
                                                // double bottomY = std::max(currentRect.bottomLeft.y, activeRect.bottomLeft.y);
                                                // double topY = std::min(currentRect.topRight.y, activeRect.topRight.y);

                                                // if (topY - bottomY >= 0) { // Եթե հատվում են Y առանցքի վրա (կա հատման մակերես կամ գիծ)
                                                //     // Գտնում ենք երկու ուղղանկյունների կենտրոնները՝ քաշը հաշվելու համար
                                                //     double cx1 = (currentRect.bottomLeft.x + currentRect.topRight.x) / 2.0;
                                                //     double cy1 = (currentRect.bottomLeft.y + currentRect.topRight.y) / 2.0;
                                                //     double cx2 = (activeRect.bottomLeft.x + activeRect.topRight.x) / 2.0;
                                                //     double cy2 = (activeRect.bottomLeft.y + activeRect.topRight.y) / 2.0;

                                                //     // Կշռի հաշվարկ (Էվկլիդեսյան հեռավորություն կենտրոնների միջև)
                                                //     double weight = roundToOneDecimal(static_cast<int>(std::round(std::sqrt(std::pow(cx1 - cx2, 2) + std::pow(cy1 - cy2, 2)))) / 50.0);
                                                //     graph.addEdge(currentId, activeId, weight); // Ավելացնում ենք կողը գրաֆում
                                                // }


                                                // հատման մակերեսով (MST-ի մեջ լինելու են հատման փոքր մակերեսները,
                                                // եթե պետքա առավելույուն տա լ մեծ հատումներին , ապա պետքա շրջել թվերը՝ օրինակ -weight կամ 1000/weight)
                                                // Ստուգում ենք հատումը ըստ Y առանցքի
                                                double bottomY = std::max(currentRect.bottomLeft.y, activeRect.bottomLeft.y);
                                                double topY = std::min(currentRect.topRight.y, activeRect.topRight.y);

                                                if (topY - bottomY > 0) { // Եթե հատվում են Y առանցքով (այսինքն՝ ունեն հատման մակերես)

                                                        // Հաշվում ենք հատման լայնությունը (X առանցքով)
                                                        double leftX = std::max(currentRect.bottomLeft.x, activeRect.bottomLeft.x);
                                                        double rightX = std::min(currentRect.topRight.x, activeRect.topRight.x);

                                                        double intersectionWidth = rightX - leftX;
                                                        double intersectionHeight = topY - bottomY;

                                                        // Հաշվում ենք հատման մակերեսը
                                                        double area = intersectionWidth * intersectionHeight;

                                                        // Որպես կշիռ վերցնում ենք մակերեսը:
                                                        // Բաժանում ենք 250.0-ի (կամ մեկ այլ թվի), որպեսզի վիզուալիզացիայի վրա թվերը շատ մեծ չստացվեն
                                                        double weight = utilities::roundToOneDecimal(static_cast<int>(std::round(area)) / 250.0);

                                                        // Ապահովագրում ենք, որ զրոյական կշիռ չունենանք (եթե շատ փոքր է հատումը)
                                                        if (weight <= 0) weight = 0.1;

                                                        graph.addEdge(currentId, activeId, weight); // Ավելացնում ենք կողը գրաֆում
                                                }
                                        }

                                        // Տեղադրում ենք նոր ուղղանկյունը ակտիվ բազմության մեջ (դառնում է "ակտիվ")
                                        activeSet.insert(currentId);

                                } else if (ev.type == utilities::RIGHT_EDGE) {
                                        // Ուղղանկյանը անցանք: Հեռացնում ենք այն ակտիվ բազմությունից (այլևս երբեք չի հատվի նորերի հետ)
                                        activeSet.erase(currentId);
                                }

                                eventIndex++; // Անցնում ենք հաջորդ իրադարձությանը
                        }

                        // Եթե գիծը հասել է վերջին կոորդինատին
                        if (sweepX > maxX) {
                                timer->stop(); // Կանգնեցնում ենք տայմերը
                                isSweeping = false;

                                // Բացում ենք Գրաֆի Վիզուալիզացիայի պատուհանը հաշվարկված գրաֆով
                                GraphVisualizer* graphWindow = new GraphVisualizer(graph);
                                graphWindow->setAttribute(Qt::WA_DeleteOnClose);
                                graphWindow->show();
                        }
                        update(); // Հրահանգում ենք վերանկարել դաշտը (paintEvent)
                });
        }
        // Սկսում ենք տայմերը: 16 մվ ապահովում է մոտ ~60 FPS (1000/16 = 62.5) վիզուալի համար
        timer->start(16);
}

void CoordinatePlane::
paintEvent(QPaintEvent *event)
{
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // Հարթեցում

        int w = width();
        int h = height();

        painter.translate(w / 2, h / 2); // Տեղափոխում ենք կոորդինատների սկզբնակետը մեջտեղ
        painter.scale(1, -1);            // Շրջում ենք Y առանցքը, որ վերևը դառնա դրական ուղղություն (մաթեմատիկական հարթություն)

        // --- Կոորդինատային ցանցի նկարչություն ---
        painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
        QFont font = painter.font(); font.setPointSize(8); painter.setFont(font);

        int step = 50; int tickSize = 4; // Վանդակների քայլը և նշագծերի (ticks) չափը
        const auto drawTick = [&step, &tickSize, &painter] (int coordinate, bool isXAxis)
        {
                painter.setPen(QPen(Qt::black, 2));

                // Նկարում ենք նշագիծը (tick)
                if (isXAxis) {
                        painter.drawLine(coordinate, -tickSize, coordinate, tickSize);
                } else {
                        painter.drawLine(-tickSize, coordinate, tickSize, coordinate);
                }

                // Նկարում ենք թվերը
                painter.save();
                painter.scale(1, -1);
                painter.setPen(Qt::darkGray);

                if (isXAxis) {
                        painter.drawText(coordinate - 10, 15, QString::number(coordinate / step));
                } else {
                        painter.drawText(10, -coordinate + 4, QString::number(coordinate / step));
                }

                painter.restore();
        };
        for (int x = step; x <= w / 2; x += step) { drawTick(x, true); }
        for (int x = -step; x >= -w / 2; x -= step) { drawTick(x, true); }
        for (int y = step; y <= h / 2; y += step) { drawTick(y, false); }
        for (int y = -step; y >= -h / 2; y -= step) { drawTick(y, false);}

        painter.save();
        painter.scale(1, -1);
        painter.setPen(Qt::darkGray);
        painter.drawText(5, 15, "0");
        painter.restore();

        painter.setPen(QPen(Qt::black, 2));
        painter.drawLine(-w / 2, 0, w / 2, 0); // X առանցք
        painter.drawLine(0, -h / 2, 0, h / 2); // Y առանցք
        // ----------------------------------------

        // Նկարում ենք բոլոր գոյություն ունեցող ուղղանկյունները
        for (unsigned int i = 0; i < rectangles.size(); ++i) {
                // Ներկում ենք ՄԻԱՅՆ Active Set-ում գտնվողները հատուկ գույնով
                if (isSweeping && activeSet.contains(i)) {
                        painter.setPen(QPen(Qt::darkGreen, 2)); // Կանաչ եզրագիծ ակտիվների համար
                        painter.setBrush(QColor(0, 255, 0, 80)); // Կանաչ կիսաթափանցիկ ֆոն
                } else {
                        painter.setPen(QPen(Qt::blue, 2)); // Կապույտ եզրագիծ պասիվների համար
                        painter.setBrush(QColor(0, 0, 255, 50)); // Կապույտ կիսաթափանցիկ ֆոն
                }
                drawRect(painter, rectangles[i]);
        }

        // Նկարում ենք այն ուղղանկյունը, որը օգտատերը հենց հիմա գծում է մկնիկով
        if (isDrawingRect) {
                painter.setPen(QPen(Qt::red, 2, Qt::DashLine)); // Կարմիր կետագիծ
                painter.setBrush(QColor(255, 0, 0, 30));
                drawRect(painter, currentRect);
        }

        // Նկարում ենք Սահող Գիծը (Sweep Line)
        if (isSweeping) {
                painter.setPen(QPen(Qt::red, 3)); // Հաստ կարմիր կետագիծ
                painter.drawLine(QPointF(sweepX, -h / 2), QPointF(sweepX, h / 2)); // Ուղղահայաց գիծ X=sweepX կոորդինատով
        }
}

void CoordinatePlane::
mousePressEvent(QMouseEvent *event)
{
        if (event->button() == Qt::LeftButton) { // Եթե ձախ կլիկ է եղել
                if (isSweeping) { // Եթե անիմացիան գնում է, կանգնեցնել այն
                        timer->stop();
                        isSweeping = false;
                }
                isDrawingRect = true; // Սկսում ենք նոր ուղղանկյան գծում
                Point p = screenToMath(event->pos()); // Էկրանի կոորդինատը վերածում ենք մաթեմատիկականի
                currentRect.bottomLeft = p; // Սահմանում ենք սկզբնակետը
                currentRect.topRight = p;
                update();
        }
}

void CoordinatePlane::
mouseMoveEvent(QMouseEvent *event)
{
        if (isDrawingRect) {
                currentRect.topRight = screenToMath(event->pos()); // Թարմացնում ենք աջ վերևի անկյունը մկնիկի դիրքով
                update(); // Վերանկարում ենք պրոցեսը ցույց տալու համար
        }
}

void CoordinatePlane::
mouseReleaseEvent(QMouseEvent *event)
{
        if (event->button() == Qt::LeftButton && isDrawingRect) { // Երբ բաց ենք թողնում ձախ կլիկը
                isDrawingRect = false; // Ավարտում ենք գծումը
                currentRect.topRight = screenToMath(event->pos());
                rectangles.push_back(normalizeRect(currentRect)); // Նորմալիզացնում ենք (ճիշտ տեղերով դնել bottomLeft և topRight) և պահում զանգվածում
                update();
        }
}

Point CoordinatePlane::
screenToMath(const QPoint& screenPos) {
        return {
                static_cast<double>(screenPos.x() - width() / 2),
                static_cast<double>(height() / 2 - screenPos.y())
        };
}

Rectangle CoordinatePlane::
normalizeRect(const Rectangle& r)
{
        Rectangle n;
        n.bottomLeft.x = std::min(r.bottomLeft.x, r.topRight.x);
        n.bottomLeft.y = std::min(r.bottomLeft.y, r.topRight.y);
        n.topRight.x = std::max(r.bottomLeft.x, r.topRight.x);
        n.topRight.y = std::max(r.bottomLeft.y, r.topRight.y);
        return n;
}

void CoordinatePlane::
drawRect(QPainter& painter, const Rectangle& r)
{
        double w = r.topRight.x - r.bottomLeft.x; // Լայնություն
        double h = r.topRight.y - r.bottomLeft.y; // Բարձրություն
        painter.drawRect(QRectF(r.bottomLeft.x, r.bottomLeft.y, w, h)); // Qt-ի ֆունկցիան
}
