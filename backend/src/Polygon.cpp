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
    isCircle = false;
	Point last;
	Point origin;
	std::vector<std::pair<size_t, Point>> newPoints{};
	int i = 0;
	for(auto& p : vertices) {
		origin = p;
		float dx = p.x - c.x;
		float dy = p.y - c.y;
		if(abs(dx) > 1.f) { dx = (dx>0?-1.f:1.f)*(2.f - abs(dx)); }
		if(abs(dy) > 1.f) { dy = (dy>0?-1.f:1.f)*(2.f - abs(dy)); }
		float s = sqrtf(1 + r*r/(dx*dx + dy*dy)) - 1.f;
		Point n(p.x + dx*s, p.y + dy*s);
		if(i && distance2(last, n) > 0.00001) {
			Point newPoint((origin.x+p.x)/2.f,(origin.y+p.y)/2.f); 
			dx = newPoint.x - c.x;
			dy = newPoint.y - c.y;
			if(abs(dx) > 1.f) { dx = (dx>0?-1.f:1.f)*(2.f - abs(dx)); }
			if(abs(dy) > 1.f) { dy = (dy>0?-1.f:1.f)*(2.f - abs(dy)); }
			s = sqrtf(1 + r*r/(dx*dx + dy*dy)) - 1.f;
			newPoint.translate(dx*s,dy*s);
			newPoints.push_back(std::make_pair(i, newPoint));
		}
		p = n;
		last = n;
		++i;
	}	
	vertices.resize(vertices.size() + newPoints.size());
	int offset = newPoints.size();
	auto nItr = newPoints.rbegin();
	for(i = vertices.size()-1; i >= 0; --i) {
		int src = i-offset;
		if(offset && src == nItr->first) {
			vertices[i] = nItr->second;
			--offset;
		} else {
			vertices[i] = vertices[src];
		}
	}
}

void Polygon::makeCircle(Point mid, float radius)
{
    if(isCircle)
    {
		// FIXME: for debug puprese only
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
