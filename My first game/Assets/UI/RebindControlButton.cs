using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;
using static ControlManager;


public class RebindControlButton : MonoBehaviour
{
    [SerializeField] private ControlManager.PlayerActions action;
    [SerializeField] int playerNumber;
    Dictionary<PlayerActions, KeyCode> keyBindings;

    Event keyEvent;
    bool waitingForKey;


    // Start is called before the first frame update
    void Start()
    {
        waitingForKey = false;
        transform.GetComponent<Button>().onClick.AddListener(HandleClick);
        keyBindings = ControlManager.instance.PlayersKeybindings[playerNumber - 1];
        transform.GetComponentInChildren<TMPro.TextMeshProUGUI>().text = keyBindings[action].ToString();
    }

    // Update is called once per frame
    void Update()
    {
        keyBindings = ControlManager.instance.PlayersKeybindings[playerNumber - 1];
        if (waitingForKey)
        {
            transform.GetComponentInChildren<TMPro.TextMeshProUGUI>().fontSize = 15;
            transform.GetComponentInChildren<TMPro.TextMeshProUGUI>().text = "WAITING FOR KEY";
        }
        else
        {
            transform.GetComponentInChildren<TMPro.TextMeshProUGUI>().fontSize = 25;
            transform.GetComponentInChildren<TMPro.TextMeshProUGUI>().text = keyBindings[action].ToString();
        }
        
    }

    void OnGUI()
    {
        keyEvent = Event.current;

        if (keyEvent.isKey && waitingForKey)
        {
            ControlManager.instance.PlayersKeybindings[playerNumber - 1][action] = keyEvent.keyCode;
            waitingForKey = false;
        }
    }

    void HandleClick() {
        if (!waitingForKey)
            StartCoroutine(AssignKey(PlayerActions.Left));
    }

    IEnumerator WaitForKey()
    {
        while (!keyEvent.isKey)
            yield return null;
    }

    public IEnumerator AssignKey(PlayerActions key)
    {
        waitingForKey = true;

        yield return WaitForKey(); //Executes endlessly until user presses a key

        yield return null;
    }


}
