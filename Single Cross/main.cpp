#include <iostream>
#include <cmath>
#include <random>

constexpr const double M_PI = 3.14159265358979323846;

/*
*   Using geometry, we can easily see that since the grids are length 1, then the maximum value for D can be sqrt(2) (diagonal).
*   Any line that is longer than this is guaranteed to cross once at least, but also, as it gets bigger, the chances of
*   crossing may get bigger as well.
*   We also know for a fact that:
*       - The origin coordinates are within [0,1]
*       - The angle is within [0, 360°] or [0, 2π]
*
*   The minimal length that guarantees crossing exactly one line is 1, which is the side length of the square.
*   Any segment shorter than this could lie entirely within a square without crossing.
*   Therefore we know that 1 <= D <= sqrt(2)
*/

class RandomGenerator
{
private:
    std::random_device rd{};
    std::mt19937_64 gen{};
    std::uniform_real_distribution<double> r_coords{ 0.0, 1.0 };
    std::uniform_real_distribution<double> r_angle{ 0.0, 360.0 };

public:
    RandomGenerator() noexcept = default;

    [[nodiscard]] inline double get_random_coord() noexcept { return r_coords(gen); }
    [[nodiscard]] inline double get_random_angle() noexcept { return r_angle(gen) * (M_PI / 180.0); }
};

[[nodiscard]] double simulate(double& d, const std::size_t& num_trials) noexcept
{
    RandomGenerator random_generator{};

    std::size_t num_crossings{ 0 };
    for (std::size_t i{ 0 }; i < num_trials; ++i)
    {
        std::size_t crossings{ 0 };

        //We generate the random origin coordinates and the random angle
        double x1 = random_generator.get_random_coord(), y1 = random_generator.get_random_coord();
        double angle = random_generator.get_random_angle();

        //We calculate the coordinates of the end of the line with the current position, the distance and the angle
        double x2 = x1 + std::cos(angle) * d;
        double y2 = y1 + std::sin(angle) * d;

        //Check the amount of crossings in the x-axis and in the y-axis
        std::size_t crossings_x = std::floor(x2) - std::floor(x1);
        std::size_t crossings_y = std::floor(y2) - std::floor(y1);

        //If we have only crossed only once, we increment the total amount of crossings
        if (crossings_x != 0) crossings++;
        if (crossings_y != 0) crossings++;

        if (crossings == 1) num_crossings++;
    }

    return static_cast<double>(num_crossings) / num_trials;
}

int main(void)
{
    const std::size_t num_trials{ 10'000'000 };
    double max_probability{ 0.0 }, optimal_d{ 0.0 };

    for (double d{ 1.0 }; d <= std::sqrt(2.0); d += 0.01)
    {
        double probability = simulate(d, num_trials);
        std::cout << std::format("[+] D {} with probability {}", d, probability) << std::endl;

        if (probability > max_probability)
        {
            max_probability = probability;
            optimal_d = d;
        }
    }

    std::cout << std::format("[+] Optimal D (line segment length) : {:.10f} {}", optimal_d) << std::endl;
    std::cout << std::format("[+] Maximal probability : {:.10f} {}", max_probability) << std::endl;

    return EXIT_SUCCESS;
}