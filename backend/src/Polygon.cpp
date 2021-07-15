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
	Point sum{0, 0};
	for(const auto& dis : displacements) {
		Point d = p - dis.p;
		d *= sqrtf(1 + dis.r*dis.r/ (d*d) );
		sum += dis.p + d;
	}
	sum /= static_cast<float>(displacements.size());
	p = sum;
	return p;
}

Point displacePoint(const Point& p, const std::vector<Displacement>& displacements) {
	Point ret(p);
	return displacePoint(ret, displacements);
}

// p = c + d
// p2 = c + d2
// p2 = c + d*s
// d2 = d * s
// d = d2/s
// s = sqrt(1+r*r/<d,d>)
// s^2 = 1 + r^2 / <d2/s,d2/s>
// s^2 = 1 + r^2 * s^2 / <d2,d2>
// 1 = 1/s^2 + r^2/<d2,d2>
// 1/s^2 = 1 - r^2/<d2,d2>
// 1/(1-r^2/<d2,d2>) = s^2
// s = sqrt(1/(1-r^2/<d2,d2>))
// d = d2 * 1/s
/*Point reverseDisplacementPoint(const Point& p, const Point& c, float r) {
	Point d = p - c;
	float det = 1.f - r*r/(d*d);
	if(det <= 0.f) {
		d /= sqrtf(d*d);
		d *= Polygon::MIN_R;
	} else {
		d /= sqrtf(1.f/(1.f-r*r/(d*d)));
	}
	return c + d;
}*/

int Polygon::calcInsertion(const Point& start, const Point& end) {
		float d2 = distance2(start, end);
		if(d2 > MAX_DISTANCE2) {
			return static_cast<int>(
					ceilf(sqrtf(d2) / VERTEX_DISTANCE));
		} else if (d2 < MIN_DISTANCE2) {
			return -1;
		}
		return 0;
}

void Polygon::displace(const std::vector<Displacement>& displacements)
{
	assert(vertices.size() > 2);
    isCircle = false;
	vertices[0].last = vertices[0].pos;
	displacePoint(vertices[0].pos, displacements);
	int size_diff = 0;
	size_t lastId = 0;
	for(std::size_t i = 1; i < vertices.size(); ++i) {
		Point& p = vertices[i].pos;
		vertices[i].last = p;

		displacePoint(p, displacements);
		vertices[i].insertion = calcInsertion(vertices[lastId].pos, p);

		if (vertices[i].insertion >= 0) {lastId = i; }
		size_diff += vertices[i].insertion;
	}
	vertices[0].insertion
		= calcInsertion(vertices.back().pos, vertices[0].pos);
	size_diff += vertices[0].insertion;

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
	int insertion = 0;
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
