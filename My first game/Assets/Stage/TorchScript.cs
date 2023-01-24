using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TorchScript : MonoBehaviour
{
    public AudioSource TorchVol;
    ParticleSystem particle;
    TorchParticlePool particleSystemPool;

    private void Start()
    {
        particleSystemPool = FindObjectOfType<TorchParticlePool>();
        particle = particleSystemPool.GetParticleSystem();
        particle.transform.position = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        TorchVol.volume = AudioManager.instance.AmbientVolume * 0.50f;
    }
}
