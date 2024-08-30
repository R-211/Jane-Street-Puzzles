#include <iostream>
#include <random>
#include <format>

constexpr const double DECREMENT_VALUE{ 0.0 };
constexpr const std::size_t NUMBER_OF_ITERATIONS{ 0 };

class RandomGenerator
{
private:
    std::random_device rd{};
    std::mt19937_64 gen{};
    std::uniform_real_distribution<double> r_coords{ 0.0, 1.0 };

public:
    RandomGenerator() noexcept : gen(rd()) {}
    [[nodiscard]] inline double get_random_coord() noexcept { return r_coords(gen); }
};

// Enum to decide if there's a draw or which robot has won
enum class Winner
{
    None = 0,
    Robot1 = 1,
    Robot2 = 2
};

struct BestDistance
{
    double ratio{}, distance{};
};

// Check the winner regarding the marker coordinates
[[nodiscard]] Winner checkWinner(double marker_x_coord) noexcept
{
    if (marker_x_coord < -0.5) return Winner::Robot2;
    if (marker_x_coord > 0.5) return Winner::Robot1;
    else return Winner::None;
}

[[nodiscard]] Winner simulate(double aprox_value) noexcept
{
    double marker_coordinates{ aprox_value };
    double r1_counter{ 0.0 }, r2_counter{ 0.0 };

    RandomGenerator rd{};

    while (true)
    {
        // We simulate each turn by making robot1 and robot2 move the flags towards them with a random distance
        double r1_movement = rd.get_random_coord();
        double r2_movement = rd.get_random_coord();

        marker_coordinates += r1_movement;
        if (checkWinner(marker_coordinates) == Winner::Robot1) return Winner::Robot1;

        marker_coordinates -= r2_movement;
        if (checkWinner(marker_coordinates) == Winner::Robot2) return Winner::Robot2;
    }
    return Winner::None;
}

int main(void)
{
    BestDistance best_distance{ 10'000, 10'000 };

    for (double i{ 0.0 }; i >= -1.0; i -= DECREMENT_VALUE)
    {
      
        std::cout << std::format("[+] Trying distance {:.7f} {}", i) << std::endl;

        double robot1_counter{ 0 }, robot2_counter{ 0 };

        for (std::size_t j{ 0 }; j < NUMBER_OF_ITERATIONS; ++j)
        {
            Winner winner = simulate(i);

            if (winner == Winner::Robot1) robot1_counter++;
            else if (winner == Winner::Robot2) robot2_counter++;
        }

        // If the robot2_counter isn't null we compute the ratio
        double ratio = (robot2_counter > 0.0) ? (robot1_counter / robot2_counter) : std::numeric_limits<double>::infinity();

        // We check if the current ratio is closer to one than the best ratio so far (this because we want a 50-50 competition)
        if (std::abs(1.0 - ratio) < std::abs(1.0 - best_distance.ratio))
        {
            best_distance.ratio = ratio;
            best_distance.distance = i;
        }
    }
    
    std::cout << std::format("[+] Ratio : {}", best_distance.ratio) << std::endl;
    std::cout << std::format("[+] Estimated initial marker position: {:.7f} {}", best_distance.distance) << std::endl;

    return EXIT_SUCCESS;
}
