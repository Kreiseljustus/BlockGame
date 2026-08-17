//
// Created by Kreiseljustus on 8/16/2026.
//

#ifndef BLOCKGAME_LAYER_H
#define BLOCKGAME_LAYER_H

#include <string>

namespace Engine::Core {
    class Layer {
    public:
        Layer();
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnImGuiRender() {}
        //Soon
        virtual void OnEvent() {}
    };
}


#endif //BLOCKGAME_LAYER_H
