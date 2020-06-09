#include "GridPartition.hpp"
#include "..//Dynamics/RigidBody.hpp"
#include <algorithm>
#include "../ColliderPrimitive/ColliderPrimitive.hpp"

namespace Engine5
{
    GridData::GridData(BoundingAABB* bounding_aabb, GridCell* owner_cell, size_t a, size_t b)
        : aabb_data(bounding_aabb), owner_cell(owner_cell), basis_a_index(a), basis_b_index(b)
    {
        bounding_aabb->m_userdata = this;
        cell_index                = owner_cell->aabb_list.size();
    }

    GridData::~GridData()
    {
    }

    GridCell::GridCell(size_t index, size_t a, size_t b)
        : grid_index(index), basis_a_index(a), basis_b_index(b)
    {
    }

    GridCell::~GridCell()
    {
    }

    GridPartition::GridPartition()
    {
    }

    GridPartition::~GridPartition()
    {
    }

    void GridPartition::Initialize()
    {
        m_a_count = (size_t)ceil((Real)m_width / m_size_of_cell);
        m_b_count = (size_t)ceil((Real)m_height / m_size_of_cell);
        m_grid_cell_list.resize(m_a_count * m_b_count);
        m_center_of_grid[m_axis_normal] = 0.0f;
        m_center_of_grid[m_basis_a]     = static_cast<Real>(m_width) * 0.5f;
        m_center_of_grid[m_basis_b]     = static_cast<Real>(m_height) * 0.5f;
        size_t  size                    = m_grid_cell_list.size();
        Vector3 cell_start(-m_center_of_grid);
        Real    size_of_cell = static_cast<Real>(m_size_of_cell);
        for (size_t i = 0; i < size; i++)
        {
            m_grid_cell_list[i].grid_index    = i;
            size_t a_index                    = i % m_b_count;
            size_t b_index                    = i / m_b_count;
            m_grid_cell_list[i].basis_a_index = a_index;
            m_grid_cell_list[i].basis_b_index = b_index;
            m_grid_cell_list[i].aabb_list.reserve(20);
            Vector3 cell_min;
            cell_min[m_basis_a] = static_cast<Real>(a_index) * size_of_cell;
            cell_min[m_basis_b] = static_cast<Real>(b_index) * size_of_cell;
            Vector3 cell_max;
            cell_max[m_basis_a] = static_cast<Real>(a_index + 1) * size_of_cell;
            cell_max[m_basis_b] = static_cast<Real>(b_index + 1) * size_of_cell;
            m_grid_cell_list[i].cell_boundary.Set(cell_start + cell_min, cell_start + cell_max);
        }
        m_basis_a = (m_axis_normal % 3) + 1;
        m_basis_b = (m_axis_normal % 3) + 2;
    }

    void GridPartition::Update(Real dt)
    {
        //check validate.
        m_invalid_nodes.clear();
        for (auto& cell : m_grid_cell_list)
        {
            for (auto& aabb : cell.aabb_list)
            {
                if (cell.cell_boundary.Contains(aabb->Center()) == false)
                {
                    //invalid aabb
                    m_invalid_nodes.push_back(aabb);
                }
            }
        }
        for (auto aabb : m_invalid_nodes)
        {
            Remove(aabb);
            Add(aabb);
        }
    }

    void GridPartition::Shutdown()
    {
        for (auto& cell : m_grid_cell_list)
        {
            for (auto& aabb : cell.aabb_list)
            {
                if (aabb != nullptr)
                {
                    delete aabb;
                    aabb = nullptr;
                }
            }
            cell.aabb_list.clear();
        }
        m_grid_cell_list.clear();
    }

    void GridPartition::Copy(BroadPhase* other)
    {
        for (auto& cell : m_grid_cell_list)
        {
            for (auto& aabb : cell.aabb_list)
            {
                other->Add(aabb);
            }
        }
    }

    void GridPartition::Add(BoundingAABB* aabb)
    {
        size_t    a_index, b_index;
        GridCell* cell = QueryCell(aabb->Center(), a_index, b_index);
        GridData  data(aabb, cell, a_index, b_index);
        cell->aabb_list.push_back(aabb);
    }

