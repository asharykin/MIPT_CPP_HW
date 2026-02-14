#include <iostream>
#include <string>
#include <vector>
#include <random>

class Evolution
{
public:
    Evolution() : engine(rd()), rate_dist(0.0, 1.0) {
        alphabet.resize(26);
        std::iota(std::begin(alphabet), std::end(alphabet), 'a');
    }

    void run()
    {
        std::string current_string = generate_initial_string();
        int round = 0;

        while (true)
            {
            int current_dist = calculate_distance(current_string);

            std::cout << "Round " << round << ": " << current_string
                      << " (Distance: " << current_dist << ")" << std::endl;

            if (current_dist == 0)
            {
                break;
            }
            round++;

            std::vector<std::string> copies;
            for (int i = 0; i < copies_size; ++i)
            {
                copies.push_back(mutate(current_string));
            }

            std::string& best_candidate = copies[0];
            int min_distance = calculate_distance(best_candidate);
            for (int i = 1; i < copies_size; ++i)
            {
                int d = calculate_distance(copies[i]);
                if (d < min_distance)
                {
                    min_distance = d;
                    best_candidate = copies[i];
                }
            }

            current_string = best_candidate;
        }
    }

private:
    const std::string target = "methinksitislikeaweasel";
    const int copies_size = 100;
    const double mutation_rate = 0.05;

    std::string alphabet;
    std::random_device rd;
    std::default_random_engine engine;
    std::uniform_real_distribution<double> rate_dist;

    std::string generate_initial_string()
    {
        std::string result;
        result.resize(target.length());
        for (auto& c : result)
        {
            c = alphabet[engine() % alphabet.length()];
        }
        return result;
    }

    int calculate_distance(const std::string& candidate) const
    {
        int distance = 0;
        for (size_t i = 0; i < target.length(); ++i)
        {
            if (target[i] != candidate[i])
            {
                distance++;
            }
        }
        return distance;
    }

    std::string mutate(const std::string& original)
    {
        std::string copy = original;
        for (char& c : copy)
        {
            if (rate_dist(engine) <= mutation_rate)
            {
                c = alphabet[engine() % alphabet.length()];
            }
        }
        return copy;
    }
};

int main()
{
    Evolution simulator;
    simulator.run();
}