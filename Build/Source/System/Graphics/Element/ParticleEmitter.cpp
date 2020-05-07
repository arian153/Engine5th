#include "ParticleEmitter.hpp"
#include "../DataType/Particle.hpp"
#include "../../Math/Utility/Random.hpp"

namespace Engine5
{
    ParticleEmitter::ParticleEmitter()
    {
    }

    ParticleEmitter::~ParticleEmitter()
    {
    }

    void ParticleEmitter::Init(size_t initial_amount, Real life_decay_rate, Real scale_decay_rate, TextureCommon* texture)
    {
        m_max_amount       = initial_amount;
        m_life_decay_rate  = life_decay_rate;
        m_scale_decay_rate = scale_decay_rate;
        m_texture          = texture;
        m_particles        = new Particle[ m_max_amount ];
    }

    void ParticleEmitter::Update(Real dt)
    {
        m_elapsed_time += dt;
        if (m_elapsed_time > m_emission_rate)
        {
            m_elapsed_time = 0.0f;
            EmitParticles();
        }
        for (size_t i = 0; i < m_max_amount; ++i)
        {
            if (m_particles[i].IsActive())
            {
                m_particles[i].position += m_particles[i].velocity * dt;
                m_particles[i].life -= m_life_decay_rate * dt;
                m_particles[i].scale -= m_scale_decay_rate * dt;
                m_active_amount++;
            }
        }
    }

    void ParticleEmitter::ShutDown()
    {
        delete[] m_particles;
        m_particles = nullptr;
    }

    void ParticleEmitter::AddParticle(const Particle& particle)
    {
        size_t index       = GetFreeIndex();
        m_particles[index] = particle;
    }

    void ParticleEmitter::AddParticle(const Vector3& pos, const Vector3& vel, const Color& color, Real life, Real scale)
    {
        size_t    index   = GetFreeIndex();
        Particle& created = m_particles[index];
        created.position  = pos;
        created.velocity  = vel;
        created.color     = color;
        created.life      = life;
        created.scale     = scale;
    }

    void ParticleEmitter::SetLifeDecayRate(Real rate)
    {
        if (rate >= 0.0f)
        {
            m_life_decay_rate = rate;
        }
    }

    void ParticleEmitter::SetScaleDecayRate(Real rate)
    {
        if (rate >= 0.0f)
        {
            m_scale_decay_rate = rate;
        }
    }

    void ParticleEmitter::Resize(size_t amount)
    {
        delete[] m_particles;
        m_max_amount = amount;
        m_particles  = new Particle[ m_max_amount ];
    }

    size_t ParticleEmitter::GetFreeIndex()
    {
        for (size_t i = m_free_index; i < m_max_amount; ++i)
        {
            if (m_particles[i].IsActive() == false)
            {
                m_free_index = i;
                return m_free_index;
            }
        }
        for (size_t i = 0; i < m_free_index; ++i)
        {
            if (m_particles[i].IsActive() == false)
            {
                m_free_index = i;
                return m_free_index;
            }
        }
        m_free_index++;
        if (m_free_index >= m_max_amount)
        {
            m_free_index = 0;
        }
        return m_free_index;
    }

    void ParticleEmitter::EmitParticles()
    {
        Random random;
        for (size_t i = 0; i < m_emission_amount; ++i)
        {
            Particle particle;
            //position
            particle.position.x = random.GetRangedRandomReal(-m_position_variance.x, m_position_variance.x);
            particle.position.y = random.GetRangedRandomReal(-m_position_variance.y, m_position_variance.y);
            particle.position.z = random.GetRangedRandomReal(-m_position_variance.z, m_position_variance.z);
            //velocity
            Vector3 random_dir;
            random_dir.x      = random.GetRangedRandomReal(-m_direction_variance.x, m_direction_variance.x);
            random_dir.y      = random.GetRangedRandomReal(-m_direction_variance.y, m_direction_variance.y);
            random_dir.z      = random.GetRangedRandomReal(-m_direction_variance.z, m_direction_variance.z);
            Real random_speed = random.GetRangedRandomReal(0.0f, m_speed_variance);
            particle.velocity = random_speed * random_dir.Normalize();
            //color
            particle.color.r = random.GetRangedRandomReal(0.0f, m_color_variance.r);
            particle.color.g = random.GetRangedRandomReal(0.0f, m_color_variance.g);
            particle.color.b = random.GetRangedRandomReal(0.0f, m_color_variance.b);
            particle.color.a = random.GetRangedRandomReal(0.0f, m_color_variance.a);
            //life
            particle.life = random.GetRangedRandomReal(0.0f, m_life_variance);
            //scale
            particle.scale = random.GetRangedRandomReal(0.0f, m_scale_variance);
            //apply default data
            particle.Merge(m_base_particle);
            AddParticle(particle);
        }
    }
}
