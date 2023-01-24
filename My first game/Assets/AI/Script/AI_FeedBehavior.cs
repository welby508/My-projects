using UnityEngine;
using UnityEngine.AI;

public class AI_FeedBehavior : AI_BehaviorBase
{
    private AI_Knowledge MyKnowledge;
    private NavMeshAgent agent;

    Vector3 TargetPosition;

    public AI_FeedBehavior(AI_Knowledge myKnowledge, NavMeshAgent agent)
    {
        MyKnowledge = myKnowledge;
        this.agent = agent;
    }

    public override void Tick()
    {
        Evaluate();
        BehaviorAction();
    }

    //Find food
    public override void Evaluate()
    {
        TargetPosition = MyKnowledge.FindClosestTarget(AI_Knowledge.TargetType.Food);
    }

    //Go to Food
    public override void BehaviorAction()
    {
        agent.transform.LookAt(TargetPosition);
        agent.SetDestination(TargetPosition);
    }
}
