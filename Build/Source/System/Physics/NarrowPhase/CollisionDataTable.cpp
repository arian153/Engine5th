#include "CollisionDataTable.hpp"

namespace Engine5
{
    CollisionData::CollisionData()
    {
    }

    CollisionData::~CollisionData()
    {
    }

    CollisionDataTable::CollisionDataTable()
    {
    }

    CollisionDataTable::~CollisionDataTable()
    {
    }

    void CollisionDataTable::Initialize()
    {
    }

    void CollisionDataTable::Shutdown()
    {
        m_collision_data_key_map.clear();
        m_collision_data_map.clear();
    }

    void CollisionDataTable::SendHasCollision(ColliderPrimitive* a, ColliderPrimitive* b, bool was_collision, const Vector3& collision_point_a, const Vector3& collision_point_b)
    {
        size_t key       = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_collision_data_map.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.collider_a == a && (*it).second.collider_b == b) ||
                    ((*it).second.collider_b == a && (*it).second.collider_a == b))
                {
                    // in this case, keep previous contact data for collision event.
                    is_duplicated                  = true;
                    (*it).second.collision_point_a = collision_point_a;
                    (*it).second.collision_point_b = collision_point_b;
                    if (was_collision == true)
                    {
                        (*it).second.state = CollisionState::Persist;
                    }
                    else
                    {
                        (*it).second.state = CollisionState::Start;
                    }
                    break;
                }
            }
            if (is_duplicated == false)
            {
                //found same key of pair. 
                //but this data is new data, so add a new collision data.
                CollisionData collision_data;
                collision_data.collider_a        = a;
                collision_data.collider_b        = b;
                collision_data.collision_point_a = collision_point_a;
                collision_data.collision_point_b = collision_point_b;
                if (was_collision == true)
                {
                    collision_data.state = CollisionState::Persist;
                }
                else
                {
                    collision_data.state = CollisionState::Start;
                }
                m_collision_data_map.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionData collision_data;
            collision_data.collider_a        = a;
            collision_data.collider_b        = b;
            collision_data.collision_point_a = collision_point_a;
            collision_data.collision_point_b = collision_point_b;
            if (was_collision == true)
            {
                collision_data.state = CollisionState::Persist;
            }
            else
            {
                collision_data.state = CollisionState::Start;
            }
            m_collision_data_map.emplace(key, collision_data);
        }
    }

    void CollisionDataTable::SendNotCollision(ColliderPrimitive* a, ColliderPrimitive* b, bool was_collision)
    {
        size_t key       = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_collision_data_map.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.collider_a == a && (*it).second.collider_b == b) ||
                    ((*it).second.collider_b == a && (*it).second.collider_a == b))
                {
                    // in this case, keep previous contact data for collision event.
                    is_duplicated = true;
                    if (was_collision == true)
                    {
                        (*it).second.state = CollisionState::End;
                    }
                    else
                    {
                        (*it).second.state = CollisionState::None;
                    }
                    break;
                }
            }
            if (is_duplicated == false)
            {
                //found same key of pair. 
                //but this data is new data, so add a new collision data.
                CollisionData collision_data;
                collision_data.collider_a = a;
                collision_data.collider_b = b;
                if (was_collision == true)
                {
                    collision_data.state = CollisionState::End;
                }
                else
                {
                    collision_data.state = CollisionState::None;
                }
                m_collision_data_map.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionData collision_data;
            collision_data.collider_a = a;
            collision_data.collider_b = b;
            if (was_collision == true)
            {
                collision_data.state = CollisionState::End;
            }
            else
            {
                collision_data.state = CollisionState::None;
            }
            m_collision_data_map.emplace(key, collision_data);
        }
    }

    void CollisionDataTable::SendInvalidCollision(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        size_t key       = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_collision_data_map.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.collider_a == a && (*it).second.collider_b == b) ||
                    ((*it).second.collider_b == a && (*it).second.collider_a == b))
                {
                    // in this case, keep previous contact data for collision event.
                    is_duplicated      = true;
                    (*it).second.state = CollisionState::Invalid;
                    break;
                }
            }
            if (is_duplicated == false)
            {
                //found same key of pair. 
                //but this data is new data, so add a new collision data.
                CollisionData collision_data;
                collision_data.collider_a = a;
                collision_data.collider_b = b;
                collision_data.state      = CollisionState::Invalid;
                m_collision_data_map.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionData collision_data;
            collision_data.collider_a = a;
            collision_data.collider_b = b;
            collision_data.state      = CollisionState::Invalid;
            m_collision_data_map.emplace(key, collision_data);
        }
    }

    void CollisionDataTable::ValidateKeyMap(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        auto key_range_a = m_collision_data_key_map.equal_range(a);
        if (key_range_a.first != key_range_a.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range_a.first; it != key_range_a.second; ++it)
            {
                if ((*it).second == b)
                {
                    is_duplicated = true;
                    break;
                }
            }
            if (is_duplicated == false)
            {
                m_collision_data_key_map.emplace(a, b);
            }
        }
        else
        {
            m_collision_data_key_map.emplace(a, b);
        }
        auto key_range_b = m_collision_data_key_map.equal_range(b);
        if (key_range_b.first != key_range_b.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range_b.first; it != key_range_b.second; ++it)
            {
                if ((*it).second == a)
                {
                    is_duplicated = true;
                    break;
                }
            }
            if (is_duplicated == false)
            {
                m_collision_data_key_map.emplace(b, a);
            }
        }
        else
        {
            m_collision_data_key_map.emplace(b, a);
        }
    }

    size_t CollisionDataTable::GenerateKey(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        return reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
    }

    auto CollisionDataTable::FindRelatedColliderPairList(ColliderPrimitive* key) 
    {
        return m_collision_data_key_map.equal_range(key);
    }

    std::list<CollisionData> CollisionDataTable::FindCollisionDataList(ColliderPrimitive* key)
    {
        std::list<CollisionData> result;
        auto                     key_range = m_collision_data_key_map.equal_range(key);
        for (auto it = key_range.first; it != key_range.second; ++it)
        {
            auto generated_key = reinterpret_cast<size_t>(key) + reinterpret_cast<size_t>(it->second);
            auto data_range    = m_collision_data_map.equal_range(generated_key);
            if (data_range.first != data_range.second)
            {
                for (auto data_it = data_range.first; data_it != data_range.second; ++data_it)
                {
                    if ((data_it->second.collider_a == key && data_it->second.collider_b == it->second) ||
                        (data_it->second.collider_b == key && data_it->second.collider_a == it->second))
                    {
                        result.push_back(data_it->second);
                        break;
                    }
                }
            }
            else
            {
                //no data error.
            }
        }
        return result;
    }

    CollisionState CollisionDataTable::FindCollisionState(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        size_t key        = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   data_range = m_collision_data_map.equal_range(key);
        if (data_range.first != data_range.second)
        {
            for (auto data_it = data_range.first; data_it != data_range.second; ++data_it)
            {
                if ((data_it->second.collider_a == a && data_it->second.collider_b == b) ||
                    (data_it->second.collider_b == a && data_it->second.collider_a == b))
                {
                    return data_it->second.state;
                }
            }
        }
        return CollisionState::Invalid;
    }
}
