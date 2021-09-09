#pragma once

#include <vector>
#include <algorithm>

#include "Polygon.hpp"

/**
 * Contains a vector of polygons and is iterable.
 */
class Scene
{
private:
    std::vector<Polygon> polygons; ///<Internal vector to store all polygons
    size_t vertCount;
	unsigned generation = 0; ///< generation of polygons, needed to reduce buffer updates on GPU
	std::vector<Displacement> displacements;

public:

	const auto& getDisplacements() const {
		return displacements;
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
		++generation;
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
	
	void clear() {
		polygons.clear();
		++generation;
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
	std::vector<Polygon>::const_reverse_iterator rbegin() const
	{
		return polygons.rbegin();
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
	std::vector<Polygon>::const_reverse_iterator rend() const
	{
		return polygons.rend();
	}

	void applyDisplacement(size_t _canvasSize) { ///< stores current state, advance generation
		std::cout << "can size: " << _canvasSize << std::endl;
		if(displacements.empty()) { return; }

		for(auto& p : *this) {
			p.displace(displacements, _canvasSize);
		}

		// remove polygons with less then 3 vertices
		polygons.erase(
			std::remove_if(begin(), end(), [](const Polygon& p){
				return p.getVertCount() < 3;
			}),
			end());

		displacements.clear();
		++generation;
	}

	void addDisplacement(Point p, float newRadius) {
		displacements.push_back({p, newRadius});
	}

	void setDisplacementRadius(float rad) {
		for(auto& dis : displacements) {
			dis.r = rad;
		}
	}
};
