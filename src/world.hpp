#ifndef WORLD_H_
#define WORLD_H_

#include <unordered_map>
#include <stdexcept>
#include <set>
#include <cstddef>
#include "box2d/box2d.h"
#include "raylib.h"
#include "tetromino.hpp"
#include "util.hpp"

class WorldMgr {
public:
    using BodyId = ssize_t;
    constexpr static float UNIT = 0.04f; // 1 px = 0.04 m (so that args for box2d turn 1 px to 0.04 meters)

    enum class BodyType {
        STATIC,
        DYNAMIC
    };

    struct BodyInit {
        constexpr static float DEF_ANGLE = 0.0f;
        constexpr static float DEF_DENSITY = 1.0f;
        constexpr static float DEF_FRICTION = 0.25f;
        constexpr static float DEF_RESTITUTION = 0.1875f;

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
        ssize_t tetromino;
        BodyInit details;
        Vector2 centroid;
    };

    BodyId add(const BodyInit init, const BodyType type, const ssize_t tetrIdx = -1, const BodyId want = -1);
    void remove(const BodyId id);
    size_t count() const;
    void update(const float dt);
    void draw() const;
    void prune(const float radius, const float x = 0.0f, const float y = 0.0f);
    void clear();

    BodyId addTetromino(const Tetromino& tetromino, const BodyInit init, const BodyType type, const BodyId want = -1);

    WorldMgr(b2World& world) : world(world) {}
    ~WorldMgr() { clear(); }

private:
    BodyId newId(BodyId want = -1);

    b2World& world;
    std::unordered_map<BodyId, Body> bodyMap;
    std::set<BodyId> freedIds;
};

#endif
