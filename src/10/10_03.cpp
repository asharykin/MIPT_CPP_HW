#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

class Life
{
public:

    Life() : rows(10), columns(10), grid(10, std::vector<unsigned char>(10, 0)) { }

    void run(int const generations, std::chrono::milliseconds const ms = std::chrono::milliseconds(300))
    {
        initialize();

        for (int i = 0; i <= generations; ++i)
        {
            display(i);
            next_generation();
            std::this_thread::sleep_for(ms);
        }
    }

private:

    void next_generation()
    {
        std::vector newgrid(rows, std::vector<unsigned char>(columns, 0));

        for (size_t r = 0; r < rows; ++r)
        {
            for (size_t c = 0; c < columns; ++c)
            {
                int count = count_neighbors(r, c);

                if (grid[r][c] == alive)
                {
                    newgrid[r][c] = (count == 2 || count == 3) ? alive : dead;
                } else
                {
                    newgrid[r][c] = (count == 3) ? alive : dead;
                }
            }
        }

        grid.swap(newgrid);
    }

    void display(int gen) const
    {
        std::cout << "Generation: " << gen << std::endl;

        for (const auto& row : grid)
        {
            for (auto cell : row)
            {
                std::cout << (cell == alive ? "* " : ". ");
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    void initialize()
    {
        // initializing with the famous "glider" figure
        grid[1][2] = alive;
        grid[2][3] = alive;
        grid[3][1] = alive;
        grid[3][2] = alive;
        grid[3][3] = alive;
    }

    int count_neighbors(int r, int c) const
    {
        int count = 0;
        
        // 8 neighbors around the cell (r, c)
        for (int i = -1; i <= 1; ++i)
        {
            for (int j = -1; j <= 1; ++j)
            {
                if (i == 0 && j == 0)
                {
                    continue; // skip the cell itself
                }

                int row = r + i;
                int col = c + j;

                // check the grid bounds
                if (row >= 0 && row < (int) rows && col >= 0 && col < (int) columns)
                {
                    if (grid[row][col] == alive) count++;
                }
            }
        }
        return count;
    }

private:

    size_t const rows;
    size_t const columns;

    std::vector<std::vector<unsigned char>> grid;
    const unsigned char alive = 1;
    const unsigned char dead = 0;
};

// my implementation is based on the one from The Modern C++ Challenge (p. 128-132)
int main()
{
    using namespace std::chrono_literals;

    Life game;
    game.run(30, 200ms);
}
