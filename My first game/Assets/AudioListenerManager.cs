using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioListenerManager : MonoBehaviour
{
    // Start is called before the first frame update
    private void Awake()
    {
        this.gameObject.AddComponent<AudioListener>();
    }

    private void OnDisable()
    {
        this.gameObject.GetComponent<AudioListener>().enabled = false;
    }

    private void OnEnable()
    {
        this.gameObject.GetComponent<AudioListener>().enabled = true;
    }
}
