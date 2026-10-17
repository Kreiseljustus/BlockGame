//
// Created by Kreiseljustus on 10/15/2026.
//

#ifndef BLOCKGAME_ISYSTEM_H
#define BLOCKGAME_ISYSTEM_H

namespace Engine {
    class Scene;

    class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void OnAttach(Scene&) {}
        virtual void OnUpdate(Scene& scene, float dt) = 0;
    };
}

#endif //BLOCKGAME_ISYSTEM_H
