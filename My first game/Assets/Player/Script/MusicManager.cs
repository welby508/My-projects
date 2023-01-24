using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MusicManager : MonoBehaviour
{

    public AudioSource Music;

    public AudioClip[] MusicArray;

    // Start is called before the first frame update
    void Start()
    {
        Music.clip = MusicArray[Random.Range(0, MusicArray.Length)];
        Music.Play();
    }

    // Update is called once per frame
    void Update()
    {
        Music.volume = AudioManager.instance.MusicVolume * 0.03f;
    }
}
