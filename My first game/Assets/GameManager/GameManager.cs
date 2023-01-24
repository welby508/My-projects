using System;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public static GameManager instance;
    public GameState currentGameState;
    public static event Action<GameState> GameStateChangedEvent;
    public static bool GameIsPaused = false;
    public static bool HardMode = false;
    public static int NumberOfAi = 3;
    public static bool MultiplayerMode = false;
    public static bool PlayersWon = false;
    public static string WinnerName = "";

    //avatar customization
    private GameObject avatar;
    private Color primaryColor;
    private Color secondaryColor;
    private Mesh hatMesh;

    public void SetAvatar(int valueIndex) {
        switch (valueIndex)
        {
            case 0:
                avatar = Resources.Load("KingAvatar") as GameObject;
                break;
            case 1:
                avatar = Resources.Load("SproutAvatar") as GameObject;
                break;
            default:
                avatar = Resources.Load("KingAvatar") as GameObject;
                break;
        }
    }
    public GameObject GetAvatar()
    {
        if (avatar)
        {
            return avatar;
        }
        else
        {
            return Resources.Load("KingAvatar") as GameObject;
        }
        
    } 

    public void SetPrimaryColor(int valueIndex) {
        switch (valueIndex)
        {
            case 0:
                primaryColor = new Color();
                break;
            case 1:
                primaryColor = Color.red;
                break;
            case 2:
                primaryColor = Color.blue;
                break;
            case 3:
                primaryColor = Color.green;
                break;
        }
    }

    public Color GetPrimaryColor()
    {
        return primaryColor;
    }

    public void SetSecondaryColor(int valueIndex) {
        switch (valueIndex)
        {
            case 0:
                secondaryColor = new Color();
                break;
            case 1:
                secondaryColor = Color.red;
                break;
            case 2:
                secondaryColor = Color.blue;
                break;
            case 3:
                secondaryColor = Color.green;
                break;
        }
    }
    public Color GetSecondaryColor() {
        return secondaryColor;
     }

    public void SetHatMesh(int valueIndex) {

        switch (valueIndex)
        {
            case 0:
                hatMesh = null;
                break;
            case 1:
                hatMesh = Resources.Load("Crown") as Mesh;
                break;
            case 2:
                hatMesh = Resources.Load("Leaf") as Mesh;
                break;
            case 3:
                hatMesh = Resources.Load("Sprout") as Mesh;
                break;
            case 4:
                hatMesh = Resources.Load("VikingHelmet") as Mesh;
                break;
            case 5:
                hatMesh = Resources.Load("MetalHelmet") as Mesh;
                break;
        }
    }
    public Mesh GetHatMesh() {
        return hatMesh;
    }

    public enum GameState
    {
        SplashScreen,
        MainMenu,
        OptionMenu,
        Pause,
        OnGoingMatch,
        MatchOptionMenu,
        MatchFinished,
        Defeat,
        Quit,
    }
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
    }

    private void Start()
    {
        UpdateGameState(GameState.SplashScreen);
    }

    private void Update()
    {
        switch (currentGameState)
        {
            case GameState.SplashScreen:
                if (Input.anyKey) UpdateGameState(GameState.MainMenu);
                break;
            case GameState.MainMenu:
                break;
            case GameState.OptionMenu:
                break;
            case GameState.MatchOptionMenu:
                break;
            case GameState.Pause:
                if (Input.GetKeyDown(KeyCode.Escape)) UpdateGameState(GameState.OnGoingMatch);
                break;
            case GameState.OnGoingMatch:
                if (Input.GetKeyDown(KeyCode.Escape)) UpdateGameState(GameState.Pause);
                
                break;
            case GameState.MatchFinished:
                break;
            case GameState.Quit:
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(GameState), currentGameState, null);
        }

    }
    public void HandleStart()
    {
        PlayersWon = false;
        WinnerName = "";
        Loader.Load(Loader.Scene.GameScene);
    }
    public void HandleOption()
    {
        GameManager.instance.UpdateGameState(GameState.OnGoingMatch);
    }

    public void HandleGoBackToMainMenu()
    {
        Loader.Load(Loader.Scene.MainMenu);
    }

    public void ChangeMode()
    {
        HardMode = !HardMode;
    }

    public void ChangeNumberOfAi(int number)
    {
        NumberOfAi = Math.Clamp(number, 0, 4);
    }

    public void UpdateGameState(GameState newState)
    {
        GameState prevState = currentGameState;
        currentGameState = newState;
        switch (newState)
        {
            case GameState.SplashScreen:
                break;
            case GameState.MainMenu:
                if (prevState == GameState.Pause || prevState == GameState.OnGoingMatch) HandleGoBackToMainMenu();
                if (prevState == GameState.Pause) Unpause();
                break;
            case GameState.OptionMenu:
                break;
            case GameState.MatchOptionMenu:
                break;
            case GameState.Pause:
                Time.timeScale = 0f;
                GameIsPaused = true;
                break;
            case GameState.OnGoingMatch:
                if (prevState == GameState.Pause) Unpause();
                else HandleStart();
                break;
            case GameState.MatchFinished:
                break;
            case GameState.Quit:
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(GameState), currentGameState, null);
        }

        GameStateChangedEvent?.Invoke(currentGameState);
    }

    private static void Unpause()
    {
        Time.timeScale = 1f;
        GameIsPaused = false;
    }
}