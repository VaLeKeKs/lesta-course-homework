#include <stdexcept>
#include <array>
#include <thread>
#include <iostream>
#include <iomanip>

#include "engine.hxx"

using namespace std::literals;

class ConsoleGame : public game {
private:
    int percent{};
    std::string symbol = "❚";
    std::string loadingPanel{};

public:
    static constexpr int max_percent{100};

    void initialize() override {}

    void onEvent(Event event) override {}

    void update() override {
        if (percent >= max_percent) {
            loadingPanel.clear();
            percent = 0;
        }
        loadingPanel += symbol;
        percent += 2;
        std::this_thread::sleep_for(150ms);
    }

    void render() const override {
        std::system("clear");
        std::cout << std::setw(max_percent/2) << std::left << loadingPanel
                  << "\n\t\t  Loading: " << percent << '%' << std::endl;
    }
};

game *create_game(engine *engine) {
    if (engine == nullptr) throw std::runtime_error{"Error : create_game : engine is nullptr"s};
    return new ConsoleGame{};
}