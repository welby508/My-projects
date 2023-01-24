using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InputFieldScript : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        transform.GetComponent<TMPro.TMP_InputField>().onEndEdit.AddListener(HandleEndEdit);
    }

    private void HandleEndEdit(string txt)
    {
        if (!string.IsNullOrEmpty(txt))
        {
            GameManager.instance.ChangeNumberOfAi(int.Parse(txt));
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
