using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Unity.VisualScripting.Member;
using Button = UnityEngine.UI.Button;
using GameState = GameManager.GameState;

public class MenuButtonScript : MonoBehaviour
{
    [SerializeField] private GameState NewState;
    // Start is called before the first frame update
    private AudioSource audioSource;

    private void Awake()
    {
        this.gameObject.AddComponent<AudioSource>();
        audioSource = this.gameObject.GetComponent<AudioSource>();
        audioSource.loop = false;
        audioSource.playOnAwake = false;
        audioSource.clip = Resources.Load<AudioClip>("clickSound"); //imported from package minimalist UI sounds

    }
    void Start()
    {
        transform.GetComponent<Button>().onClick.AddListener(HandleClick);
    }

    void HandleClick()
    {
        StartCoroutine(PlaySound());
        StartCoroutine(ChangeGameState());
    }

    IEnumerator PlaySound()
    {
        audioSource.Play();
        yield return new WaitWhile(() => audioSource.isPlaying);
        GameManager.instance.UpdateGameState(NewState);
    }

    IEnumerator ChangeGameState()
    {
        while (audioSource.isPlaying)
            yield return null;
        GameManager.instance.UpdateGameState(NewState);
    }

}
