using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Bullet_Script : MonoBehaviour
{
    public float debuffModifier = 2.0f;

    ParticleSystem particle;
    BulletParticlePool particleSystemPool;

    public bool bTargetIsHit;

    private void Start()
    {
        particleSystemPool = FindObjectOfType<BulletParticlePool>();
        particle = particleSystemPool.GetParticleSystem();

        particle.transform.rotation = Quaternion.LookRotation(-transform.forward);
    }

    private void Update()
    {
        particle.transform.position = transform.position;
    }

    private void OnTriggerEnter(Collider other)
    {
        bTargetIsHit = false;

        if (!other.isTrigger)
        {
            if (other.CompareTag("AI_Pawn"))
            {
                other.GetComponent<NavMeshAgent>().speed = debuffModifier;
                bTargetIsHit = true;
            }

            if (other.CompareTag("Player"))
            {
                other.GetComponent<Controller>().speed = debuffModifier;
                bTargetIsHit = true;
            }

            particleSystemPool.ReturnParticleSystem(particle);

            Destroy(gameObject);
        }
    }
}
