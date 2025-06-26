#pragma once
class Velocity
{
public:
    Velocity(float vx = 0.0f, float vy = 0.0f, float vz = 0.0f)
        : vx(vx), vy(vy), vz(vz) {};
    Velocity() = default;
    float vx, vy, vz;
};