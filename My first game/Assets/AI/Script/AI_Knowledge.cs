
using System.Collections.Generic;
using UnityEngine;
using System;
using static UnityEngine.GraphicsBuffer;
using Unity.VisualScripting;


public class AI_Knowledge : MonoBehaviour
{
    public enum TargetType
    {
        Food,
        Neutral,
        Prey,
        Threat
    }

    public struct KnowledgeEntry
    {
        public string Name;
        public bool bIsNearby;
        public TargetType Type;
        public Vector3 LastKnownPosition;

        public KnowledgeEntry(string name, bool isnearby, TargetType targetType, Vector3 lastKnownPosition)
        {
            Name = name;
            bIsNearby = isnearby;
            LastKnownPosition = lastKnownPosition;
            Type = targetType;
        }
    }

    //Subscribing to the event so we can remove entries upon their gameobject being destroyed
    private void Awake()
    {
        Food_Script.SendDeadFoodName += ReceiveDeadTargetName;
        AI_Brain.SendDeadAIName += ReceiveDeadTargetName;
        Controller.SendDeadPlayerName += ReceiveDeadTargetName;
    }

    private List<GameObject> coverList = new List<GameObject>();

    //Add all the Cover node in the AI meomry
    private void Start()
    {
        foreach (GameObject cover in GameObject.FindGameObjectsWithTag("Cover"))
        {
            coverList.Add(cover);
        }
    }

    private void OnDestroy()
    {
        Food_Script.SendDeadFoodName -= ReceiveDeadTargetName;
        AI_Brain.SendDeadAIName -= ReceiveDeadTargetName;
        Controller.SendDeadPlayerName -= ReceiveDeadTargetName;
    }

    //AI that are currently nearby an entry owner that gets destroyed will need to lose that knowledge as it is no longer useful
    private void ReceiveDeadTargetName(String Name)
    {
        if (bIsAlreadyRegistered(Name))
        {
            int idx = getKnowledgeIdx(Name);
            if (knowledgeEntries[idx].bIsNearby == true)
            {
                RemoveEntry(idx);
            }
        }
    }

    protected List<KnowledgeEntry> knowledgeEntries = new List<KnowledgeEntry>();

    public Vector3 FindClosestTarget(TargetType type)
    {
        float distance = float.MaxValue;
        int idx = 0;
        for (int i = 0; i < knowledgeEntries.Count; i++)
        {
            if (knowledgeEntries[i].Type == type && knowledgeEntries[i].bIsNearby)
            {
                if (Vector3.Distance(transform.position, knowledgeEntries[i].LastKnownPosition) < distance)
                {
                    distance = Vector3.Distance(transform.position, knowledgeEntries[i].LastKnownPosition);
                    idx = i;
                }
            }
        }

        //Chase Target is required for post chase sequence
        if (type == TargetType.Prey)
        {
            ChaseTarget = knowledgeEntries[idx].Name;
        }

        return knowledgeEntries[idx].LastKnownPosition;
    }

    private String ChaseTarget;

    public String GetChaseTarget()
    {
        return ChaseTarget;
    }


    //Specific case to cheat a LKP of a "hidden" target after losing them during chase
    public Vector3 GetChaseTargetLKP(string target)
    {
        if (bIsAlreadyRegistered(target))
        {
            return knowledgeEntries[getKnowledgeIdx(target)].LastKnownPosition;
        }

        //If chase target no longer exist in memory it has been deleted 
        return transform.position;  
    }

    public Vector3 FindClosestValidCover(bool bWasCovered)
    {
        float distance = float.MaxValue;
        //If AI was previously in a cover when asking to search for a new one we give a min distance so they don't go too close
        float minDistance = (bWasCovered) ? minDistance = 10.0f : minDistance = 0;
        int idx = 0;
   
        for (int i = 0; i < coverList.Count; i++)
        {
            if (bIsCoverValid(coverList[i].transform.position))
            {
                if (Vector3.Distance(transform.position, coverList[i].transform.position) < distance &&
                    Vector3.Distance(transform.position, coverList[i].transform.position) > minDistance &&
                    !bIsThreatCloserToCover(i))
                {
                    distance = Vector3.Distance(transform.position, coverList[i].transform.position);
                    idx = i;
                }
            }
        }
        return coverList[idx].transform.position;
    }

    //We attempt to find a cover that's not close to the current Threat
    private bool bIsThreatCloserToCover(int idx)
    {
        Vector3 ThreatPos = FindClosestTarget(TargetType.Threat);

        return Vector3.Distance(ThreatPos, coverList[idx].transform.position) < Vector3.Distance(transform.position, coverList[idx].transform.position);
    }

    
    public float GetDistanceBetweenActors(Vector3 firstActor, Vector3 secondActor)
    {
        return Vector3.Distance(firstActor, secondActor);
    }

    public bool bIsTypeNearby(TargetType type)
    {
        if (bAIHasKnowledge())
        {
            foreach (KnowledgeEntry entry in knowledgeEntries)
            {
                if (entry.Type == type && entry.bIsNearby == true)
                {
                    return true;
                }
            }
        }
        return false;
    }

    public bool bIsTypeKnown(TargetType type)
    {
        if (bAIHasKnowledge())
        {
            foreach (KnowledgeEntry entry in knowledgeEntries)
            {
                if (entry.Type == type)
                {
                    return true;
                }
            }
        }
        return false;
    }

    protected bool bAIHasKnowledge()
    {
        return knowledgeEntries.Count > 0;
    }

    protected bool bIsAlreadyRegistered(string name)
    {
        foreach (KnowledgeEntry entry in knowledgeEntries)
        {
            if (entry.Name == name)
            { return true; }
        }
        return false;
    }

    protected int getKnowledgeIdx(string name)
    {
        int idx = 0;
        for (int i = 0; i < knowledgeEntries.Count; i++)
        {
            if (knowledgeEntries[i].Name == name)
            {
                idx = i;
            }
        }
        return idx;
    }

    public void RemoveEntry(int idx)
    {
        knowledgeEntries.RemoveAt(idx);
    }

    protected TargetType EvaluateTarget(float TargetSize, float SelfSize)
    {
        if (TargetSize > SelfSize)
        {
            return TargetType.Threat;
        }

        if (TargetSize < SelfSize)
        {
            return TargetType.Prey;
        }

        return TargetType.Neutral;
    }

    //Check to see if the target you are looking at is reachable without any meaningful obstacles
    public bool bIsTargetStraightLineReachable()
    {
        RaycastHit hit;
        Physics.SphereCast(transform.position, 0.4f ,transform.TransformDirection(Vector3.forward), out hit, Mathf.Infinity);
        Debug.DrawRay(transform.position, transform.TransformDirection(Vector3.forward) * hit.distance, Color.yellow);

        if (hit.collider != null)
        {
            //If the ray hits any of those technically your target is reachable
            if (hit.collider.CompareTag("AI_Pawn") || hit.collider.CompareTag("Player") || hit.collider.CompareTag("Bullet") || hit.collider.CompareTag("Food"))
            {
                return true;
            }
        }

        return false;
    }

    //Attempting to find a Cover position that is at least behind a walls, needs re-work.
    public bool bIsCoverValid(Vector3 inPos)
    {
        RaycastHit hit;
        Physics.Linecast(transform.position, inPos, out hit);

        if (hit.collider.CompareTag("Cover Wall"))
        {
            return true;
        }

        return false;
    }
}
