#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include "NTL/Graphics.hpp"
#include <string_view>
#include <unordered_map>
#include <memory>

class TextureManager {
public:
    enum class ID {
        NextButton,
        YourMove,
        WinText,
    };

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    static inline TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }

    ntl::Texture& load(ID id, std::string_view filename);

    ntl::Texture& get(ID id);

    inline ntl::Texture& operator[](ID id) {
        return get(id);
    }

    inline void release(ID id) {
        textures_.erase(id);
    }

    inline void releaseAll() {
        textures_.clear();
    }

private:
    TextureManager() = default;
    ~TextureManager() = default;

    std::unordered_map<ID, std::unique_ptr<ntl::Texture>> textures_;
};

#endif // TEXTURE_MANAGER_HPP