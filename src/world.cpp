#include "world.hpp"

/* --- public --- */

WorldMgr::BodyId WorldMgr::add(const BodyInit init, const BodyType type, const ssize_t tetrIdx, const Color color, const BodyId want) {
    if (tetrIdx >= static_cast<ssize_t>(Tetromino::TETROMINO_COUNT))
        throw std::runtime_error("add: Invalid tetromino index.");

    BodyId id = newId(want);

    b2BodyDef bodyDef;
    bodyDef.type = (type == BodyType::STATIC) ? b2_staticBody : b2_dynamicBody;
    bodyDef.position.Set(init.x * UNIT, init.y * UNIT);
    bodyDef.angle = init.angle * DEG2RAD;
    bodyDef.linearDamping = 0.025f;

    b2Body* body = world.CreateBody(&bodyDef);

    const float blockWidth = init.w * UNIT;
    const float blockHeight = init.h * UNIT;

    if (tetrIdx < 0) {

        b2PolygonShape shape;
        shape.SetAsBox(
            blockWidth / 2.0f, 
            blockHeight / 2.0f
        );

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = init.density;
        fixtureDef.friction = init.friction;
        fixtureDef.restitution = init.restitution;
        body->CreateFixture(&fixtureDef);

    } else {

        const Tetromino::TetrominoArray& shapeArray = Tetromino::SHAPES[tetrIdx];
        const Vector2 centroid = Tetromino::getCentroid(shapeArray, blockWidth, blockHeight);

        bodyDef.position.Set(init.x * UNIT - centroid.x, init.y * UNIT - centroid.y);

        for (const Vector2& square : Tetromino::getSquares(shapeArray, blockWidth, blockHeight)) {
            b2PolygonShape blockShape;
            blockShape.SetAsBox(
                blockWidth / 2.0f, 
                blockHeight / 2.0f, 
                { square.x - centroid.x, square.y - centroid.y }, 
                0.0f
            );

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &blockShape;
            fixtureDef.density = init.density;
            fixtureDef.friction = init.friction;
            fixtureDef.restitution = init.restitution;

            body->CreateFixture(&fixtureDef);
        }
    }

    bodyMap[id] = { .body = body, .tetromino = tetrIdx, .color = color, .details = init };
    return id;
}


void WorldMgr::remove(const BodyId id) {
    if (bodyMap.find(id) == bodyMap.end())
        throw std::runtime_error("remove: Body identifier not found.");

    world.DestroyBody(bodyMap[id].body);
    bodyMap.erase(id);
    freedIds.insert(id);
}

size_t WorldMgr::count() const {
    return bodyMap.size();
}

void WorldMgr::update(const float dt) {
    world.Step(dt, 6, 2);
}

void WorldMgr::draw() const {
    for (const std::pair<const BodyId, Body>& entry : bodyMap) {
        const Body& identBody = entry.second;
        
        const b2Vec2 position = identBody.body->GetPosition();
        const float rad = identBody.body->GetAngle();
        const float deg = rad * RAD2DEG;

        if (identBody.tetromino < 0) {

            Rectangle rec = {
                position.x / UNIT,
                position.y / UNIT,
                identBody.details.w,
                identBody.details.h
            };

            DrawRectanglePro(rec, { identBody.details.w / 2.0f, identBody.details.h / 2.0f }, deg, identBody.color);

        } else {
            // NOTE: possible code duplication in PlayerCursor
            const Tetromino::TetrominoArray& shapeArray = Tetromino::SHAPES[identBody.tetromino];
            const Vector2 centroid = Tetromino::getCentroid(shapeArray, identBody.details.w, identBody.details.h);

            for (Vector2& square : Tetromino::getSquares(shapeArray, identBody.details.w, identBody.details.h)) {
                MathUtils::rot2D(square, centroid, rad);
                square.x += position.x / UNIT - centroid.x;
                square.y += position.y / UNIT - centroid.y;

                DrawRectanglePro(
                    { square.x, square.y, identBody.details.w, identBody.details.h }, 
                    { identBody.details.w / 2.0f, identBody.details.h / 2.0f }, 
                    deg, 
                    identBody.color
                );
            }
        }
    }
}

void WorldMgr::prune(const float radius, const float x, const float y) {
    for (std::unordered_map<BodyId, Body>::iterator it = bodyMap.begin(); it != bodyMap.end(); ) {
        b2Vec2 position = it->second.body->GetPosition();
        position.x -= x * UNIT;
        position.y -= y * UNIT;
        
        if (position.x * position.x + position.y * position.y > radius * radius * UNIT * UNIT) {
            world.DestroyBody(it->second.body);
            freedIds.insert(it->first);
            it = bodyMap.erase(it);
        } 
        
        else ++it;
    }
}

void WorldMgr::clear() {
    for (const std::pair<const BodyId, Body>& entry : bodyMap)
        world.DestroyBody(entry.second.body);

    bodyMap.clear();
}

void WorldMgr::applyAll(const float forceX, const float forceY) {
    for (const std::pair<const BodyId, Body>& entry : bodyMap)
        entry.second.body->ApplyForceToCenter({ forceX, forceY }, true);
}

/* --- private --- */

WorldMgr::BodyId WorldMgr::newId(BodyId want) {
    if (want < -1)
        throw std::runtime_error("newId: Invalid body identifier.");

    if (want != -1) {
        if (bodyMap.find(want) != bodyMap.end())
            throw std::runtime_error("newId: Body identifier already used.");
        return want;   
    } 
    
    if (!freedIds.empty()) {
        BodyId id = *freedIds.begin();
        freedIds.erase(freedIds.begin());
        return id;
    }
    
    else return bodyMap.size();
}

