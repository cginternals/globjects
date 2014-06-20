#pragma once

struct PrivateGlowInterface;

class GlowInterface
{
public:
    GlowInterface();
    ~GlowInterface();

    void initialize();

    void setupTexture(unsigned id, unsigned target);

    void resize(int x, int y);

    void paint();

protected:
    PrivateGlowInterface * impl;
};
