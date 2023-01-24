using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static AI_Knowledge;

public class AI_ProximitySensor : AI_Knowledge
{
    private void OnTriggerEnter(Collider other)
    {
        //If entry is already present in knowledge we update with new info
        if (bIsAlreadyRegistered(other.name))
        {
            int idx = getKnowledgeIdx(other.gameObject.name);
            KnowledgeEntry entry = knowledgeEntries[idx];
            entry.bIsNearby = true;
            if (other.CompareTag("Player") || other.CompareTag("AI_Pawn") && !other.isTrigger)
            {
                float targetSize = other.transform.localScale.magnitude;
                entry.Type = EvaluateTarget(targetSize, transform.localScale.magnitude);
            }
            knowledgeEntries[idx] = entry;
            return;
        }

        string name = other.gameObject.name;
        Vector3 targetPosition = other.transform.position;

        if (other.CompareTag("Food"))
        {
            KnowledgeEntry newEntry = new KnowledgeEntry(name, true, TargetType.Food, targetPosition);
            knowledgeEntries.Add(newEntry);
        }

        if (other.CompareTag("Player") || other.CompareTag("AI_Pawn") && !other.isTrigger)
        {
            float targetSize = other.transform.localScale.magnitude;
            TargetType targetType = EvaluateTarget(targetSize, transform.localScale.magnitude);

            KnowledgeEntry newEntry = new KnowledgeEntry(name, true, targetType, targetPosition);
            knowledgeEntries.Add(newEntry);
        }
    }

    private void OnTriggerStay(Collider other)
    {
        //If entry is already present in knowledge we update with new info
        if (bIsAlreadyRegistered(other.gameObject.name))
        {
            int idx = getKnowledgeIdx(other.gameObject.name);
            KnowledgeEntry entry = knowledgeEntries[idx];
            entry.LastKnownPosition = other.transform.position;
            if (other.CompareTag("Player") || other.CompareTag("AI_Pawn") && !other.isTrigger)
            {
                float targetSize = other.transform.localScale.magnitude;
                entry.Type = EvaluateTarget(targetSize, transform.localScale.magnitude);
            }
            knowledgeEntries[idx] = entry;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        //When entry owner is no longer present in sensor we set it to "hidden" with the bIsNearby bool
        if (bIsAlreadyRegistered(other.gameObject.name))
        {
            int idx = getKnowledgeIdx(other.gameObject.name);
            KnowledgeEntry entry = knowledgeEntries[idx];
            entry.bIsNearby = false;
            knowledgeEntries[idx] = entry;
        }
    }

}
