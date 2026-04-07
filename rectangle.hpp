#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "point.hpp"

// Ուղղանկյան ստրուկտուրա, սահմանվում է երկու հակադիր անկյուններով
struct Rectangle {
        Point bottomLeft; // Ներքևի ձախ անկյուն (X_min, Y_min)
        Point topRight;   // Վերևի աջ անկյուն (X_max, Y_max)
};

#endif // RECTANGLE_HPP
