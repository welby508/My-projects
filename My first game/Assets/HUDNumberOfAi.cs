using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HUDNumberOfAi : MonoBehaviour
{
    int NumberOfAi = 0;
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        
        if (MatchController.instance != null)
        {
            
            transform.GetComponentInChildren<TMPro.TextMeshProUGUI>().text = "Enemy remaining :" + NumberOfAi.ToString();
            NumberOfAi = MatchController.instance.GetRemainingAINumber();
        }



    }
}
