#include <Palette.hpp>

#include <random>
#include <cassert>

void Palette::updateP() {
	unsigned sum = 0;
	for(const auto& color : colors) { sum += color.m_ratio; }
	assert(sum != 0);
	const float part = 1.f / static_cast<float>(sum);
	float prv = 0;
	for(auto& color : colors) {
		prv += color.m_ratio * part;
		color.m_p = prv;
	}
}

Palette::Palette(): colors{}
{}

Palette::Palette(size_t num_colors): colors(num_colors)
{
	updateP();
}

size_t Palette::add(Color const col)
{
    colors.push_back(col);
	updateP();
    return colors.size() - 1;
}

void Palette::setRatioAt(size_t index, unsigned ratio) {
	(*this)[index].m_ratio = ratio;
	updateP();
}

void Palette::setColorAt(size_t index, const Color &color) {
	(*this)[index] = color;
	updateP();
}

Color& Palette::operator[](size_t const index)
{
    return colors.at(index);
}

const Color& Palette::operator[](size_t const index) const
{
    return colors.at(index);
}

size_t Palette::getSize() const
{
    return colors.size();
}

unsigned Palette::getRandomColorId() const 
{
	static std::mt19937 rng(std::random_device{}());
	static std::uniform_real_distribution<float> dis(0, 1);
	
	const float n = dis(rng);
	for(auto itr = colors.begin(); itr != colors.end(); ++itr) {
		if(n <= itr->m_p) { return itr - colors.begin(); }
	}
	return colors.size() - 1;
}
