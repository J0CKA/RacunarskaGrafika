
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::resources {

void Model::draw(const Shader *shader) {
    shader->use();

    // Opaque meshes first so their depth is established before
    // transparent glass/windows are blended over them.
    for (auto &mesh: m_meshes) {
        if (!mesh.is_transparent()) {
            mesh.draw(shader);
        }
    }

    for (auto &mesh: m_meshes) {
        if (mesh.is_transparent()) {
            mesh.draw(shader);
        }
    }
}

void Model::destroy() {
    for (auto &mesh: m_meshes) {
        mesh.destroy();
    }
}
}// namespace engine::resources
