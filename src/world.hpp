#ifndef WORLD_H_
#define WORLD_H_

#include <unordered_map>
#include <stdexcept>
#include <set>
#include <cstdint>

#include "box2d/box2d.h"
#include "raylib.h"

class WorldMgr {
public:
    using bodyid_t = ssize_t;
    constexpr static float UNIT = 0.04f; // 1 px = 0.04 m (so that args for box2d turn 1 px to 0.04 meters)

    enum class BodyType {
        STATIC,
        DYNAMIC
    };

    struct BodyInit {
        constexpr static float DEF_ANGLE = 0.0f;
        constexpr static float DEF_DENSITY = 1.0f;
        constexpr static float DEF_FRICTION = 0.25f;
        constexpr static float DEF_RESTITUTION = 0.66667f;

        float x, y, w, h;
        float angle;
        float density;
        float friction;
        float restitution;

        BodyInit(
            float x = 0.0f, 
            float y = 0.0f, 
            float w = 0.0f, 
            float h = 0.0f, 
            float angle = DEF_ANGLE, 
            float density = DEF_DENSITY, 
            float friction = DEF_FRICTION, 
            float restitution = DEF_RESTITUTION
        ) : x(x), y(y), w(w), h(h), angle(angle), density(density), friction(friction), restitution(restitution) {}
    };

    struct Body {
        b2Body* body;
        BodyInit details;
    };

    bodyid_t add(const BodyInit init, const BodyType type, const bodyid_t want = -1);
    void remove(const bodyid_t id);
    size_t count() const;
    void update();
    void draw() const;
    void prune(const float radius, const float x = 0.0f, const float y = 0.0f);
    void clear();

    WorldMgr(b2World& world) : world(world) {}
    ~WorldMgr() { clear(); }

protected:
    bodyid_t newId(bodyid_t want = -1);

    b2World& world;
    std::unordered_map<bodyid_t, Body> bodyMap;
    std::set<bodyid_t> freedIds;
};

#endif
