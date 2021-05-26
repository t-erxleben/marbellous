#include "Polygon.hpp"

Polygon::Polygon(Point mid, float radius, GLushort colorIndex): colorIndex{colorIndex}, isCircle{true}
{
    makeCircle(mid, radius);
}

Polygon::Polygon() : vertices{}, isCircle{true}
{}

size_t Polygon::getVertCount() const
{
    return vertices.size();
}

void Polygon::getDrawInfo(std::vector<GLuint> *indices, std::vector<WGLVertex> *vertices) const
{
    // following polylines in the vector would be interpreted as holes
    std::vector<std::vector<Point>> poly{};
    poly.push_back(this->vertices);
    *indices = mapbox::earcut<GLuint>(poly);

    for (auto& p : this->vertices)
    {
        vertices->push_back(WGLVertex{p, (GLuint)this->colorIndex});
    }
}

size_t Polygon::circleVertCount(float radius)
{
    auto opt = WGLContext::getContext();
    auto csize = opt->getCanvasSize();
    size_t squareCanvasSize = csize.first;

    size_t vert_count = static_cast<size_t>(M_PI * radius * squareCanvasSize);
    if (vert_count > 200) {
        vert_count = vert_count/10;
    }
    return vert_count;
}

void Polygon::displace(Point mid, float radius)
{
    isCircle = false;
    // TODO
}

void Polygon::makeCircle(Point mid, float radius)
{
    size_t count = circleVertCount(radius);
    vertices.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        // float angle = (float)i / (float)count * 2 * M_PI;
        float angle = (float)i / (float)count * 2 * M_PI;
        vertices.push_back(Point(radius * cosf(angle) + mid.x, radius * sinf(angle) + mid.y));
    }
}