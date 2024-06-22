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

    float centroidX = 0.0f;
    float centroidY = 0.0f;

    if (tetrIdx < 0) {

        b2PolygonShape shape;
        shape.SetAsBox(blockWidth / 2.0f, blockHeight / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = init.density;
        fixtureDef.friction = init.friction;
        fixtureDef.restitution = init.restitution;
        body->CreateFixture(&fixtureDef);

    } else {

        const Tetromino::TetrominoArray& shapeArray = Tetromino::SHAPES[tetrIdx];

        const float blockWidth = init.w * UNIT;
        const float blockHeight = init.h * UNIT;

        size_t count = 0;

        for (size_t i = 0; i < Tetromino::TETROMINO_DEFW; ++i)
            for (size_t j = 0; j < Tetromino::TETROMINO_DEFH; ++j)
                if (shapeArray[j * Tetromino::TETROMINO_DEFW + i]) {
                    centroidX += i * blockWidth;
                    centroidY += j * blockHeight;
                    ++count;
                }

        if (count > 0) {
            centroidX /= count;
            centroidY /= count;
        }

        bodyDef.position.Set(init.x * UNIT - centroidX, init.y * UNIT - centroidY);

        for (size_t i = 0; i < Tetromino::TETROMINO_DEFW; ++i)
            for (size_t j = 0; j < Tetromino::TETROMINO_DEFH; ++j)
                if (shapeArray[j * Tetromino::TETROMINO_DEFW + i]) {
                    b2PolygonShape blockShape;
                    blockShape.SetAsBox(
                        blockWidth / 2.0f, 
                        blockHeight / 2.0f, 
                        { (i * blockWidth) - centroidX, (j * blockHeight) - centroidY }, 
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

    bodyMap[id] = { .body = body, .tetromino = tetrIdx, .details = init, .centroid = { centroidX, centroidY } };
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
            const Vector2 centroid = { 
                position.x / UNIT + entry.second.centroid.x / UNIT, 
                position.y / UNIT + entry.second.centroid.y / UNIT
            };

            for (size_t i = 0; i < Tetromino::TETROMINO_DEFW; ++i)
                for (size_t j = 0; j < Tetromino::TETROMINO_DEFH; ++j)
                    if (shapeArray[j * Tetromino::TETROMINO_DEFW + i]) {
                        Vector2 sqOffset = {
                            position.x / UNIT + (i * entry.second.details.w) - entry.second.centroid.x,
                            position.y / UNIT + (j * entry.second.details.h) - entry.second.centroid.y
                        };

                        MathUtils::rot2D(sqOffset, centroid, rad);

                        Rectangle rec = {
                            sqOffset.x,
                            sqOffset.y,
                            entry.second.details.w,
                            entry.second.details.h
                        };

                        DrawRectanglePro(rec, { entry.second.details.w / 2.0f, entry.second.details.h / 2.0f }, deg, WHITE);
                        //DrawCircle(centroid.x, centroid.y, 3.0f, RED);
                        //DrawCircle(rec.x, rec.y, 3.0f,BLUE);
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

