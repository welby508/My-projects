using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FadeIn: MonoBehaviour
{

    public Color SartingColor;
    public float Duration;
    private Color targetColor;
    private Image ComponentToChange;

    private void Awake()
    {
        targetColor = GetComponent<Image>().color;
        GetComponent<Image>().color = SartingColor;
        ComponentToChange = GetComponent<Image>();
    }
    void Start()
    {

        StartCoroutine(LerpFunction(targetColor, Duration));
    }
    IEnumerator LerpFunction(Color endValue, float duration)
    {
        float time = 0;
        Color startValue = SartingColor;
        while (time < duration)
        {
            ComponentToChange.color = Color.Lerp(startValue, endValue, time / duration);
            time += Time.deltaTime;
            yield return null;
        }
        ComponentToChange.color = endValue;
    }

}
