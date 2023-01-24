using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class AI_Brain : MonoBehaviour
{

    public Animator animator;

    private NavMeshAgent agent;
    private AI_Knowledge MyKnowledge;
    private Shoot_Script shoot;

    private float AIspeed;

    private float DebuffDuration = 2.0f;
    private float DebuffTimer = 0;
    private bool bIsNotSlowed = true;

    private bool bDifficultyHard = false;
    private float DistanceFleeHard = 6.0f;
    private float DistanceFoodChaseHard = 3.0f;

    public static event Action<string> SendDeadAIName;

    AI_ChaseBehavior ChaseBehavior;
    AI_WanderBehavior WanderBehavior;
    AI_FleeBehavior FleeBehavior;
    AI_FeedBehavior FeedBehavior;

    public AudioSource OofSFX;

    private SphereCollider sensor;

    public enum BehaviorList
    {
        Chase, Feed, Flee, Wander, Null
    }

    private BehaviorList CurrentBehavior = BehaviorList.Null;
    private BehaviorList LastEvaluatedBehavior;

    private void Awake()
    {
        agent = GetComponent<NavMeshAgent>();
        MyKnowledge = GetComponent<AI_Knowledge>();
        shoot = GetComponent<Shoot_Script>();

        //Store the AI speed;
        AIspeed = GetComponent<NavMeshAgent>().speed;
        bDifficultyHard = GameManager.HardMode;

        //Get the 2nd SphereCollider which is used as a Sensor
        var collider = GetComponents<SphereCollider>();
        sensor = collider[1];
    }
    void Start()
    {
        ChaseBehavior = new AI_ChaseBehavior(MyKnowledge, agent, shoot);
        WanderBehavior = new AI_WanderBehavior(MyKnowledge, agent, this);
        FleeBehavior = new AI_FleeBehavior(MyKnowledge, agent, shoot, this);
        FeedBehavior = new AI_FeedBehavior(MyKnowledge, agent);

        //Increase the Radius if in hard mode;
        if(bDifficultyHard)
        {
            sensor.radius = sensor.radius * 5;
        }
    }

    void Update()
    {
        OofSFX.volume = AudioManager.instance.SFXVolume;

        animator.SetFloat("Speed", agent.velocity.magnitude);
        ManageDebuffStatus();
        if(bDifficultyHard)
        {
            BehaviorSelectorHard();
        }

        else
        {
            BehaviorSelector();
        }

        switch (CurrentBehavior)
        {
            case BehaviorList.Chase:
                ChaseBehavior.Tick();
                break;
            case BehaviorList.Feed:
                FeedBehavior.Tick();
                break;
            case BehaviorList.Flee:
                FleeBehavior.Tick();
                break;
            case BehaviorList.Wander:
                WanderBehavior.Tick();
                break;
        }
    }

    private void BehaviorSelector()
    {
        //Always priortize Flee is a Threat is Nearby
        if (MyKnowledge.bIsTypeNearby(AI_Knowledge.TargetType.Threat))
        {
            if (MyKnowledge.bIsTypeNearby(AI_Knowledge.TargetType.Food))
            {
                //Check if Food can be accessed despite Threat being nearby
                float distFromThreat = MyKnowledge.GetDistanceBetweenActors(transform.position, MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Threat));
                float distFromFood = MyKnowledge.GetDistanceBetweenActors(transform.position, MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Food));
                float distThreatFromFood = MyKnowledge.GetDistanceBetweenActors(MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Threat),
                                                                                MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Food));
                if (distFromThreat > distFromFood && distThreatFromFood > 2.0f)
                {
                    LastEvaluatedBehavior = CurrentBehavior;
                    CurrentBehavior = BehaviorList.Feed;
                    return;
                }
            }
            LastEvaluatedBehavior = CurrentBehavior;
            CurrentBehavior = BehaviorList.Flee;
            return;
        }
        
        //Second priority Chasing Preys, in normal difficulty they will ignore nearby food even if benificial
        if (MyKnowledge.bIsTypeNearby(AI_Knowledge.TargetType.Prey))
        {
            LastEvaluatedBehavior = CurrentBehavior;
            CurrentBehavior = BehaviorList.Chase;
            return;
        }

        //If no Threat or Preys look if Food is nearby
        else if (MyKnowledge.bIsTypeNearby(AI_Knowledge.TargetType.Food))
        {
            LastEvaluatedBehavior = CurrentBehavior;
            CurrentBehavior = BehaviorList.Feed;
            return;
        }

        LastEvaluatedBehavior = CurrentBehavior;
        CurrentBehavior = BehaviorList.Wander;
    }

    //Hard Mode gives AI a giant sensor so they require a overhaul in the Behavior Selector
    private void BehaviorSelectorHard()
    {
        if (MyKnowledge.bIsTypeKnown(AI_Knowledge.TargetType.Threat))
        {
            //Prioritize Threat only if they are in a certain distance
            float distFromThreat = MyKnowledge.GetDistanceBetweenActors(transform.position, MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Threat));
            if (distFromThreat <= DistanceFleeHard)
            {
                //Like normal difficulty, can try to reach food if the threat isn't too close from it
                if (MyKnowledge.bIsTypeKnown(AI_Knowledge.TargetType.Food))
                {
                    float distFromFood = MyKnowledge.GetDistanceBetweenActors(transform.position, MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Food));
                    float distThreatFromFood = MyKnowledge.GetDistanceBetweenActors(MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Threat), 
                                                                                    MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Food));
                    {
                        if (distFromThreat > distFromFood && distThreatFromFood > 2.0f)
                        {
                            LastEvaluatedBehavior = CurrentBehavior;
                            CurrentBehavior = BehaviorList.Feed;
                            return;
                        }
                    }
                }
                LastEvaluatedBehavior = CurrentBehavior;
                CurrentBehavior = BehaviorList.Flee;
                return;
            }
        }

        //Unless Threat is near the Distance set, always prioritze chasing Prey
        if (MyKnowledge.bIsTypeKnown(AI_Knowledge.TargetType.Prey))
        {
            //If there's food in the way of the Prey we gather it if it's close enough and the Prey isn't a priority.
            if (MyKnowledge.bIsTypeKnown(AI_Knowledge.TargetType.Food))
            {
                float distFromFood = MyKnowledge.GetDistanceBetweenActors(transform.position, MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Food));
                float distFromPrey = MyKnowledge.GetDistanceBetweenActors(transform.position, MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Prey));
                if (distFromFood < DistanceFoodChaseHard && distFromPrey > distFromFood)
                {
                    LastEvaluatedBehavior = CurrentBehavior;
                    CurrentBehavior = BehaviorList.Feed;
                    return;
                }

                LastEvaluatedBehavior = CurrentBehavior;
                CurrentBehavior = BehaviorList.Chase;
                return;
            }
        }

        //If no Prey are detected focus on getting food so you get bigger and find new Preys.
        if (MyKnowledge.bIsTypeKnown(AI_Knowledge.TargetType.Food))
        {
            LastEvaluatedBehavior = CurrentBehavior;
            CurrentBehavior = BehaviorList.Feed;
            return;
        }

        LastEvaluatedBehavior = CurrentBehavior;
        CurrentBehavior = BehaviorList.Wander;
    }


    public BehaviorList GetPreviousBehavior()
    {
        return LastEvaluatedBehavior;
    }

    private void ManageDebuffStatus()
    {
        //If Debuff Status returns True when not slowed we start the timer
        if (bIsNotSlowed && CheckDebuffStatus())
        {
            DebuffTimer = Time.time;
            bIsNotSlowed = false;
        }

        //Once slowed and the timer is up we return to the original speed
        if (!bIsNotSlowed && Time.time > DebuffTimer + DebuffDuration)
        {
            bIsNotSlowed = true;
            GetComponent<NavMeshAgent>().speed = AIspeed;
        }
    }

    //Return if the AI is slower than his original speed.
    private bool CheckDebuffStatus()
    {
        return agent.speed < AIspeed;
    }

    private void OnCollisionEnter(Collision collision)
    {
        //Checks for Size of the collider and eats it if it is smaller
        if (collision.gameObject.tag == "AI_Pawn" || collision.gameObject.tag == "Player")
        {
            if (collision.transform.localScale.magnitude < transform.localScale.magnitude)
            {
                transform.localScale += new Vector3(0.1f, 0.1f, 0.1f);
                Destroy(collision.gameObject);
            }

            //Bandaid fix for when an AI and a Player tried to share the same position
            //AI would push the player on the Y axis rendering them unreachable which would break the game
            if (collision.gameObject.tag == "Player" && collision.transform.localScale.magnitude == transform.localScale.magnitude)
            {
                Vector3 Direction = (transform.position - collision.transform.position).normalized;

                transform.position = collision.transform.position + (Direction * (collision.transform.localScale.magnitude/1.5f));
            }
        }
    }

    private void StopAgent()
    {
        agent.isStopped = true;
        animator.SetFloat("Speed", 0);
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "Bullet")
        {
            bool bTempBool = collision.GetComponent<Bullet_Script>().bTargetIsHit;
            if (bTempBool)
            {
                StopAgent();
                animator.SetTrigger("Hit");
                OofSFX.Play();
            }
        }
    }

    public void HandleAnimationEvent(string message)
    {
        agent.isStopped = false;
    }

    private void OnDestroy()
    {
        string AIName = gameObject.name;
        SendDeadAIName?.Invoke(AIName);
    }
}
