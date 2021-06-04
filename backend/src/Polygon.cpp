#include "Polygon.hpp"
#include <optional>

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
	constexpr float MAX_DISTANCE2 = 0.001f;
    isCircle = false;
	std::cout << "c: " << c.x << ", " << c.y << std::endl;
	size_t insertCount = 0;
	std::optional<Point> last = std::nullopt;
	for(auto& p : vertices) {
		float dx = p.x - c.x;
		float dy = p.y - c.y;
		float s = sqrtf(1 + r*r/(dx*dx + dy*dy));
		dx *= s;
		dy *= s;
		p.x = c.x + dx;
		p.y = c.y + dy;
		if(last) {
			if(distance2(last.value(), p) > MAX_DISTANCE2) {
				++insertCount;
			}
		}
		last = p;
	}	
}

void Polygon::makeCircle(Point mid, float radius)
{
    if(isCircle)
    {
        size_t count = 200; // circleVertCount(radius);
		vertices.resize(count);
        for (int i = 0; i < count; ++i)
        {
            float angle = (float)i / (float)count * 2 * M_PI;
            vertices[i] = Point(radius * cosf(angle) + mid.x, radius * sinf(angle) + mid.y);
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
