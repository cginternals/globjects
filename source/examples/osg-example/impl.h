#pragma once

struct PrivateGlobjectsInterface;

class GlobjectsInterface
{
public:
    GlobjectsInterface();
    ~GlobjectsInterface();

    void initialize();

    void setupTexture(unsigned id, unsigned target);

    void resize(int x, int y);

    void paint();

protected:
    PrivateGlobjectsInterface * impl;
};
