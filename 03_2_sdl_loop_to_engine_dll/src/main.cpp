#include <iostream>
#include <memory>
#include <functional>
#include "engine.hxx"

int main() {
    try {
        std::unique_ptr<engine, std::function<void(engine *e)>> engine{create_engine(), destroy_engine};

        engine->initialize("");

        bool continue_loop = true;
        while (continue_loop) {
            Event event{};

            while (engine->readInput(event)) {
                std::cout << event << '\n';
                if (event == Event::turn_off) {
                    continue_loop = false;
                    break;
                }
            }
        }

        engine->uninitialize();
        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown error." << '\n';
    }
    return EXIT_FAILURE;
}
