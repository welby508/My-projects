using UnityEngine;
using System.Collections;
using static ControlManager;
using System.Collections.Generic;
using System.Drawing;
using System;
using UnityEditor;
using UnityEngine.AI;
using System.Linq;

public class Controller : MonoBehaviour
{

    [SerializeField] int playerNumber;
    public Animator animator;
    public float speed = 3.5f;

    private Dictionary<PlayerActions, KeyCode> keyBindings = new();

    private float PlayerSpeed;

    private float DebuffDuration = 2.0f;
    private float DebuffTimer = 0;
    private bool bIsNotSlowed = true;
    private int currentSpeed = 0;
    private bool bCanMove = true;

    public static event Action<string> SendDeadPlayerName;

    private Shoot_Script shoot;

    public AudioSource OofSFX;

    private void Awake()
    {
        shoot = GetComponent<Shoot_Script>();
        PlayerSpeed = speed;
        keyBindings = ControlManager.instance.PlayersKeybindings[playerNumber];
    }


    // Update is called once per frame
    void Update()
    {
        OofSFX.volume = AudioManager.instance.SFXVolume;

        if (!bCanMove) return; //Player is in hitstun

        ManageDebuffStatus();
        if (Input.GetKey(keyBindings[ControlManager.PlayerActions.Shoot]))
        {
            shoot.AllowShooting();
            if (shoot.bIsShootingReady())
            {
                animator.SetTrigger("Attack");
            }
        }
        if (Input.GetKey(keyBindings[ControlManager.PlayerActions.Forward]))
        {
            transform.position += transform.forward * Time.deltaTime * speed;
            currentSpeed = 1;
        }
        if (Input.GetKey(keyBindings[ControlManager.PlayerActions.Backward]))
        {
            transform.position -= transform.forward * Time.deltaTime * speed;
            currentSpeed = -1;
        }
        if (Input.GetKey(keyBindings[ControlManager.PlayerActions.Left]))
        {
            transform.RotateAround(transform.position, Vector3.up, -75 * Time.deltaTime);
        }
        if (Input.GetKey(keyBindings[ControlManager.PlayerActions.Right]))
        {
            transform.RotateAround(transform.position, Vector3.up, 75 * Time.deltaTime);
        }

        if (!(Input.GetKey(keyBindings[ControlManager.PlayerActions.Forward]) ||
                Input.GetKey(keyBindings[ControlManager.PlayerActions.Left]) ||
                Input.GetKey(keyBindings[ControlManager.PlayerActions.Right]) ||
                Input.GetKey(keyBindings[ControlManager.PlayerActions.Backward])))
        { 

            currentSpeed = 0;
           
        }

        animator.SetFloat("Speed", currentSpeed);
    }

    private void ManageDebuffStatus()
    {
        if (bIsNotSlowed && CheckDebuffStatus())
        {
            DebuffTimer = Time.time;
            bIsNotSlowed = false;
        }

        if (!bIsNotSlowed && Time.time > DebuffTimer + DebuffDuration)
        {
            bIsNotSlowed = true;
            GetComponent<Controller>().speed = PlayerSpeed;
        }
    }

    private bool CheckDebuffStatus()
    {
        return speed < PlayerSpeed;
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "AI_Pawn" || collision.gameObject.tag == "Player")
        {
            if (collision.transform.localScale.magnitude < transform.localScale.magnitude)
            {
                transform.localScale += new Vector3(0.1f, 0.1f, 0.1f);
                Destroy(collision.gameObject);
            }
        }


    }

    public void HandleAnimationEvent(string message)
    {
        if (message.Equals("AnimationHitStarted"))
        {
            bCanMove = false;
        }
        if (message.Equals("AnimationHitFinished"))
        {
            bCanMove = true;
        }
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            animator.SetTrigger("Hit");
            OofSFX.Play();
        }
    }

    private void OnDestroy()
    {
        string PlayerName = gameObject.name;
        SendDeadPlayerName?.Invoke(PlayerName);
    }



}
