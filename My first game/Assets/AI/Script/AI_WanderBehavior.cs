
using TMPro;
using UnityEngine;
using UnityEngine.AI;

public class AI_WanderBehavior : AI_BehaviorBase
{
    private AI_Knowledge MyKnowledge;
    private NavMeshAgent agent;
    private AI_Brain MyBrain;

    Vector3 WanderPosition;
    bool bPositionReached = true;

    public AI_WanderBehavior(AI_Knowledge MyKnowledge, NavMeshAgent agent, AI_Brain MyBrain)
    {
        this.MyKnowledge = MyKnowledge;
        this.agent = agent;
        this.MyBrain = MyBrain;
    }

    public override void Tick()
    {
        Evaluate();
        BehaviorAction();
    }

    public override void Evaluate()
    {
        //Before initiating a Wander AI's will attempt to reach a Chase target in case they've lost one
        if(MyBrain.GetPreviousBehavior() == AI_Brain.BehaviorList.Chase && MyKnowledge.bIsTypeKnown(AI_Knowledge.TargetType.Prey))
        {
            bPositionReached = false;
            WanderPosition = MyKnowledge.GetChaseTargetLKP(MyKnowledge.GetChaseTarget());
        }

        //If last WanderPosition was reached or we come from a Flee we want a new Position
        if(bPositionReached || MyBrain.GetPreviousBehavior() == AI_Brain.BehaviorList.Flee)
        {
            FindRandomPosition();
        }
       
        //Check to see if a Position is Reached
        if(Vector3.Distance(WanderPosition, agent.transform.position) < (1.0f * agent.transform.localScale.magnitude))
        {
            bPositionReached = true;
        }

    }
    //Go to Wander Position
    public override void BehaviorAction()
    {
        agent.transform.LookAt(WanderPosition);
        agent.SetDestination(WanderPosition);
    }

    private void FindRandomPosition()
    {
        //Uses manually placed Min and Max of the GameScene (See WorldBound script)
        WorldBound worldBounds = GameObject.FindObjectOfType<WorldBound>();
        Vector3 min = worldBounds.Min.position;
        Vector3 max = worldBounds.Max.position;
        bool bPositionFound = false;

        //Ensure the position is on Navmesh
        while(!bPositionFound)
        {
            Vector3 TempWanderPosition = new Vector3(
            Random.Range(min.x, max.x),
            0.09f,
            Random.Range(min.z, max.z));


            Vector3 randomPoint = TempWanderPosition + Random.insideUnitSphere * 10.0f;
            NavMeshHit hit;

            if (NavMesh.SamplePosition(randomPoint, out hit, 1.0f, NavMesh.AllAreas))
            {
                bPositionFound = true;
                WanderPosition = hit.position;
            }
        }
        bPositionReached = false;
    }
}
