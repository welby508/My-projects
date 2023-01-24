using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TorchParticlePool : MonoBehaviour
{
    public ParticleSystem prefab;
    public int poolSize = 16;

    private Queue<ParticleSystem> particles; 

    void Awake()
    {
        particles = new Queue<ParticleSystem>(poolSize);
        for (int i = 0; i < poolSize; i++)
        {
            ParticleSystem particle = Instantiate(prefab);
            particle.gameObject.SetActive(false);
            particles.Enqueue(particle);
        }
    }

    public ParticleSystem GetParticleSystem()
    {
        if (particles.Count == 0)
        {
            ParticleSystem particle = Instantiate(prefab);
            particles.Enqueue(particle);
        }

        ParticleSystem particleSystem = particles.Dequeue();
        particleSystem.gameObject.SetActive(true);
        return particleSystem;
    }

    public void ReturnParticleSystem(ParticleSystem particleSystem)
    {
        particleSystem.gameObject.SetActive(false);
        particles.Enqueue(particleSystem);
    }
}