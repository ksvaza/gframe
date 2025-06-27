#pragma once
class Velocity
{
public:
    Velocity(float vx, float vy, float vz)
        : vx(vx), vy(vy), vz(vz) {};
    Velocity() = default;
    float vx, vy, vz;
};