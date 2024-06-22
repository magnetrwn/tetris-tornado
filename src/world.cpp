#include "world.hpp"

/* --- public --- */

WorldMgr::BodyId WorldMgr::add(const BodyInit init, const BodyType type, const ssize_t tetrIdx, const BodyId want) {
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

    bodyMap[id] = { .body = body, .tetromino = tetrIdx, .details = init };
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
        
        const b2Vec2 position = entry.second.body->GetPosition();
        const float rad = entry.second.body->GetAngle();
        const float deg = rad * RAD2DEG;

        if (entry.second.tetromino < 0) {

            Rectangle rec = {
                position.x / UNIT,
                position.y / UNIT,
                entry.second.details.w,
                entry.second.details.h
            };

            DrawRectanglePro(rec, { entry.second.details.w / 2.0f, entry.second.details.h / 2.0f }, deg, WHITE);

        } else {

            const Tetromino::TetrominoArray& shapeArray = Tetromino::SHAPES[entry.second.tetromino];
            const Vector2 centroid = Tetromino::getCentroid(shapeArray, entry.second.details.w, entry.second.details.h);

            for (Vector2& square : Tetromino::getSquares(shapeArray, entry.second.details.w, entry.second.details.h)) {
                MathUtils::rot2D(square, centroid, rad);

                DrawRectanglePro(
                    { 
                        position.x / UNIT + square.x - centroid.x,
                        position.y / UNIT + square.y - centroid.y,
                        entry.second.details.w, 
                        entry.second.details.h
                    }, { entry.second.details.w / 2.0f, entry.second.details.h / 2.0f }, deg, WHITE
                );
                    /*DrawCircle(position.x / UNIT + square.x, position.y / UNIT + square.y, 3.0f, BLUE);
                    DrawCircle(position.x / UNIT + centroid.x, position.y / UNIT + centroid.y, 3.0f, RED);
                    for (const b2Fixture* fixture = entry.second.body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
                        const b2PolygonShape* shape = static_cast<const b2PolygonShape*>(fixture->GetShape());
                        for (int i = 0; i < 4; ++i)
                            DrawCircle(
                                position.x / UNIT + shape->m_vertices[i].x / UNIT,
                                position.y / UNIT + shape->m_vertices[i].y / UNIT,
                                3.0f,
                                GREEN
                            );
                    }*/
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

