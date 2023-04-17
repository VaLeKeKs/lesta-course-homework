#include <iostream>
#include <unordered_map>

#include <SDL3/SDL.h>

void initSDL() {

    using namespace std::literals;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error{"Error: failed call SDL_Init: "s + SDL_GetError()};
    }
}

SDL_Window *createWindow() {

    using namespace std::literals;

    if (auto window{SDL_CreateWindow("SDL3 loop", 640, 480, SDL_WINDOW_OPENGL)}; window != nullptr)
        return window;

    SDL_Quit();
    throw std::runtime_error{"Error: failed call SDL_CreateWindow: "s + SDL_GetError()};
}


void check_keyboard_input(const SDL_Event &e) {
    std::unordered_map<SDL_Keycode, std::string_view> keys{
            {SDLK_w,      "up"},
            {SDLK_s,      "down"},
            {SDLK_d,      "right"},
            {SDLK_a,      "left"},
            {SDLK_LCTRL,  "L CTRL"},
            {SDLK_SPACE,  "space"},
            {SDLK_ESCAPE, "escape"},
            {SDLK_RETURN, "return"},
    };

    if (auto found{keys.find(e.key.keysym.sym)}; found != nullptr) {
        if (e.type == SDL_EVENT_KEY_DOWN) {
            std::cout << found->second << " is pressed\n";
        } else if (e.type == SDL_EVENT_KEY_UP) {
            std::cout << found->second << " is released\n";
        }
    }
}

int main() {
    try {
        initSDL();
        auto window{createWindow()};

        bool continue_loop = true;

        while (continue_loop) {
            SDL_Event sdlEvent;

            while (SDL_PollEvent(&sdlEvent)) {
                switch (sdlEvent.type) {
                    case SDL_EVENT_KEY_DOWN:
                        [[fallthrough]];
                    case SDL_EVENT_KEY_UP:
                        check_keyboard_input(sdlEvent);
                        break;
                    case SDL_EVENT_QUIT:
                        continue_loop = false;
                        break;
                    default:
                        break;
                }
            }
        }

        SDL_DestroyWindow(window);

        SDL_Quit();

        return EXIT_SUCCESS;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (...){
        std::cerr<<"Unknown error.\n";
        return EXIT_FAILURE;
    }
}
