using System;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.UIElements;
using GameState = GameManager.GameState;

public class MenuManager : MonoBehaviour
{
    [SerializeField] public GameObject UICanvasPrefab;
    public static MenuManager instance;

    private void Awake()
    {
        if (instance is null)
        {
            DontDestroyOnLoad(gameObject);
            instance = this;
            GameManager.GameStateChangedEvent += HandleGameStateChange;

        }
        else if (instance != this)
        {
            GameManager.GameStateChangedEvent -= HandleGameStateChange;
            Destroy(gameObject);
        }
    }


    private void HandleGameStateChange(GameState state)
    {
        switch(state)
        {
            case GameState.SplashScreen:
                ShowMenu("SpashScreen");
                break;
            case GameState.MainMenu:
                ShowMenu("MainMenu");
                break;
            case GameState.OptionMenu:
                ShowMenu("OptionMenu");
                break;
            case GameState.MatchOptionMenu:
                ShowMenu("MatchOptionMenu");
                break;
            case GameState.Pause:
                ShowMenu("PauseMenu");
                break;
            case GameState.OnGoingMatch:
                CloseAllMenus();
                break;
            case GameState.MatchFinished:
                break;
            case GameState.Quit:
                Application.Quit();

                #if UNITY_EDITOR
                UnityEditor.EditorApplication.isPlaying = false;
                #endif
                break;
            default:
                break;
        }
    }

    private void ShowMenu(string MenuName)
    {
        UICanvasPrefab.gameObject.SetActive(true);
        bool MenuFound = false;
        foreach (Transform child in UICanvasPrefab.transform)
        {
            if (child.gameObject.name == MenuName)
            {

                child.gameObject.SetActive(true);
                MenuFound = true;
            }
            else
            {
                child.gameObject.SetActive(false);
            }
        }
        if (!MenuFound)
        {
            throw new ArgumentOutOfRangeException(nameof(MenuName), MenuName, null);
        }
    }

    private void CloseAllMenus()
    {
        foreach (Transform child in UICanvasPrefab.transform)
        {
            child.gameObject.SetActive(false);

        }
        UICanvasPrefab.gameObject.SetActive(false);
    }
}
