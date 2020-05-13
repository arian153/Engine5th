#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../DataType/Particle.hpp"
#include <vector>
#include "../Vertex/InstanceDataCommon.hpp"

namespace Engine5
{
    class RendererCommon;
    class Transform;
    class InstanceBufferCommon;
    class Color;
    class TextureCommon;

    class ParticleEmitter
    {
    public:
        ParticleEmitter();
        ~ParticleEmitter();

        void Init();
        void Update(Real dt);
        void Render() const;
        void ShutDown();

        void AddParticle(const Particle& particle);
        void AddParticle(const Vector3& pos, const Vector3& vel, const Color& color, Real life = 1.0f, Real scale = 1.0f);

        void SetLifeDecayRate(Real rate);
        void SetScaleDecayRate(Real rate);
        void SetParticleAmount(size_t amount);
        void SetTexture(TextureCommon* texture);
        void SetTransform(Transform* transform);
        void SetColor(const Color& color);
        void SetRenderer(RendererCommon* renderer);

    private:
        size_t   GetFreeIndex();
        void     EmitParticles();
        void     BuildBuffer();
        Matrix44 ParticleToWorld(const Particle& particle) const;
    private:
        Particle                        m_base_particle;
        Particle*                       m_particles = nullptr;
        RendererCommon*                 m_renderer  = nullptr;
        TextureCommon*                  m_texture   = nullptr;
        InstanceBufferCommon*           m_buffer    = nullptr;
        Transform*                      m_transform = nullptr;
        std::vector<InstanceDataCommon> m_instances;

        //other data
        Real   m_life_decay_rate  = 0.0f;
        Real   m_scale_decay_rate = 0.0f;
        Real   m_emission_rate    = 0.0f;
        Real   m_elapsed_time     = 0.0f;
        size_t m_max_amount       = 100;
        size_t m_free_index       = 0;
        size_t m_emission_amount  = 1;
        size_t m_active_amount    = 0;
        Color  m_emitter_color;
        //variance
        Vector3 m_position_variance;
        Vector3 m_direction_variance;
        Color   m_color_variance;
        Real    m_speed_variance = 0.0f;
        Real    m_scale_variance = 0.0f;
        Real    m_life_variance  = 0.0f;
    };
}
