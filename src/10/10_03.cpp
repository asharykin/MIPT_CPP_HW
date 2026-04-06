#include <iostream>
#include <chrono>
#include <thread>

class Life {
public:

    Life() : grid(0) { }

    void run(int const generations, std::chrono::milliseconds const ms = std::chrono::milliseconds(200))
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

    void initialize()
    {
        set_cell(1, 2);
        set_cell(2, 3);
        set_cell(3, 1);
        set_cell(3, 2);
        set_cell(3, 3);
    }

    void set_cell(int r, int c)
    {
        grid |= (1ULL << (r * 8 + c));
    }

    void display(int gen) const
    {
        std::cout << "Generation: " << gen << std::endl;
        for (int r = 0; r < 8; ++r)
        {
            for (int c = 0; c < 8; ++c)
            {
                bool is_alive = (grid & (1ULL << (r * 8 + c)));
                std::cout << (is_alive ? "* " : ". ");
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }

    void next_generation()
    {
        // Masks to prevent bits from "wrapping around" to the opposite side of the grid in the next row
        const uint64_t NOT_LEFT_COL  = 0xFEFEFEFEFEFEFEFEULL;
        const uint64_t NOT_RIGHT_COL = 0x7F7F7F7F7F7F7F7FULL;

        // Shift the entire grid to align each cell with its 8 possible neighbors
        uint64_t L  = (grid & NOT_LEFT_COL)  >> 1; // Neighbor to the Left (West)
        uint64_t R  = (grid & NOT_RIGHT_COL) << 1; // Neighbor to the Right (East)
        uint64_t U  = grid >> 8; // Neighbor Up (North)
        uint64_t D  = grid << 8; // Neighbor Down (South)
        uint64_t UL = (grid & NOT_LEFT_COL)  >> 9; // Neighbor Up-Left (North-West)
        uint64_t UR = (grid & NOT_RIGHT_COL) >> 7; // Neighbor Up-Right (North-East)
        uint64_t DL = (grid & NOT_LEFT_COL)  << 7; // Neighbor Down-Left (South-West)
        uint64_t DR = (grid & NOT_RIGHT_COL) << 9; // Neighbor Down-Right (South-East)

        // 3-bit vertical counter (b2, b1, b0) to store neighbor counts (0-8) for each cell
        uint64_t b0 = 0, b1 = 0, b2 = 0;

        add_to_counter(b0, b1, b2, L);
        add_to_counter(b0, b1, b2, R);
        add_to_counter(b0, b1, b2, U);
        add_to_counter(b0, b1, b2, D);
        add_to_counter(b0, b1, b2, UL);
        add_to_counter(b0, b1, b2, UR);
        add_to_counter(b0, b1, b2, DL);
        add_to_counter(b0, b1, b2, DR);

        // Identify cells with exactly 3 neighbors (binary 011)
        uint64_t three_neighbors = b1 & b0 & ~b2;
        // Identify cells with exactly 2 neighbors (binary 010)
        uint64_t two_neighbors   = b1 & ~b0 & ~b2;

        // Birth if has 3 neighbors
        // Survival if has 2 neighbors AND currently alive
        grid = three_neighbors | (grid & two_neighbors);
    }

    void add_to_counter(uint64_t& b0, uint64_t& b1, uint64_t& b2, uint64_t val)
    {
        // Bitwise half-adder logic to increment the 3-bit parallel counter
        uint64_t carry0 = b0 & val;
        b0 ^= val;
        uint64_t carry1 = b1 & carry0;
        b1 ^= carry0;
        b2 |= carry1;
    }

private:

    uint64_t grid;
};

int main()
{
    using namespace std::chrono_literals;

    Life game;
    game.run(20, 200ms);
}
