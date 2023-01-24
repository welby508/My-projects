using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Reference: https://gamedevbeginner.com/the-right-way-to-lerp-in-unity-with-examples/
public class SlideAnimation : MonoBehaviour
{
    private Vector3 targetValue;
    public int offsetX;
    public int offsetY;
    public float duration;

    private void OnEnable()
    {
        targetValue = transform.position;
        transform.position = new Vector3(targetValue.x - offsetX, targetValue.y - offsetY, targetValue.z);
        StartCoroutine(EaseInLerpFunction(targetValue, duration));
    }
    IEnumerator EaseInLerpFunction(Vector3 endValue, float duration)
    {
        float time = 0;
        Vector3 startValue = transform.position;
        while (time < duration)
        {
            float t = time / duration;
            t = t * t * (3f - 2f * t);
            transform.position = Vector3.Lerp(startValue, endValue, t);
            time += Time.deltaTime;
            yield return null;
        }
        transform.position = endValue;
    }
}
