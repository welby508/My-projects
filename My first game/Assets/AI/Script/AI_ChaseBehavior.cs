using UnityEngine;
using UnityEngine.AI;

public class AI_ChaseBehavior : AI_BehaviorBase
{
    private AI_Knowledge MyKnowledge;
    private NavMeshAgent agent;
    private Shoot_Script shoot;


    Vector3 TargetPosition;

    public AI_ChaseBehavior(AI_Knowledge myKnowledge, NavMeshAgent agent, Shoot_Script shoot)
    {
        MyKnowledge = myKnowledge;
        this.agent = agent;
        this.shoot = shoot;
    }

    public override void Tick()
    {
        Evaluate();
        BehaviorAction();
    }

    //Find Prey
    public override void Evaluate()
    {
        TargetPosition = MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Prey);
    }

    //Go to Prey and shoot if it's reachable
    public override void BehaviorAction()
    {

        agent.transform.LookAt(TargetPosition);
        if (MyKnowledge.bIsTargetStraightLineReachable())
        {
            shoot.AllowShooting();
        }
        agent.SetDestination(TargetPosition);

    }
}
