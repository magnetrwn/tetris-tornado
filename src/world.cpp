#include "world.hpp"

/* --- public --- */

WorldMgr::bodyid_t WorldMgr::add(const BodyInit init, const BodyType type, const bodyid_t want) {
    bodyid_t id = newId(want);

    b2BodyDef bodyDef;
    bodyDef.type = (type == BodyType::STATIC) ? b2_staticBody : b2_dynamicBody;
    bodyDef.position.Set(init.x * UNIT, init.y * UNIT);
    bodyDef.angle = init.angle * DEG2RAD;
    bodyDef.linearDamping = 0.025f;

    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(init.w * UNIT / 2.0f, init.h * UNIT / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = init.density;
    fixtureDef.friction = init.friction;
    fixtureDef.restitution = init.restitution;
    body->CreateFixture(&fixtureDef);

    bodyMap[id] = { .body = body, .details = init };
    return id;
}

void WorldMgr::remove(const bodyid_t id) {
    if (bodyMap.find(id) == bodyMap.end())
        throw std::runtime_error("remove: Body identifier not found.");

    world.DestroyBody(bodyMap[id].body);
    bodyMap.erase(id);
    freedIds.insert(id);
}

size_t WorldMgr::count() const {
    return bodyMap.size();
}

void WorldMgr::update() {
    world.Step(0.01667f, 6, 2);
}

void WorldMgr::draw() const {
    BeginDrawing();
        ClearBackground({ 53, 53, 53, 255 });

        for (const std::pair<const bodyid_t, Body>& entry : bodyMap) {
            b2Vec2 position = entry.second.body->GetPosition();
            float angle = entry.second.body->GetAngle() * RAD2DEG;

            Rectangle rec = {
                position.x / UNIT - (entry.second.details.w / 2.0f) * UNIT,
                position.y / UNIT - (entry.second.details.h / 2.0f) * UNIT,
                entry.second.details.w,
                entry.second.details.h
            };

            Vector2 origin = {
                (entry.second.details.w / 2.0f),
                (entry.second.details.h / 2.0f)
            };

            DrawRectanglePro(rec, origin, angle, WHITE);
        }

    EndDrawing();
}

void WorldMgr::prune(const float radius, const float x, const float y) {
    for (std::unordered_map<bodyid_t, Body>::iterator it = bodyMap.begin(); it != bodyMap.end();) {
        b2Vec2 position = it->second.body->GetPosition();
        position.x -= x * UNIT;
        position.y -= y * UNIT;
        
        if (position.x * position.x + position.y * position.y > radius * radius * UNIT * UNIT) {
            world.DestroyBody(it->second.body);
            freedIds.insert(it->first);
            it = bodyMap.erase(it);
        } 
        
        else it++;
    }
}

void WorldMgr::clear() {
    for (const std::pair<const bodyid_t, Body>& entry : bodyMap)
        world.DestroyBody(entry.second.body);

    bodyMap.clear();
}

/* --- protected --- */

WorldMgr::bodyid_t WorldMgr::newId(bodyid_t want) {
    if (want < -1)
        throw std::runtime_error("newId: Invalid body identifier.");

    if (want != -1) {
        if (bodyMap.find(want) != bodyMap.end())
            throw std::runtime_error("newId: Body identifier already used.");
        return want;   
    } 
    
    if (!freedIds.empty()) {
        bodyid_t id = *freedIds.begin();
        freedIds.erase(freedIds.begin());
        return id;
    }
    
    else return bodyMap.size();
}

