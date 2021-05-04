#pragma once
#include "../../../Math/Algebra/Vector3.hpp"
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
    class ParticleEmitterComponent;

    class ParticleEmitter
    {
    public:
        ParticleEmitter();
        ~ParticleEmitter();

        void Initialize();
        void Update(Real dt);
        void Render() const;
        void Shutdown();

        void AddParticle(const Particle& particle);
        void AddParticle(const Vector3& pos, const Vector3& vel, const Color& color, Real life = 1.0f, Real scale = 1.0f);

        //setter
        void SetRenderer(RendererCommon* renderer);
        void SetBuffer();
        void SetTexture(TextureCommon* texture);
        void SetTransform(Transform* transform);
        void SetBillboardPosition(const Vector3& position);
        void SetParticleAmount(const size_t& amount);
        void SetEmissionAmount(const size_t& amount);
        void SetEmissionRate(const Real& rate);
        void SetLifeDecayRate(const Real& rate);
        void SetScaleDecayRate(const Real& rate);
        void SetBaseParticle(const Particle& particle);
        void SetBasePosition(const Vector3& position);
        void SetBaseVelocity(const Vector3& velocity);
        void SetBaseColor(const Color& color);
        void SetBaseLife(const Real& life_time);
        void SetBaseScale(const Real& scale);
        void SetPositionVariance(const Vector3& variance);
        void SetVelocityVariance(const Vector3& variance);
        void SetColorVariance(const Color& variance);
        void SetScaleVariance(const Real& variance);
        void SetLifeVariance(const Real& variance);

        //getter
        size_t         GetIndexCount() const;
        size_t         GetInstanceCount() const;
        TextureCommon* GetTexture() const;
        Matrix44       GetModelMatrix() const;
    private:
        size_t   GetFreeIndex();
        void     EmitParticles();
        void     KillParticles();
        void     SortParticle();
        Matrix44 ParticleToWorldMatrix(const Particle& particle, const Matrix44& orientation) const;

    private:
        friend class ParticleEmitterComponent;

    private:
        //Particle*                       m_particles = nullptr;
        RendererCommon*                 m_renderer  = nullptr;
        TextureCommon*                  m_texture   = nullptr;
        InstanceBufferCommon*           m_buffer    = nullptr;
        Transform*                      m_transform = nullptr;
        ParticleEmitterComponent*       m_component = nullptr;
        std::vector<Particle>           m_particles;
        std::vector<InstanceDataCommon> m_instances;
        size_t                          m_index_count = 0;
        Vector3                         m_billboard_position;

        //particle data
        size_t   m_max_amount       = 100;
        size_t   m_free_particle    = 0;
        size_t   m_active_amount    = 0;
        size_t   m_emission_amount  = 1;
        Real     m_emission_rate    = 0.0f;
        Real     m_elapsed_time     = Math::REAL_POSITIVE_MAX;
        Real     m_life_decay_rate  = 0.0f;
        Real     m_scale_decay_rate = 0.0f;
        Particle m_base_particle;

        //particle variance
        Vector3 m_position_variance;
        Vector3 m_velocity_variance;
        Color   m_color_variance;
        Real    m_life_variance  = 0.0f;
        Real    m_scale_variance = 0.0f;
    };
}
