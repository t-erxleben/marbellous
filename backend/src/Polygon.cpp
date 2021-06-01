#include "Polygon.hpp"

Polygon::Polygon(Point mid, float radius, GLuint colorIndex): colorIndex{colorIndex}, isCircle{true}, creationPoint{mid}
{
    makeCircle(mid, radius);
}

Polygon::Polygon() : vertices{}, creationPoint{}, isCircle{true}
{}

size_t Polygon::getVertCount() const
{
    return vertices.size();
}

void Polygon::getDrawInfo(std::vector<WGLVertex>& vertices, GLuint z) const
{
	vertices.resize(this->vertices.size());
	for(size_t i = 0; i < this->vertices.size(); ++i) {
		vertices[i] = WGLVertex{this->vertices[i], z, colorIndex};
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
    } else if(vert_count < 3) {
		vert_count = 3;
	}
    return vert_count;
}

void Polygon::displace(Point c, float r)
{
    isCircle = false;
	auto itr = dis.begin();
	for(auto& p : vertices) {
		Point o(p.x - itr->x, p.y - itr->y);
		float dx = o.x - c.x;
		float dy = o.y - c.y;
		float s = sqrtf(1 + r*r/(dx*dx + dy*dy));
		p.x = c.x + dx*s;
		p.y = c.y + dy*s;
		*itr = Point(p.x - o.x, p.y - o.y);
		++itr;
	}	
}

void Polygon::makeCircle(Point mid, float radius)
{
    if(isCircle)
    {
        size_t count = circleVertCount(radius);
		vertices.resize(count);
		dis.resize(count);

        for (int i = 0; i < count; ++i)
        {
            float angle = (float)i / (float)count * 2 * M_PI;
            vertices[i] = Point(radius * cosf(angle) + mid.x, radius * sinf(angle) + mid.y);
			dis[i] = Point(0,0);
        }
    }
    else
    {
        fprintf(stderr, "Tried to make a new circle out of a arbitrary polygon!\n");
    }

}

Point const & Polygon::getCreationPoint()
{
    return creationPoint;
}
