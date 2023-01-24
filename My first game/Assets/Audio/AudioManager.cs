using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AudioManager : MonoBehaviour
{
    public static AudioManager instance;

    public float SFXVolume;
    public float AmbientVolume;
    public float MusicVolume;

    private void Awake()
    {
        if (instance is null)
        {
            DontDestroyOnLoad(gameObject);
            instance = this;
        }
        else if (instance != this)
        {
            Destroy(gameObject);
        }

        SFXVolume = 1.0f;
        AmbientVolume = 1.0f;
        MusicVolume = 1.0f;
}

    public void SetSFXVolume(float volume)
    {
        SFXVolume = volume;
    }

    public void SetAmbientVolume(float volume)
    {
        AmbientVolume = volume;
    }

    public void SetMusicVolume(float volume)
    {
        MusicVolume = volume;
    }


}
