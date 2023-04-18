//
// Created by valentin on 17.04.23.
//

#ifndef INC_03_3_SDL_LOOP_HOT_RELOAD_ENGINE_HXX
#define INC_03_3_SDL_LOOP_HOT_RELOAD_ENGINE_HXX
#include <string>
#include <string_view>
#include <iosfwd>

enum class Event {
    left_pressed,
    left_released,
    right_pressed,
    right_released,
    up_pressed,
    up_released,
    down_pressed,
    down_released,
    space_pressed,
    space_released,
    lctrl_pressed,
    lctrl_released,
    button1_pressed,
    button1_released,
    button2_pressed,
    button2_released,

    turn_off,


};

std::ostream& operator<<(std::ostream& out, const Event& event);

class engine;

engine* create_engine();
void destroy_engine(engine* e);

class engine {
public:
    virtual ~engine();
    virtual std::string initialize(std::string_view config)=0;
    virtual bool readInput(Event& event)=0;
    virtual bool uninitialize()=0;
};

class game
{
public:
    virtual ~game() = default;
    virtual void initialize() = 0;
    virtual void onEvent(Event event) = 0;
    virtual void update() = 0;
    virtual void render() const = 0;
};

extern "C" game* create_game(engine* engine);


#endif //INC_03_3_SDL_LOOP_HOT_RELOAD_ENGINE_HXX
