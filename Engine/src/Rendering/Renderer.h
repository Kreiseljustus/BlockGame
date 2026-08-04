//
// Created by crisel on 7/20/26.
//

#ifndef BLOCKGAME_RENDERER_H
#define BLOCKGAME_RENDERER_H

#include <vector>
#include <memory>

#include "Rendering/IRenderBackend.h"
#include "Rendering/RenderData.h"

namespace Engine::Rendering {
    struct DrawCommand {
        MeshHandle mesh;
        Material material;
        Transform transform;
    };

    class Renderer {
    public:
        explicit Renderer(std::unique_ptr<IRenderBackend> render_backend) : m_RenderBackend(std::move(render_backend)) {}

        void Submit(MeshHandle mesh, Material material, Transform transform);
        void DrawLine(glm::vec3 a, glm::vec3 b, Color color) const;

        void Begin(const Camera& camera);
        void End();

        void SwitchBackend(std::unique_ptr<IRenderBackend> render_backend) {End(); Flush(); m_RenderBackend.swap(render_backend);}

        //Resource creation methods etc
        [[nodiscard]] MeshHandle CreateMesh(const MeshData& data) const {return m_RenderBackend->CreateMesh(data);}
        [[nodiscard]] TextureHandle CreateTexture(const TextureParameters& parameters) const {return m_RenderBackend->CreateTexture(parameters);}
        [[nodiscard]] ShaderHandle CreateShader(const ShaderParameters& parameters) const {return m_RenderBackend->CreateShader(parameters);}
    private:
        void Flush();
    private:
        std::unique_ptr<IRenderBackend> m_RenderBackend;

        std::vector<DrawCommand> m_DrawCommands;
    };
}


#endif //BLOCKGAME_RENDERER_H
