//
// Created by crisel on 7/20/26.
//

#ifndef BLOCKGAME_RENDERER_H
#define BLOCKGAME_RENDERER_H

#include <vector>
#include <memory>

#include "Rendering/IRenderBackend.h"
#include "Rendering/RenderData.h"

struct DrawCommand {
    MeshHandle mesh;
    Material material;
    Transform transform;
};

class Renderer {
public:
    explicit Renderer(std::unique_ptr<IRenderBackend> render_backend) : m_RenderBackend(std::move(render_backend)) {}

    void Submit(MeshHandle mesh, Material material, Transform transform);
    void DrawLine(Position3 a, Position3 b, Color color) const;

    void Begin();
    void End();

    void SwitchBackend(std::unique_ptr<IRenderBackend> render_backend) {End(); Flush(); m_RenderBackend.swap(render_backend);}

    //Resource creation methods etc
private:
    void Flush();
private:
    std::unique_ptr<IRenderBackend> m_RenderBackend;

    std::vector<DrawCommand> m_DrawCommands;
};


#endif //BLOCKGAME_RENDERER_H
