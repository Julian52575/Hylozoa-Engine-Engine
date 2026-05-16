#include "EngineFixture.hpp"

TEST_F(HeadlessEngineFixture, EntitySpawnedInScene)
{
    engine.scene().createScene("test scene");
    engine.scene().loadScene("test scene");

    std::string entityName{"Test Entity"};

    auto entity = engine.scene().spawnEntity(entityName);

    ASSERT_TRUE(entity.hasComponent<Hylozoa::Components::Name>());

    EXPECT_EQ(
        entity.getComponent<Hylozoa::Components::Name>().name,
        entityName
    );
}

TEST_F(HeadlessEngineFixture, MultipleEntitiesSpawnedSameFrame)
{
    engine.scene().createScene("test");
    engine.scene().loadScene("test");

    constexpr size_t entityCount = 1000;

    std::vector<Hylozoa::Entity> entities;
    entities.reserve(entityCount);

    for (size_t i = 0; i < entityCount; ++i)
    {
        auto entity = engine.scene().spawnEntity(
            "Entity_" + std::to_string(i)
        );

        entities.push_back(entity);
    }

    EXPECT_EQ(entities.size(), entityCount);

    for (size_t i = 0; i < entityCount; ++i)
    {
        ASSERT_TRUE(
            entities[i].hasComponent<Hylozoa::Components::Name>()
        );

        EXPECT_EQ(
            entities[i].getComponent<Hylozoa::Components::Name>().name,
            "Entity_" + std::to_string(i)
        );
    }
}

TEST_F(HeadlessEngineFixture, SpawnAndDespawnSameFrame)
{
    engine.scene().createScene("test");
    engine.scene().loadScene("test");

    constexpr size_t entityCount = 1000;

    std::vector<Hylozoa::Entity> entities;
    entities.reserve(entityCount);

    for (size_t i = 0; i < entityCount; ++i)
    {
        auto entity = engine.scene().spawnEntity(
            "Entity_" + std::to_string(i)
        );

        entities.push_back(entity);
    }

    for (auto entity : entities)
    {
        entity.destroy();
    }

    engine.runTick();

    for (auto entity : entities)
    {
        EXPECT_FALSE(entity.isValid());
    }
}

TEST_F(HeadlessEngineFixture, SpawnDuringIteration)
{
    engine.scene().createScene("test");
    engine.scene().loadScene("test");

    for (int i = 0; i < 100; ++i)
    {
        engine.scene().spawnEntity("Initial");
    }

    auto view = engine.getRegistry().view<Hylozoa::Components::Name>();

    size_t spawned = 0;

    for (auto entityId : view)
    {
        engine.scene().spawnEntity("Spawned");
        ++spawned;
    }

    EXPECT_EQ(spawned, 100);
}

TEST_F(HeadlessEngineFixture, DestroyDuringIteration)
{
    engine.scene().createScene("test");
    engine.scene().loadScene("test");

    for (int i = 0; i < 100; ++i)
    {
        engine.scene().spawnEntity("Entity");
    }

    auto view = engine.getRegistry().view<Hylozoa::Components::Name>();

    size_t destroyed = 0;

    for (auto entityId : view)
    {
        auto entity = Hylozoa::Entity(entityId, engine.getRegistry());

        entity.destroy();

        ++destroyed;
    }

    engine.runTick();

    EXPECT_EQ(destroyed, 100);
    EXPECT_TRUE(view.empty());
}

TEST_F(HeadlessEngineFixture, MassiveSpawnDespawnStress)
{
    engine.scene().createScene("test");
    engine.scene().loadScene("test");

    constexpr size_t iterations = 100;
    constexpr size_t entitiesPerIteration = 10000;

    for (size_t frame = 0; frame < iterations; ++frame)
    {
        std::vector<Hylozoa::Entity> entities;

        for (size_t i = 0; i < entitiesPerIteration; ++i)
        {
            entities.push_back(
                engine.scene().spawnEntity("Stress")
            );
        }

        for (auto entity : entities)
        {
            entity.destroy();
        }

        engine.runTick();
    }

    auto view = engine.getRegistry().view<Hylozoa::Components::Name>();

    EXPECT_TRUE(view.empty());
}

TEST_F(HeadlessEngineFixture, DestroyedEntityBecomesInvalid)
{
    engine.scene().createScene("test");
    engine.scene().loadScene("test");

    auto entity = engine.scene().spawnEntity("Test");

    auto entityId = entity.getHandle();

    entity.destroy();

    engine.runTick();

    EXPECT_FALSE(entity.isValid());

    auto newEntity = engine.scene().spawnEntity("New");

    EXPECT_NE(newEntity.getHandle(), entityId);
}