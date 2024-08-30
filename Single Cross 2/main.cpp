#include <iostream>
#include <format>
#include <random>

constexpr const std::size_t NUM_TRIALS{ 0 };
constexpr const double DIFFERENCE{ 0.0 };

class RandomGenerator
{
private:
    std::random_device rd{};
    std::mt19937_64 gen{ rd() };
    std::uniform_real_distribution<double> r_coords{ 0.0, 1.0 };
    std::normal_distribution<double> r_normal{ 0.0, 1.0 };

public:
    RandomGenerator() noexcept = default;

    [[nodiscard]] inline double get_random_coord() noexcept { return r_coords(gen); }
    [[nodiscard]] inline double get_random_normal() noexcept { return r_normal(gen); }
};

/*  @simulate
*
*   1-. Random variables x, y and z are generated as the origin of the line
*   2-. Random direction dx, dy and dz are generated using normal distribution
*   3-. The direction vector is normalized
*   4-. The endpoint is calculated with the origin, the distance and the direction
*   5-. Check for intersections with orthogonally adjacent cubes
*/

[[nodiscard]] double simulate(double D, int iteration_amount) noexcept
{
    RandomGenerator rd{};

    std::size_t num_crossings{ 0 };

    for (std::size_t i{ 0 }; i < iteration_amount; ++i)
    {
        // Generate random starting point (x,y,z)
        double x = rd.get_random_coord(), y = rd.get_random_coord(), z = rd.get_random_coord();

        // Generate random direction using normal distribution (mean 0 & st. deviation 1)
        double dx = rd.get_random_normal(), dy = rd.get_random_normal(), dz = rd.get_random_normal();

        // Normalize the direction vector (making it a unitary vector)
        double norm = std::sqrt(dx * dx + dy * dy + dz * dz);

        dx /= norm;
        dy /= norm;
        dz /= norm;

        // Calculates the endpoint by adding to the starting point (x,y,z) the direction vector (dx,dy,dz) multiplied by the distance D.
        double x2 = x + D * dx;
        double y2 = y + D * dy;
        double z2 = z + D * dz;


        /*
        *    Since the problem states that it should count only when it intersects orthogonally adjacent cubes,
        *    then it will check for planes_crossed being equal to 1, because if planes_crossed is 1,
        *    it means the line segment has crossed into an adjacent cube in exactly one dimension, and therefore num_corssings
        *    will be updated.
        */

        std::size_t planes_crossed{ 0 };
        if (std::floor(x) != std::floor(x2)) planes_crossed++;
        if (std::floor(y) != std::floor(y2)) planes_crossed++;
        if (std::floor(z) != std::floor(z2)) planes_crossed++;

        if (planes_crossed == 1)
        {
            num_crossings++;
        }
    }
    return static_cast<double>(num_crossings) / iteration_amount;
}

int main(void)
{
    double max_probability{ 0.0 }, optimal_d{ 0.0 };

    for (double D = 0.0; D <= 1.0; D += DIFFERENCE)
    {
        double probability = simulate(D, NUM_TRIALS);
        std::cout << std::format("[+] D {} with probability {}", D, probability) << std::endl;

        if (probability > max_probability)
        {
            max_probability = probability;
            optimal_d = D;
        }
    }

    std::cout << std::format("[+] {:.10f},{:.10f}", optimal_d, max_probability) << std::endl;

    return EXIT_SUCCESS;
}