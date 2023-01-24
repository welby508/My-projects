using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using static AI_Brain;

public class AI_FleeBehavior : AI_BehaviorBase
{
    private AI_Knowledge MyKnowledge;
    private NavMeshAgent agent;
    private Shoot_Script shoot;
    private AI_Brain brain;

    Vector3 TargetPosition;
    Vector3 CoverPosition;

    private bool bIsFleeingAgain = false;

    private enum BehaviorSequence
    {
        Search, Reach, Wait
    }

    private BehaviorSequence FleeSequence = BehaviorSequence.Search;
 
    public AI_FleeBehavior(AI_Knowledge myKnowledge, NavMeshAgent agent, Shoot_Script shoot, AI_Brain brain)
    {
        MyKnowledge = myKnowledge;
        this.agent = agent;
        this.shoot = shoot;
        this.brain = brain;
    }

    public override void Tick()
    {
        Evaluate();
        BehaviorAction();
    }

    public override void Evaluate()
    {
        //If we are entering Flee for the first time we set the proper variable (see Knowledge and FindClosestValidCover())
        if (brain.GetPreviousBehavior() != AI_Brain.BehaviorList.Flee)
        {
            bIsFleeingAgain = false;
            FleeSequence = BehaviorSequence.Search;
        }

        //First part of the Sequence is getting a Cover Position
        if (FleeSequence == BehaviorSequence.Search)
        {
            CoverPosition = MyKnowledge.FindClosestValidCover(bIsFleeingAgain);
            FleeSequence = BehaviorSequence.Reach;
        }
    }

    public override void BehaviorAction()
    {
        //Second part of the sequence is moving the AI toward the cover
        if (FleeSequence == BehaviorSequence.Reach)
        {

            if (Vector3.Distance(CoverPosition, agent.transform.position) < (1.0f * agent.transform.localScale.magnitude))
            {
                FleeSequence = BehaviorSequence.Wait;
            }
            else
            {
                agent.SetDestination(CoverPosition);
            }
        }

        //Third part of the sequence is waiting for the Threat at the corner of the cover
        if(FleeSequence == BehaviorSequence.Wait)
        {
            TargetPosition = MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Threat);
            TargetPosition.y = agent.transform.position.y; //shoot straight
            agent.transform.LookAt(TargetPosition);

            //Fourth optional part of the sequence is shooting the threat if they peak the cover
            //We then renitiate the sequence by fleeing to another point 
            if (MyKnowledge.bIsTargetStraightLineReachable())
            {
                shoot.AllowShooting();
                FleeSequence = BehaviorSequence.Search;
                bIsFleeingAgain = true;
            }
        }
    }
}
