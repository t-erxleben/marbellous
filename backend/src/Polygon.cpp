#include "Polygon.hpp"

#include <cassert>

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
		vertices[i] = WGLVertex{this->vertices[i].pos, z, colorIndex};
	}
}

size_t Polygon::circleVertCount(float radius)
{
    auto opt = WGLContext::getContext();
    auto csize = opt->getDropRes();
    size_t squareCanvasSize = csize;

    size_t vert_count = static_cast<size_t>(M_PI * radius * squareCanvasSize);
    if (vert_count > 200) {
        vert_count = vert_count/10;
    } else if(vert_count < 3) {
		vert_count = 3;
	}
    return vert_count;
}

/** displace a point in place
 */
Point& displacePoint(Point& p, const std::vector<Displacement>& displacements) {
	Point move{0, 0};
	for(const auto& dis : displacements) {
		Point d = p - dis.p;
		d *= sqrtf(1 + dis.r*dis.r/ (d*d) );
		move += dis.p + d - p;
	}
	p += move;
	return p;
}

Point displacePoint(const Point& p, const std::vector<Displacement>& displacements) {
	Point ret(p);
	return displacePoint(ret, displacements);
}
int Polygon::calcInsertion(const Point& p0, const Point& p1, float min_dis2, float max_dis2) {
	float d2 = distance2(p0, p1);
	if(d2 > max_dis2) {
		return static_cast<int>(sqrtf(d2 / min_dis2));
	} else if (d2 < min_dis2) {
		return -1;
	}
	return 0;
}

void Polygon::displace(const std::vector<Displacement>& displacements, size_t canvas_size)
{
	assert(vertices.size() > 2);
	const float px_size = 2.f / static_cast<float>(canvas_size);
	float min_dis2 = px_size * MIN_DISTANCE; min_dis2 *= min_dis2;
	float max_dis2 = px_size * MIN_DISTANCE; max_dis2 *= max_dis2;
    isCircle = false;
	vertices[0].last = vertices[0].pos;
	displacePoint(vertices[0].pos, displacements);
	int size_diff = 0;
	size_t lastId = 0;
	vertices[0].insertion = 0;
	for(std::size_t i = 1; i < vertices.size(); ++i) {
		Point& p = vertices[i].pos;
		vertices[i].last = p;

		displacePoint(p, displacements);
		vertices[i].insertion = calcInsertion(vertices[lastId].pos, p, min_dis2, max_dis2);

		if (vertices[i].insertion >= 0) {lastId = i; }
		size_diff += vertices[i].insertion;
	}

	int insertion
		= calcInsertion(vertices.back().pos, vertices[0].pos, min_dis2, max_dis2);

	// remove collapsed vertices
	int del = 0;
	{
		auto dst = vertices.begin();
		decltype(vertices)::iterator src;
		for(src = vertices.begin(); src != vertices.end(); ++src) {
			if (src->insertion >= 0) {
				*dst = *src;
				++dst;
			}
		}
		del = src - dst;
	}

	// insert new vertices
	vertices.resize(vertices.size() + size_diff);
	auto dst = vertices.rbegin();
	auto src = vertices.rbegin() + del + size_diff;
	insertion = 0;
	float fraction = 0.f;
	Point last{};
	Point next{};
	while(dst != vertices.rend()) {
		if(insertion) {
			float x = fraction * insertion;
			dst->pos = displacePoint(Point::mix(last, next, x), displacements);
			++dst;
			--insertion;
		} else if(src->insertion >= 0) {
			assert(src != vertices.rend());
			*dst = *src;
			insertion = src->insertion;
			fraction = 1.f / static_cast<float>(insertion+1);
			last = src->last;
			++dst;
			++src;
			next = src != vertices.rend()
				? src->last
				: vertices.back().last;
		} else {
			++src;
		}
		assert(src >= dst);
	}
	assert(src == vertices.rend());
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
            vertices[i] = {Point(radius * cosf(angle) + mid.x, radius * sinf(angle) + mid.y), 0};
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