    void GridPartition::Remove(BoundingAABB* aabb)
    {
        if (aabb->m_userdata != nullptr)
        {
            GridData*                   grid_data = static_cast<GridData*>(aabb->m_userdata);
            std::vector<BoundingAABB*>& aabb_list = grid_data->owner_cell->aabb_list;
            aabb_list[grid_data->cell_index]      = aabb_list.back();
            aabb_list.pop_back();
            if (grid_data->cell_index < aabb_list.size())
            {
                static_cast<GridData*>(aabb_list[grid_data->cell_index]->m_userdata)->cell_index = grid_data->cell_index;
            }
            aabb->m_userdata = nullptr;
        }
    }

    void GridPartition::Clear()
    {
        for (auto& cell : m_grid_cell_list)
        {
            cell.aabb_list.clear();
        }
        m_grid_cell_list.clear();
    }

    void GridPartition::Release()
    {
        for (auto& cell : m_grid_cell_list)
        {
            for (auto& aabb : cell.aabb_list)
            {
                if (aabb != nullptr)
                {
                    delete aabb;
                    aabb = nullptr;
                }
            }
        }
    }

    void GridPartition::Render(PrimitiveRenderer* primitive_renderer, const ColorFlag& broad_phase_color, const ColorFlag& primitive_color)
    {
    }

    void GridPartition::ComputePairs(std::list<ColliderPair>& result)
    {
        size_t cell_size = m_grid_cell_list.size();
        for (size_t i = 0; i < cell_size; ++i)
        {
            GridCell& cell = m_grid_cell_list[i];

            // Loop through all balls in a cell
            size_t aabb_size = cell.aabb_list.size();
            for (size_t j = 0; j < aabb_size; ++j)
            {
                BoundingAABB* aabb = cell.aabb_list[j];
                // Update with the residing cell
                IntersectAABB(aabb, &cell, j + 1, result);
                GridData* grid_data = static_cast<GridData*>(aabb->m_userdata);
                size_t    a_index   = grid_data->basis_a_index;
                size_t    b_index   = grid_data->basis_b_index;

                // Update collision with neighbor cells
                if (a_index > 0)
                {
                    IntersectAABB(aabb, QueryCell(a_index - 1, b_index), 0, result);
                    if (b_index < m_b_count - 1)
                    {
                        IntersectAABB(aabb, QueryCell(a_index - 1, b_index + 1), 0, result);
                    }
                    if (b_index > 0)
                    {
                        IntersectAABB(aabb, QueryCell(a_index - 1, b_index - 1), 0, result);
                    }
                }
                if (b_index < m_b_count - 1)
                {
                    IntersectAABB(aabb, QueryCell(a_index, b_index + 1), 0, result);
                }
            }
        }
    }

    ColliderPrimitive* GridPartition::Pick(const Vector3& point) const
    {
        size_t a_index, b_index;
        auto   cell = QueryCell(point, a_index, b_index);
        for (auto& aabb : cell->aabb_list)
        {
            if (aabb->Contains(point) == true)
            {
                return aabb->GetCollider();
            }
        }
        return nullptr;
    }

    void GridPartition::Query(BoundingAABB* aabb, std::vector<ColliderPrimitive*>& output) const
    {
        size_t min_a, min_b, max_a, max_b;
        QueryCell(aabb->m_min, min_a, min_b);
        QueryCell(aabb->m_max, max_a, max_b);
        for (size_t i = min_a; i <= max_a; ++i)
        {
            for (size_t j = min_b; j <= max_b; ++j)
            {
                auto cell = QueryCell(i, j);
                for (auto& collider_aabb : cell->aabb_list)
                {
                    if (collider_aabb->Intersect(aabb) == true)
                    {
                        output.push_back(collider_aabb->GetCollider());
                    }
                }
            }
        }
    }

