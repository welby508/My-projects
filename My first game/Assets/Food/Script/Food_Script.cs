using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Food_Script : MonoBehaviour
{
    public static event Action<string> SendDeadFoodName;

    private void OnTriggerEnter(Collider other)
    {
        if(!other.isTrigger)
        {
            other.transform.localScale += new Vector3 (0.1f, 0.1f, 0.1f);
            Destroy(gameObject); 
        }
    }

    private void OnDestroy()
    {
        string FoodName = gameObject.name;
        SendDeadFoodName?.Invoke(FoodName);
    }
}
