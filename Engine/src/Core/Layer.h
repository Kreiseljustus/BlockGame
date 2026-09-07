//
// Created by Kreiseljustus on 8/16/2026.
//

#ifndef BLOCKGAME_LAYER_H
#define BLOCKGAME_LAYER_H

#include <Core/Event.h>

namespace Engine::Core {
    class Layer {
    public:
        Layer();
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate([[maybe_unused]] float deltaTime) {}
        virtual void OnImGuiRender() {}
        //Soon
        virtual void OnEvent([[maybe_unused]] Event& event) {}
    };
}


#endif //BLOCKGAME_LAYER_H
