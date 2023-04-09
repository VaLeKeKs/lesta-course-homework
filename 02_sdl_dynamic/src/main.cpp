#include <iostream>
#include <SDL3/SDL_version.h>

std::ostream &operator<<(std::ostream &out, SDL_version version) {
    out << static_cast<int>(version.major) << '.'
        << static_cast<int>(version.minor) << '.'
        << static_cast<int>(version.patch);
    return out;
}

int main() {

    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    std::cout << "compiler: " << compiled << '\n';

    std::cout << "linked: " << linked << '\n';

}
