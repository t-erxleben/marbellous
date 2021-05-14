#pragma once

#include "WGLRenderer.hpp"
#include "Scene.hpp"

class WGLSceneRenderer: private WGLRenderer
{
    public:
        void drawScene(Scene const & scene);
};