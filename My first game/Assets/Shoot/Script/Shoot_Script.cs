using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shoot_Script : MonoBehaviour
{
    [SerializeField]
    GameObject bullet;

    public float FireRate = 6.0f;
    private float nextFire;
    private float ProjectileSpeed = 350.0f;

    bool bShootingAllowed;

    public AudioSource FireSFX;

    // Start is called before the first frame update
    void Start()
    {
        nextFire = Time.time;
    }

    // Update is called once per frame
    void Update()
    {
        FireSFX.volume = AudioManager.instance.SFXVolume; 

        if(bShootingAllowed)
        {
            Shoot();
        }
        bShootingAllowed = false;
    }

    private void Shoot()
    {
        if (bIsShootingReady())
        {
            Vector3 startPos = transform.position;
            startPos.y = transform.localScale.magnitude / 2;
            Vector3 forwardDir = transform.forward;
            forwardDir.y = 0.5f;
            GameObject Projectile = Instantiate(bullet, transform.position + (forwardDir * transform.localScale.magnitude / 2), Quaternion.identity);

            Projectile.transform.rotation = transform.rotation;

            Projectile.GetComponent<Rigidbody>().AddRelativeForce(transform.forward * ProjectileSpeed);

            nextFire = Time.time + FireRate;

            FireSFX.Play();
        }
    }

    public void AllowShooting()
    {
        bShootingAllowed = true;
    }

    public bool bIsShootingReady()
    {
        return Time.time > nextFire;
    }
}
