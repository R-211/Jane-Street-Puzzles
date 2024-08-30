#include <iostream>
#include <format>
#include <unordered_map>
#include <algorithm>
#include <numeric>

constexpr const int NUMBER{ 1247 };
constexpr const int EXPELLED_MAX_NUMBER{ 11 };

using Sequence = std::vector<int>; 

Sequence next(const Sequence& line, int row) 
{
    Sequence before(line.begin(), line.begin() + row);  // Sequence of the elements before the expelled element
    Sequence after(line.begin() + row + 1, line.end()); // Sequence of the elements after the expelled element
    Sequence next_row{};                                // Vector to store the next row

    while (!before.empty()) 
    {
        // Get the closest element from the before Sequence to the expelled and add it to next_row Sequence while also removing that value from before
        next_row.push_back(before.back());
        before.pop_back();

        // As long as there are elements remaining in the after Sequence, the closest element to the expelled number gets added to next_row and then removed from after
        if (!after.empty()) 
        {
            next_row.push_back(after.front());
            after.erase(after.begin());
        }
    }

    // Once the whole process is done the remaining elements after the expelled are inserted into next_row
    next_row.insert(next_row.end(), after.begin(), after.end());
    return next_row;
}

void getExpelledRow(int number)
{
    int row{ 0 };
    Sequence line(NUMBER - 1);
    std::iota(line.begin(), line.end(), 1);

    std::unordered_map<int, int>expelled{}; // (expelled_number, row)

    while (row < line.size())
    {
        // Save the row at which the expelled number was expelled
        expelled[line[row]] = row + 1;

        // We get the next sequence (move to the next row)
        line = next(line, row);
        ++row;
    }

    if (expelled.find(number) != expelled.end()) std::cout << std::format("[+] Number {} was expelled at row {}", number, expelled[number]);
    else std::cout << std::format("[!] Couldn't find the number provided");
}

int main(int agrc, char** argv)
{
    getExpelledRow(EXPELLED_MAX_NUMBER);

	return EXIT_SUCCESS;
}