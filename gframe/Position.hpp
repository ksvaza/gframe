#pragma once
class Position
{
public:
    Position(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : x(x), y(y), z(z) {}
    float x, y, z;
};