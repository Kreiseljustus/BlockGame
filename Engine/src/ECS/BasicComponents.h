//
// Created by Kreiseljustus on 10/14/2026.
//

#ifndef BLOCKGAME_BASICCOMPONENTS_H
#define BLOCKGAME_BASICCOMPONENTS_H

#include <string>
#include <glm/glm.hpp>

#include "Rendering/RenderData.h"

namespace Engine::Components {

    using namespace Engine::Rendering;

    struct TagComponent {
        std::string name;
    };

    struct MeshRendererComponent {
        MeshHandle mesh;
        Material material;
    };
}

#endif //BLOCKGAME_BASICCOMPONENTS_H
