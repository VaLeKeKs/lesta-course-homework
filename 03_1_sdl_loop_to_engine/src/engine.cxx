//
// Created by valentin on 17.04.23.
//

#include "engine.hxx"

#include <unordered_map>
#include <optional>
#include <SDL3/SDL.h>
#include <stdexcept>

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
        {Event::turn_off, "turn_off"}
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

    bool uninitialize() override{
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool readInput(Event& event) override {
        SDL_Event sdlEvent;

        if(SDL_PollEvent(&sdlEvent)){
            if(sdlEvent.type==SDL_EVENT_KEY_DOWN || sdlEvent.type==SDL_EVENT_KEY_UP){
                if(auto e{checkInput(sdlEvent)}){
                    event=*e;
                    return true;
                }
            }

            if(sdlEvent.type==SDL_EVENT_QUIT) {
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
    out<<event_names.at(event);
    return out;
}

engine::~engine() = default;