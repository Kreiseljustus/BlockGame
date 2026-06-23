//
// Created by crisel on 6/23/26.
//

#include "Input.h"

void Input::handleKey(int key, int scancode, int action, int mods) {
    switch (key) {
        default:
            std::cout << "Input for key " << key << " is unhandled!" << std::endl;
    }
}

void Input::handleResize(const int width, const int height) {
    glViewport(0,0, width, height);
}
