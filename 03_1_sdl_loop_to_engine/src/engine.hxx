//
// Created by valentin on 17.04.23.
//

#ifndef INC_03_1_SDL_LOOP_TO_ENGINE_ENGINE_HXX
#define INC_03_1_SDL_LOOP_TO_ENGINE_ENGINE_HXX
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


#endif //INC_03_1_SDL_LOOP_TO_ENGINE_ENGINE_HXX
