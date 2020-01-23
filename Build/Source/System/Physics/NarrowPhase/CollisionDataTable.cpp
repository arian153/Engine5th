#include "CollisionDataTable.hpp"
#include "../Resolution/ContactManifold.hpp"
#include "../Dynamics/ColliderSet.hpp"
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    CollisionDataTable::CollisionDataTable()
    {
    }

    CollisionDataTable::~CollisionDataTable()
    {
    }

    void CollisionDataTable::Initialize(std::unordered_multimap<size_t, ContactManifold>* data_table)
    {
        m_manifold_table = data_table;
    }

    void CollisionDataTable::Shutdown()
    {
        m_key_table.clear();
    }

    void CollisionDataTable::SendHasCollision(RigidBody* a, RigidBody* b, bool was_collision)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_state_table.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.body_a == a && (*it).second.body_b == b) ||
                    ((*it).second.body_b == a && (*it).second.body_a == b))
                {
                    // in this case, keep previous contact data for collision event.
                    is_duplicated = true;
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
                CollisionStateData collision_data;
                collision_data.body_a = a;
                collision_data.body_b = b;

                if (was_collision == true)
                {
                    collision_data.state = CollisionState::Persist;
                }
                else
                {
                    collision_data.state = CollisionState::Start;
                }
                m_state_table.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionStateData collision_data;
            collision_data.body_a = a;
            collision_data.body_b = b;

            if (was_collision == true)
            {
                collision_data.state = CollisionState::Persist;
            }
            else
            {
                collision_data.state = CollisionState::Start;
            }

            m_state_table.emplace(key, collision_data);
        }
    }

    void CollisionDataTable::SendNotCollision(RigidBody* a, RigidBody* b, bool was_collision)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_state_table.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.body_a == a && (*it).second.body_b == b) ||
                    ((*it).second.body_b == a && (*it).second.body_a == b))
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
                CollisionStateData collision_data;
                collision_data.body_a = a;
                collision_data.body_b = b;

                if (was_collision == true)
                {
                    collision_data.state = CollisionState::End;
                }
                else
                {
                    collision_data.state = CollisionState::None;
                }
                m_state_table.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionStateData collision_data;
            collision_data.body_a = a;
            collision_data.body_b = b;

            if (was_collision == true)
            {
                collision_data.state = CollisionState::End;
            }
            else
            {
                collision_data.state = CollisionState::None;
            }
            m_state_table.emplace(key, collision_data);
        }
    }

    void CollisionDataTable::SendInvalidCollision(RigidBody* a, RigidBody* b)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   key_range = m_state_table.equal_range(key);
        if (key_range.first != key_range.second)
        {
            bool is_duplicated = false;
            for (auto it = key_range.first; it != key_range.second; ++it)
            {
                if (((*it).second.body_a == a && (*it).second.body_b == b) ||
                    ((*it).second.body_b == a && (*it).second.body_a == b))
                {
                    // in this case, keep previous contact data for collision event.
                    is_duplicated = true;
                    (*it).second.state = CollisionState::Invalid;
                    break;
                }
            }
            if (is_duplicated == false)
            {
                //found same key of pair. 
                //but this data is new data, so add a new collision data.
                CollisionStateData collision_data;
                collision_data.body_a = a;
                collision_data.body_b = b;

                collision_data.state = CollisionState::Invalid;
                m_state_table.emplace(key, collision_data);
            }
        }
        else
        {
            //found new pair. add a new collision data.
            CollisionStateData collision_data;
            collision_data.body_a = a;
            collision_data.body_b = b;

            collision_data.state = CollisionState::Invalid;
            m_state_table.emplace(key, collision_data);
        }
    }

    void CollisionDataTable::ValidateKey(RigidBody* a, RigidBody* b)
    {
        auto key_range_a = m_key_table.equal_range(a);
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
                m_key_table.emplace(a, b);
            }
        }
        else
        {
            m_key_table.emplace(a, b);
        }


        auto key_range_b = m_key_table.equal_range(b);
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
                m_key_table.emplace(b, a);
            }
        }
        else
        {
            m_key_table.emplace(b, a);
        }
    }

    size_t CollisionDataTable::GenerateKey(RigidBody* a, RigidBody* b)
    {
        return reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
    }

    size_t CollisionDataTable::GenerateKey(ColliderSet* a, ColliderSet* b)
    {
        return reinterpret_cast<size_t>(a->m_rigid_body) + reinterpret_cast<size_t>(b->m_rigid_body);
    }

    size_t CollisionDataTable::GenerateKey(ColliderPrimitive* a, ColliderPrimitive* b)
    {
        return reinterpret_cast<size_t>(a->m_rigid_body) + reinterpret_cast<size_t>(b->m_rigid_body);
    }

    size_t CollisionDataTable::GenerateKey(ContactManifold* manifold)
    {
        return reinterpret_cast<size_t>(manifold->m_body_a) + reinterpret_cast<size_t>(manifold->m_body_b);
    }

    auto CollisionDataTable::FindAssociatedPairs(RigidBody* key)
    {
        return m_key_table.equal_range(key);
    }

    auto CollisionDataTable::FindCollisionDatas(RigidBody* a, RigidBody* b, size_t at)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);

        Vector3Pair result;

        auto   data_range = m_manifold_table->equal_range(key);
        if (data_range.first != data_range.second)
        {
            for (auto it = data_range.first; it != data_range.second; ++it)
            {
                if (it->second.m_body_a == a && it->second.m_body_b == b)
                {
                    size_t count = it->second.ContactsCount();
                    size_t index = at < count ? at : 0;
                    result.a = it->second.contacts.at(index).local_position_a;
                    result.b = it->second.contacts.at(index).local_position_b;
                    return result;
                }
                else if (it->second.m_body_b == a && it->second.m_body_a == b)
                {
                    size_t count = it->second.ContactsCount();
                    size_t index = at < count ? at : 0;
                    result.b = it->second.contacts.at(index).local_position_a;
                    result.a = it->second.contacts.at(index).local_position_b;
                    return result;
                }
            }
        }

        //result.a = Math::Vector3::INVALID;
        //result.b = Math::Vector3::INVALID;
        return result;
    }

    auto CollisionDataTable::FindColisionState(RigidBody* a, RigidBody* b)
    {
        size_t key = reinterpret_cast<size_t>(a) + reinterpret_cast<size_t>(b);
        auto   data_range = m_state_table.equal_range(key);
        if (data_range.first != data_range.second)
        {
            for (auto it = data_range.first; it != data_range.second; ++it)
            {
                if ((it->second.body_a == a && it->second.body_b == b) ||
                    (it->second.body_b == a && it->second.body_a == b))
                {
                    return it->second.state;
                }
            }
        }
        return CollisionState::None;
    }

  }
