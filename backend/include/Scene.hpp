#pragma once

#include <vector>

#include "Polygon.hpp"

/**
 * Contains a vector of polygons and is iterable.
 */

static constexpr int CANVAS_SIZE = 1080;
static constexpr int IMAGE_SIZE = 1080;

class Scene
{
private:
    std::vector<Polygon> polygons; ///<Internal vector to store all polygons
    size_t vertCount;
	unsigned generation = 0; ///< generation of polygons, needed to reduce buffer updates on GPU
	struct {
		Point p;
		float r;
	} displacement;

public:

	const auto& getDisplacement() const {
		return displacement;
	}

	unsigned getGeneration() const {
		return generation;
	}

    /** Internal vector saving the references to the polygons
     * @param pol Polygon that will be added to the scene
     * @return Index of the saved polygon in the scene
    */
    int addPolygon(Polygon const &pol)
    {
        polygons.push_back(pol);
        return polygons.size() - 1;
    }

    /** Get or set the polygon at index, might throw std::out_of_range
     * @param index Index of the polygon you want to access
     * @return Reference to the requested polygon
    */
    Polygon &operator[](size_t const index)
    {
        return polygons.at(index);
    }

    /** Get the polygon at index, might throw std::out_of_range
     * @param index Index of the polygon you want to read
     * @return Reference to the requested polygon
    */
    Polygon const & operator[](size_t const index) const
    {
        return polygons.at(index);
    }

    /**
     * @return Number of polygons in the scene
    */
    size_t getPolygonCount() const
    {
        return polygons.size();
    }

    /**
     * @return Begin iterator over the polygons in the scene
    */
    std::vector<Polygon>::iterator begin()
    {
        return polygons.begin();
    }

    /**
     * @return Begin const_iterator over the polygons in the scene
    */
    std::vector<Polygon>::const_iterator begin() const
    {
        return polygons.begin();
    }

    /**
     * @return End iterator over the polygons in the scene
    */
    std::vector<Polygon>::iterator end()
    {
        return polygons.end();
    }

    /**
     * @return End const_iterator over the polygons in the scene
    */
    std::vector<Polygon>::const_iterator end() const
    {
        return polygons.end();
    }

	// stores current state, advance generation
	void store() {
		for(auto& p : *this) {
			p.displace(displacement.p, displacement.r);
		}
		displacement.r = 0;
		++generation;
	}

	void displace(int dropID, float newRadius) {
		displacement.p = polygons[dropID].getCreationPoint();
		displacement.r = fmin(0.1f, newRadius); // FIXME: find maximum before math breaks
	}
};