    void GridPartition::CastRay(RayCastResult& result, Real max_distance) const
    {
        Ray     ray          = result.ray;
        Vector3 ab_direction = ray.direction;
        auto    dir_a        = DirectionHelper(ray.position[m_basis_a] + m_center_of_grid[m_basis_a], ray.direction[m_basis_a]);
        auto    dir_b        = DirectionHelper(ray.position[m_basis_b] + m_center_of_grid[m_basis_b], ray.direction[m_basis_b]);
        Real    t            = 0.0f;

        //remove perpendicular
        ab_direction[m_axis_normal] = 0.0f;
        if (ab_direction.LengthSquared() > 0.0f)
        {
            while (dir_a.cell > 0 && dir_a.cell <= m_width && dir_b.cell > 0 && dir_b.cell <= m_height)
            {
                CastRayCell(QueryCell(dir_a.cell, dir_b.cell), result, max_distance);
                if (result.hit_data.hit == true)
                {
                    break;
                }
                if (dir_a.delta_t < dir_b.delta_t)
                {
                    dir_a.cell += dir_a.delta_cell;
                    auto dt = dir_a.delta_t;
                    t += dt;
                    dir_a.delta_t = dir_a.delta_t + dir_a.delta_dir_t - dt;
                    dir_b.delta_t -= dt;
                }
                else
                {
                    dir_b.cell += dir_b.delta_cell;
                    auto dt = dir_b.delta_t;
                    t += dt;
                    dir_a.delta_t -= dt;
                    dir_b.delta_t = dir_b.delta_t + dir_b.delta_dir_t - dt;
                }
            }
        }
        else
        {
            //ray direction is perpendicular to grid plane
            CastRayCell(QueryCell(dir_a.cell, dir_b.cell), result, max_distance);
        }
    }

    void GridPartition::IntersectRay(RayIntersectionResult& result, Real max_distance) const
    {
    }

    void GridPartition::TraceRay(RayTraceResult& result, Real max_distance, size_t reflect_count) const
    {
    }

    void GridPartition::IntersectAABB(BoundingAABB* aabb, GridCell* cell, size_t index, std::list<ColliderPair>& result)
    {
        size_t size = cell->aabb_list.size();
        for (size_t i = index; i < size; i++)
        {
            ColliderPrimitive* collider_a = aabb->GetCollider();
            ColliderPrimitive* collider_b = cell->aabb_list[i]->GetCollider();
            if (aabb->Intersect(cell->aabb_list[i]))
            {
                result.emplace_back(collider_a, collider_b);
            }
        }
    }

    GridCell* GridPartition::QueryCell(const Vector3& point, size_t& a_index, size_t& b_index)
    {
        //translate point position
        Real   a_pos  = (point[m_basis_a] + m_center_of_grid[m_basis_a]) / (Real)m_size_of_cell;
        Real   b_pos  = (point[m_basis_b] + m_center_of_grid[m_basis_b]) / (Real)m_size_of_cell;
        size_t cell_a = static_cast<size_t>(a_pos);
        size_t cell_b = static_cast<size_t>(b_pos);
        if (a_pos < 0.0f)
        {
            cell_a = 0;
        }
        else if (cell_a >= m_a_count)
        {
            cell_a = m_a_count - 1;
        }
        if (b_pos < 0.0f)
        {
            cell_b = 0;
        }
        else if (cell_b >= m_b_count)
        {
            cell_b = m_b_count - 1;
        }
        a_index = cell_a;
        b_index = m_a_count * cell_b;
        return &m_grid_cell_list[a_index + b_index];
    }

    GridCell* GridPartition::QueryCell(size_t a, size_t b)
    {
        if (a >= m_a_count)
        {
            a = m_a_count - 1;
        }
        if (b >= m_b_count)
        {
            b = m_b_count - 1;
        }
        size_t a_index = a;
        size_t b_index = m_a_count * b;
        return &m_grid_cell_list[a_index + b_index];
    }

