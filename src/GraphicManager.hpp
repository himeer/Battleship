#ifndef GRAPHIC_MANAGER_HPP
#define GRAPHIC_MANAGER_HPP

#include "NTL/Graphics.hpp"

#include <set>
#include <memory>
#include <concepts>
#include <functional>
#include <optional>

template<class Drawable>
concept HasBounds = requires(Drawable t) {
    { t.getBounds() } -> std::same_as<ntl::FloatRect>;
};

template<class Drawable>
concept HasNotBounds = !HasBounds<Drawable>;

class GraphicManager : public ntl::Drawable {
public:
    template<class Drawable, class... Args>
    Drawable* add(int zLayer, Args&&... args) {
        return dynamic_cast<Drawable*>(objects_.emplace(
            getMethod_getBounds<Drawable>(),
            zLayer,
            std::make_unique<Drawable>(args...)
        ).first->drawable.get());
    }

    void remove(ntl::Drawable *drawable) {
        for (auto itr = objects_.begin(); itr != objects_.end(); ++itr) {
            if (itr->drawable.get() == drawable) {
                objects_.erase(itr);
                return;
            }
        }
    }

    std::optional<std::pair<Drawable*, ntl::FloatRect>> getTouched(ntl::Vector2f point) const {
        for (auto itr = objects_.begin(); itr != objects_.end(); ++itr) {
            if (itr->getBounds) {
                auto globalBounds = itr->getBounds(*itr->drawable);
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
        for (auto itr = objects_.begin(); itr != objects_.end(); ++itr) {
            if (itr->drawable.get() == drawable) {
                auto objectNode = objects_.extract(itr);
                objectNode.value().zLayer = zLayer;
                objects_.insert(std::move(objectNode));
                return true;
            }
        }

        return false;
    }

private:
    struct Object {
        int zLayer;
        std::unique_ptr<ntl::Drawable> drawable;
        std::function<ntl::FloatRect(const ntl::Drawable&)> getBounds;

        Object(Object &&other) :
            zLayer(other.zLayer),
            drawable(std::move(other.drawable)),
            getBounds(other.getBounds)
        {}

        Object(
            std::function<ntl::FloatRect(const ntl::Drawable&)> getBounds,
            int zLayer,
            std::unique_ptr<ntl::Drawable> &&drawable
        ) :
            zLayer(zLayer),
            drawable(std::move(drawable)),
            getBounds(getBounds)
        {}

        auto operator<=>(const Object &other) const {
            if (zLayer == other.zLayer) return drawable <=> other.drawable;
            return zLayer <=> other.zLayer;
        }

        bool operator==(const Object &other) const {
            return drawable == other.drawable;
        }
    };

    std::set<Object> objects_;

    void draw(ntl::Window &window) const override {
        for (const auto &object : objects_) target.draw(*object.drawable, states);
    }

    template<HasGlobalBounds Drawable>
    decltype(Object::getBounds) getMethod_getBounds() {
        return [](const ntl::Drawable &x) {
            return dynamic_cast<const Drawable&>(x).getBounds();
        };
    }

    template<HasNotGlobalBounds Drawable>
    decltype(Object::getBounds) getMethod_getBounds() {
        return nullptr;
    }
};

#endif // GRAPHIC_MANAGER_HPP