#include "TextureManager.hpp"

ntl::Texture& TextureManager::load(ID id, std::string_view filename) {
    if (auto itr = textures_.find(id); itr != textures_.end()) return *itr->second;

    auto texture = std::make_unique<ntl::Texture>();
    texture->loadFromFile(filename);

    auto& ref = *texture;
    textures_[id] = std::move(texture);
    return ref;
}

ntl::Texture& TextureManager::get(ID id) {
    if (auto itr = textures_.find(id); itr != textures_.end()) return *itr->second;

    throw std::runtime_error("Texture not loaded");
}