#include "Polygon.hpp"

Polygon Polygon::createCircle(Point mid, float radius)
{
    Polygon p{};
    size_t count = circleVertCount(radius);

    for (int i = 0; i < count + 1; ++i)
    {
        float angle = (float)i / (float)count * 2 * M_PI;
        p.vertices.push_back(Point(radius * cosf(angle) + mid.x, radius * sinf(angle) + mid.y));
    }
    return p;
}

size_t Polygon::getVertCount()
{
    return vertices.size();
}

void Polygon::getDrawInfo(std::vector<GLuint> &indices, std::vector<WGLVertex> &vertices)
{
    // following polylines in the vector would be interpreted as holes
    std::vector<std::vector<Point>> poly = {this->vertices};
    indices = mapbox::earcut<GLuint>(poly);
    for(auto p: this->vertices)
    {
        vertices.push_back(WGLVertex{p, this->colorIndex});
    }
}

size_t Polygon::circleVertCount(float radius)
{
    auto opt = Options::getInstance();
    auto csize = opt->getCanvasSize();
    size_t squareCanvasSize = csize.first;
    return (size_t)(M_PI * radius * squareCanvasSize);//todo: make cast beautiful

}

void displace(Point mid, float radius)
{
    // TODO
}

void scale(float factor)
{
    // todo
    // idea: set a private scaling member and apply at getDrawInfo for perfomance
}