    const GridCell* GridPartition::QueryCell(const Vector3& point, size_t& a_index, size_t& b_index) const
    {
        Real   a_pos  = (point[m_basis_a] + m_center_of_grid[m_basis_a]) / (Real)m_size_of_cell;
        Real   b_pos  = (point[m_basis_b] + m_center_of_grid[m_basis_b]) / (Real)m_size_of_cell;
        size_t cell_a = static_cast<size_t>(a_pos);
        size_t cell_b = static_cast<size_t>(b_pos);
        if (a_pos < 0.0f)
        {
            cell_a = 0;
        }
        else if (cell_a >= m_a_count)
        {
            cell_a = m_a_count - 1;
        }
        if (b_pos < 0.0f)
        {
            cell_b = 0;
        }
        else if (cell_b >= m_b_count)
        {
            cell_b = m_b_count - 1;
        }
        a_index = cell_a;
        b_index = m_a_count * cell_b;
        return &m_grid_cell_list[a_index + b_index];
    }

    const GridCell* GridPartition::QueryCell(size_t a, size_t b) const
    {
        if (a >= m_a_count)
        {
            a = m_a_count - 1;
        }
        if (b >= m_b_count)
        {
            b = m_b_count - 1;
        }
        size_t a_index = a;
        size_t b_index = m_a_count * b;
        return &m_grid_cell_list[a_index + b_index];
    }

    GridPartition::GridDirection GridPartition::DirectionHelper(Real pos, Real dir) const
    {
        GridDirection result;
        result.cell = static_cast<int>(std::floorf(pos / (Real)m_size_of_cell)) + 1;
        if (Utility::IsGreatorThanZero(dir) == true)
        {
            result.delta_cell = 1;
            result.delta_t    = (result.cell * m_size_of_cell - pos) / dir;
        }
        else
        {
            result.delta_cell = -1;
            result.delta_t    = ((result.cell - 1) * m_size_of_cell - pos) / dir;
        }
        result.delta_dir_t = result.delta_cell * m_size_of_cell / dir;
        return result;
    }

    void GridPartition::CastRayCell(const GridCell* cell, RayCastResult& result, Real max_distance) const
    {
        Ray ray = result.ray;
        // test AABBs for candidates
        std::vector<ColliderPrimitive*> candidate_list;
        candidate_list.reserve(cell->aabb_list.size());
        Real t = Math::REAL_POSITIVE_MAX;
        for (auto& aabb : cell->aabb_list)
        {
            if (aabb->TestRayIntersection(ray, t, max_distance) == true)
            {
                candidate_list.push_back(aabb->GetCollider());
            }
        }

        // test actual colliders
        std::vector<HitData> result_list;
        result_list.reserve(candidate_list.size());
        for (ColliderPrimitive* collider : candidate_list)
        {
            RayCastResult ray_cast_result(ray);
            collider->CastRay(ray_cast_result, max_distance);
            if (ray_cast_result.hit_data.hit == true)
            {
                result_list.push_back(ray_cast_result.hit_data);
            }
        }

        // sort the result list
        std::sort(result_list.begin(), result_list.end());
        if (result_list.empty() == false)
        {
            // the first result entry is the closest one
            HitData& entry  = result_list.front();
            result.hit_data = entry;
        }
        else
        {
            result.hit_data.hit = false;
        }
    }

    void GridPartition::IntersectRayCell(const GridCell* cell, RayIntersectionResult& result, Real max_distance) const
    {
        Ray ray = result.ray;
        // test AABBs for candidates
        std::vector<ColliderPrimitive*> candidate_list;
        candidate_list.reserve(cell->aabb_list.size());
        Real t = Math::REAL_POSITIVE_MAX;
        for (auto& aabb : cell->aabb_list)
        {
            if (aabb->TestRayIntersection(ray, t, max_distance) == true)
            {
                candidate_list.push_back(aabb->GetCollider());
            }
        }

        // test actual colliders
        result.hit_list.reserve(candidate_list.size());
        for (ColliderPrimitive* collider : candidate_list)
        {
            RayCastResult ray_cast_result(ray);
            collider->CastRay(ray_cast_result, max_distance);
            if (ray_cast_result.hit_data.hit == true)
            {
                result.hit_list.push_back(ray_cast_result.hit_data);
            }
        }

        // sort the result list
        std::sort(result.hit_list.begin(), result.hit_list.end());
    }
}
