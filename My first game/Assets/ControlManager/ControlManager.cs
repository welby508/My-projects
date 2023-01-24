using System;
using System.Collections.Generic;
using UnityEngine;


public class ControlManager : MonoBehaviour
{
    public static ControlManager instance;

    public enum PlayerActions { 
        Shoot,
        Forward,
        Backward,
        Left,
        Right
    }

    public int NumberOfPlayers = 2;

    private readonly Dictionary<PlayerActions, KeyCode> P1DefaultKeybinds = new()
    {
        { PlayerActions.Shoot, KeyCode.Space},
        { PlayerActions.Forward, KeyCode.W},
        { PlayerActions.Backward, KeyCode.S },
        { PlayerActions.Left, KeyCode.A },
        { PlayerActions.Right, KeyCode.D },
    };

    private readonly Dictionary<PlayerActions, KeyCode> P2DefaultKeyBinds = new()
    {
        { PlayerActions.Shoot, KeyCode.RightControl},
        { PlayerActions.Forward, KeyCode.UpArrow},
        { PlayerActions.Backward, KeyCode.DownArrow },
        { PlayerActions.Left, KeyCode.LeftArrow },
        { PlayerActions.Right, KeyCode.RightArrow },
    };


    public List<Dictionary<PlayerActions, KeyCode>> PlayersKeybindings = new();
    public List<Dictionary<PlayerActions, KeyCode>> PlayersDefaultKeybindings = new();

    private void setPlayerDefaultKeybinding()
    {
        PlayersDefaultKeybindings.Add(P1DefaultKeybinds);
        PlayersDefaultKeybindings.Add(P2DefaultKeyBinds);
    }
    private void GetPlayerPrefKeybinding()
    {
        PlayersKeybindings.Clear();
        
        for (int i = 0; i < NumberOfPlayers; i++)
        {

            Dictionary<PlayerActions, KeyCode> playerPrefKeybinding = new();

            foreach (PlayerActions action in Enum.GetValues(typeof(PlayerActions)))
            {
                playerPrefKeybinding[action] = (KeyCode)System.Enum.Parse(typeof(KeyCode), PlayerPrefs.GetString(i.ToString() + action.ToString(),
                    PlayersDefaultKeybindings[i][action].ToString()));

            }
            PlayersKeybindings.Add(playerPrefKeybinding);
        }
    }

    public void SetPlayerPrefKeybinding()
    {

        for (int i = 0; i < NumberOfPlayers; i++)
        {

            foreach (PlayerActions action in Enum.GetValues(typeof(PlayerActions)))
            {
                PlayerPrefs.SetString(i.ToString() + action.ToString(), PlayersKeybindings[i][action].ToString());

            }
        }
    }

    private void Awake()
    {
        if (instance is null)
        {
            DontDestroyOnLoad(gameObject);
            instance = this;
            setPlayerDefaultKeybinding();
            GetPlayerPrefKeybinding();
        }
        else if (instance != this)
        {
            Destroy(gameObject);
        }
    }


    public void ResetKeybindingsToDefault(int playerNumber)
    {
            PlayersKeybindings.RemoveAt(playerNumber);
            Dictionary<PlayerActions, KeyCode> playerPrefKeybinding = new();

            foreach (PlayerActions action in Enum.GetValues(typeof(PlayerActions)))
            {
                playerPrefKeybinding[action] = PlayersDefaultKeybindings[playerNumber][action];
                PlayerPrefs.SetString(playerNumber.ToString() + PlayerActions.Shoot.ToString(),
                    PlayersDefaultKeybindings[playerNumber][action].ToString());
        }
        PlayersKeybindings.Insert(playerNumber, playerPrefKeybinding);
    }


}
