#include "ParticleEmitter.hpp"
#include "../DataType/Particle.hpp"

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

    void ParticleEmitter::Update(Real dt) const
    {
        for (size_t i = 0; i < m_max_amount; ++i)
        {
            if (m_particles[i].IsActive())
            {
                m_particles[i].position += m_particles[i].velocity * dt;
                m_particles[i].life -= m_life_decay_rate * dt;
                m_particles[i].scale -= m_scale_decay_rate * dt;
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
}
