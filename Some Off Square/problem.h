#pragma once

#include <iostream>
#include <format>
#include <random>

using Coordinate = double;

struct Point 
{
	Coordinate x{};
	Coordinate y{};
};

struct Ellipse
{
	Point center{};
	double radius{};
};

struct Box
{
	Point coordinate1{};
	Point coordinate2{};
};

class PointGenerator
{
private:
	std::mt19937_64                             generator_{};
	std::uniform_real_distribution<double>      x_dist_{};
	std::uniform_real_distribution<double>      y_dist_{};

public:
	PointGenerator(Coordinate x_min, Coordinate x_max, Coordinate y_min, Coordinate y_max);

	[[nodiscard]] Point generatePoint() noexcept;
};

class ProcessSimulator
{
private:
	Box box_{};
	std::size_t iterations_{};
	Coordinate box_x1_{}, box_y1_{}, box_x2_{}, box_y2_{};
public:
	ProcessSimulator(Coordinate box_x1_, Coordinate box_y1_, Coordinate box_x2_, Coordinate box_y2_, std::size_t iterations);
	void simulateProcess() const;
};

[[nodiscard]] bool isInside(const Box& box, const Ellipse& ellipse) noexcept;
[[nodiscard]] double calculateRadius(const Point& p1, const Point& p2) noexcept;