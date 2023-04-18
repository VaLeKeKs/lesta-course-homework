//
// Created by valentin on 17.04.23.
//

#include "engine.hxx"

#include <unordered_map>
#include <optional>
#include <SDL3/SDL.h>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <thread>

using namespace std::literals;

std::unordered_map<Event, std::string_view> event_names{
        {Event::up_pressed,       "up_pressed"},
        {Event::up_released,      "up_released"},
        {Event::left_pressed,     "left_pressed"},
        {Event::left_released,    "left_released"},
        {Event::right_pressed,    "right_pressed"},
        {Event::right_released,   "right_released"},
        {Event::down_pressed,     "down_pressed"},
        {Event::down_released,    "down_released"},
        {Event::space_pressed,    "space_pressed"},
        {Event::space_released,   "space_released"},
        {Event::button1_pressed,  "button1_pressed"},
        {Event::button1_released, "button1_released"},
        {Event::button2_pressed,  "button2_pressed"},
        {Event::button2_released, "button2_released"},
        {Event::lctrl_pressed,    "lctrl_pressed"},
        {Event::lctrl_released,   "lctrl_released"},
        {Event::turn_off,         "turn_off"}
};

struct bind {
    Event event_pressed{};
    Event event_released{};
};

std::optional<Event> checkInput(SDL_Event &event) {
    std::unordered_map<SDL_Keycode, bind> keys{
            {SDLK_d,      bind{Event::right_pressed, Event::right_released}},
            {SDLK_a,      bind{Event::left_pressed, Event::left_released}},
            {SDLK_w,      bind{Event::up_pressed, Event::up_released}},
            {SDLK_s,      bind{Event::down_pressed, Event::down_released}},
            {SDLK_SPACE,  bind{Event::space_pressed, Event::space_released}},
            {SDLK_LCTRL,  bind{Event::lctrl_pressed, Event::lctrl_released}},
            {SDLK_ESCAPE, bind{Event::button1_pressed, Event::button1_released}},
            {SDLK_RETURN, bind{Event::button2_pressed, Event::button2_released}}
    };

    if (auto found{keys.find(event.key.keysym.sym)}; found != keys.end()) {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            return found->second.event_pressed;
        } else if (event.type == SDL_EVENT_KEY_UP) {
            return found->second.event_released;
        }
    }

    return std::nullopt;
}

class engine_impl final : public engine {
public:

    std::string initialize(std::string_view config) override {
        initSDL();
        window = createWindow();
        return "";
    }

    bool uninitialize() override {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool readInput(Event &event) override {
        SDL_Event sdlEvent;

        if (SDL_PollEvent(&sdlEvent)) {
            if (sdlEvent.type == SDL_EVENT_KEY_DOWN || sdlEvent.type == SDL_EVENT_KEY_UP) {
                if (auto e{checkInput(sdlEvent)}) {
                    event = *e;
                    return true;
                }
            }

            if (sdlEvent.type == SDL_EVENT_QUIT) {
                event = Event::turn_off;
                return true;
            }
        }

        return false;
    }

private:

    SDL_Window *window{};

    void initSDL() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
            throw std::runtime_error{"Error: failed to call SDL_Init."s + SDL_GetError()};
        }
    }

    SDL_Window *createWindow() {
        if (auto window{SDL_CreateWindow("SDL3 loop_to_engine", 640, 480, SDL_WINDOW_OPENGL)}; window != nullptr) {
            return window;
        }

        SDL_Quit();
        throw std::runtime_error{"Error: failed to call SDL_CreateWindow."s + SDL_GetError()};
    }


};

static bool alreadyExist{false};

engine *create_engine() {
    if (alreadyExist) {
        throw std::runtime_error{"Error: engine is already create."};
    }
    alreadyExist = true;
    engine *result = new engine_impl();
    return result;
}

void destroy_engine(engine *e) {
    if (!alreadyExist) {
        throw std::runtime_error{"Error: engine not created."};
    }
    if (e == nullptr) {
        throw std::runtime_error{"Error: engine is 0."};
    }
    delete e;
}

std::ostream &operator<<(std::ostream &out, const Event &event) {
    out << event_names.at(event);
    return out;
}

engine::~engine() = default;

game *reload_game(game *old, std::string_view library_name,
                  std::string_view tmp_library_name, engine &engine, void *&old_handle) {
    if (old) {
        delete old;
        SDL_UnloadObject(old_handle);
    }

    std::filesystem::remove(tmp_library_name);
    std::filesystem::copy(library_name, tmp_library_name);

    auto game_handle{SDL_LoadObject(tmp_library_name.data())};
    if (game_handle == nullptr) {
        std::cerr << "Failed to SDL_LoadObject\n";
        return nullptr;
    }

    old_handle = game_handle;

    auto create_game_func_ptr{SDL_LoadFunction(game_handle, "create_game")};
    if (create_game_func_ptr == nullptr) {
        std::cerr << "Failed to SDL_LoadFunction\n";
        return nullptr;
    }

    using Create_Game = decltype(&create_game);
    auto create_game_linked{reinterpret_cast<Create_Game>(create_game_func_ptr)};

    return create_game_linked(&engine);
}

int main() {
    try {
        auto engine{create_engine()};
        engine->initialize("");
        std::cout << "start app" << std::endl;

        std::string_view libraryName{"libgame.so"};
        std::string_view tempLibraryName{"./temp.dll"};
        void *gameLibraryHandle{};
        auto game{reload_game(nullptr, libraryName, tempLibraryName, *engine, gameLibraryHandle)};
        auto timeDuringLoading{std::filesystem::last_write_time(libraryName)};
        game->initialize();

        bool continue_loop{};
        while (!continue_loop) {
            auto currentWriteTime{std::filesystem::last_write_time(libraryName)};
            if (currentWriteTime != timeDuringLoading) {
                while (true) {
                    std::this_thread::sleep_for(100ms);
                    auto nextWriteTime{std::filesystem::last_write_time(libraryName)};

                    if (nextWriteTime != currentWriteTime)
                        currentWriteTime = nextWriteTime;
                    else
                        break;
                }

                std::cout << "reloading game\n";
                game = reload_game(game, libraryName, tempLibraryName, *engine, gameLibraryHandle);

                if (game == nullptr) {
                    std::cerr << "next attempt to reload game\n";
                    continue;
                }

                timeDuringLoading = currentWriteTime;
            }

            Event event{};
            while (engine->readInput(event)) {
                std::cout << event << '\n';

                if (event == Event::turn_off) {
                    continue_loop = true;
                    break;
                }

                game->onEvent(event);
            }

            if (event == Event::turn_off) break;
            game->update();
            game->render();
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
