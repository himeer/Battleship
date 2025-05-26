#ifndef GRAPHIC_MANAGER_HPP
#define GRAPHIC_MANAGER_HPP

#include "NTL/Graphics.hpp"

#include <set>
#include <memory>
#include <concepts>
#include <functional>
#include <optional>

template<class Drawable>
concept HasGlobalBounds = requires(Drawable t) {
    { t.getGlobalBounds() } -> std::same_as<ntl::FloatRect>;
};

template<class Drawable>
concept HasNotGlobalBounds = !HasGlobalBounds<Drawable>;

class GraphicManager : public ntl::Drawable {
public:
    template<class Drawable, class... Args>
    Drawable* add(int zLayer, Args&&... args) {
        return dynamic_cast<Drawable*>(objects_.emplace(
            getMethod_getGlobalBounds<Drawable>(),
            zLayer,
            std::make_unique<Drawable>(std::forward<Args>(args)...)
        ).first->drawable.get());
    }

    void remove(ntl::Drawable *drawable) {
        auto itr = find(drawable);

        if (itr != objects_.end()) objects_.erase(itr);
    }

    std::optional<std::pair<Drawable*, ntl::FloatRect>> getTouched(ntl::Vector2f point) const {
        for (auto itr = objects_.rbegin(); itr != objects_.rend(); ++itr) {
            if (!itr->hidden && itr->getGlobalBounds) {
                auto globalBounds = itr->getGlobalBounds(*itr->drawable);
                if (globalBounds.contains(point)) {
                    return std::make_pair(itr->drawable.get(), globalBounds);
                }
            }
        }

        return {};
    }

    std::optional<int> getLayer(ntl::Drawable *drawable) const {
        for (const auto &object : objects_) {
            if (object.drawable.get() == drawable) return object.zLayer;
        }

        return {};
    }

    bool setLayer(ntl::Drawable *drawable, int zLayer) {
        auto itr = find(drawable);
        if (itr != objects_.end()) {
            auto objectNode = objects_.extract(itr);
            objectNode.value().zLayer = zLayer;
            objects_.insert(std::move(objectNode));
            return true;
        }

        return false;
    }

    bool setVisibility(ntl::Drawable *drawable, bool isVisible) {
        auto itr = find(drawable);
        if (itr != objects_.end()) {
            auto objectNode = objects_.extract(itr);
            objectNode.value().hidden = !isVisible;
            objects_.insert(std::move(objectNode));
            return true;
        }

        return false;
    }

private:
    struct Object {
        bool hidden = false;
        int zLayer{};
        std::unique_ptr<ntl::Drawable> drawable;
        std::function<ntl::FloatRect(const ntl::Drawable&)> getGlobalBounds;

        Object(Object &&other) :
            hidden(other.hidden),
            zLayer(other.zLayer),
            drawable(std::move(other.drawable)),
            getGlobalBounds(other.getGlobalBounds)
        {}

        Object(
            std::function<ntl::FloatRect(const ntl::Drawable&)> getGlobalBounds,
            int zLayer,
            std::unique_ptr<ntl::Drawable> &&drawable
        ) :
            zLayer(zLayer),
            drawable(std::move(drawable)),
            getGlobalBounds(getGlobalBounds)
        {}

        auto operator<=>(const Object &other) const {
            if (zLayer == other.zLayer) return drawable <=> other.drawable;
            return zLayer <=> other.zLayer;
        }

        bool operator==(const Object &other) const {
            return drawable == other.drawable;
        }
    };

public:
    std::set<Object> objects_;

    void draw(ntl::Window &window, ntl::RenderStates states) const override {
        for (const auto &object : objects_) {
            if (!object.hidden) window.draw(*object.drawable, states);
        }
    }

    template<HasGlobalBounds Drawable>
    decltype(Object::getGlobalBounds) getMethod_getGlobalBounds() {
        return [](const ntl::Drawable &x) {
            return dynamic_cast<const Drawable&>(x).getGlobalBounds();
        };
    }

    template<HasNotGlobalBounds Drawable>
    decltype(Object::getGlobalBounds) getMethod_getGlobalBounds() {
        return nullptr;
    }

    decltype(objects_)::iterator find(ntl::Drawable *drawable) {
        for (auto itr = objects_.begin(); itr != objects_.end(); ++itr) {
            if (itr->drawable.get() == drawable) return itr;
        }

        return objects_.end();
    }
};

#endif // GRAPHIC_MANAGER_HPP