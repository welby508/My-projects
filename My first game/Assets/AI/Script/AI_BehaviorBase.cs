using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class AI_BehaviorBase
{
    //Update function of a behavior
    public abstract void Tick();
    //Look for the position required in the action
    public abstract void Evaluate();
    //Actions performed by the behavior following the evaluation
    public abstract void BehaviorAction();

}
