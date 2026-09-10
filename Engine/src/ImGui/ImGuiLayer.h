//
// Created by Kreiseljustus on 8/17/2026.
//

#ifndef BLOCKGAME_IMGUILAYER_H
#define BLOCKGAME_IMGUILAYER_H
#include "Core/Layer.h"

namespace Engine {
    class ImGuiLayer : public Core::Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Core::Event& event) override;

        static void Begin();
        static void End();
    };
}


#endif //BLOCKGAME_IMGUILAYER_H
