#pragma once

#include <functional>

#include <glow/ref_ptr.h>

namespace glowutils {
class Camera;
}

namespace glow {

class Program;
class Texture;

class AbstractTransparencyAlgorithm {
public:
    typedef std::function<void(Program*)> DrawFunction;

    virtual void initialize() {}
    virtual void draw(const DrawFunction& drawFunction, glowutils::Camera* camera, int width, int height) = 0;
    virtual void resize(int width, int height) {}
    virtual Texture* getOutput() = 0;
protected:
    Texture* createColorTex();
};

} // namespace glow
