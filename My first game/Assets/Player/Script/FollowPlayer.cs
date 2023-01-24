using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class FollowPlayer : MonoBehaviour
{
    public Transform player;

    // Update is called once per frame
    void Update()
    {
        if (!player.IsDestroyed())
        {
            transform.LookAt(player.transform.position);
            transform.position = player.transform.position - player.forward * 10 + new Vector3(0, 10, 0);
        }
    }
}
