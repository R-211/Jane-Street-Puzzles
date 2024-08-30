#include "problem.h"

ProcessSimulator::ProcessSimulator(Coordinate box_x1, Coordinate box_y1, Coordinate box_x2, Coordinate box_y2, std::size_t iterations) :
	box_x1_(box_x1), box_y1_(box_y1), box_x2_(box_x2), box_y2_(box_y2), iterations_(iterations) {};

PointGenerator::PointGenerator(Coordinate x_min, Coordinate x_max, Coordinate y_min, Coordinate y_max) :
    generator_(std::random_device{}()),
    x_dist_(x_min, x_max),
    y_dist_(y_min, y_max) {}

void ProcessSimulator::simulateProcess() const
{
    if (iterations_ == 0) throw std::invalid_argument("[!] ProcessSimulator::simulateProcess -> Null value for iterations given");

    std::size_t counter{ 0 };
    Ellipse ellipse;
    Point point_1{}, point_2{};
    PointGenerator point_generator(box_x1_, box_x2_, box_y1_, box_y2_);

    for (std::size_t i{ 0 }; i < iterations_; ++i)
    {
        point_1 = point_generator.generatePoint();
        point_2 = point_generator.generatePoint();

        // Calculate the center of the ellipse (coordinates of the point)
        Coordinate center_x = (point_1.x + point_2.x) / 2.0;
        Coordinate center_y = (point_1.y + point_2.y) / 2.0;

        double radius = calculateRadius(point_1, point_2);

        ellipse.center.x = center_x;
        ellipse.center.y = center_y;
        ellipse.radius = radius;

        if (!isInside({ {box_x1_, box_y1_}, {box_x2_, box_y2_} }, ellipse))
        {
            counter++;
        }
    }

    double probability = static_cast<double>(counter) / iterations_;
    std::cout << std::format("[+] The probability after {} iterations is {:.10f}\n", iterations_, probability);
}

[[nodiscard]] Point PointGenerator::generatePoint() noexcept
{
    return { x_dist_(generator_), y_dist_(generator_) };
}

[[nodiscard]] double calculateRadius(const Point& p1, const Point& p2) noexcept
{
    // Calculate the vector which goes from p1 to p2 and then calculating its modulus to get the diameter
    double d_x = p2.x - p1.x, d_y = p2.y - p1.y;
    double distance = std::sqrt(d_x * d_x + d_y * d_y);
   
    return distance / 2.0;
}

[[nodiscard]] bool isInside(const Box& box, const Ellipse& ellipse) noexcept
{
    double box_x1 = box.coordinate1.x, box_y1 = box.coordinate1.y;
    double box_x2 = box.coordinate2.x, box_y2 = box.coordinate2.y;

    double ellipse_x = ellipse.center.x, ellipse_y = ellipse.center.y;
    double ellipseRadius = ellipse.radius;

    /*  To check whether the circle is fully inside the box the following requiremenets must be met:
    * 
    *   1-. ellipse_x - ellipseRadius >= box_x1 checks if the leftmost point of the circle is aligned with or to the right of the left edge of the box
    *   2-. ellipse_x + ellipseRadius <= box_x2 if the rightmost point of the circle is aligned with or to the left of the right edge of the box
    *   3-. ellipse_y - ellipseRadius >= box_y1 checks if the bottommost point of the circle is aligned with or above the bottom edge of the box
    *   4-. ellipse_y + ellipseRadius <= box_y2 checks if the topmost point of the circle is aligned with or below the top edge of the box
    * 
    */

    return ellipse_x - ellipseRadius >= box_x1 &&
        ellipse_x + ellipseRadius <= box_x2 &&
        ellipse_y - ellipseRadius >= box_y1 &&
        ellipse_y + ellipseRadius <= box_y2;
}