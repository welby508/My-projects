using UnityEngine;
using UnityEngine.UI;

public class LoadingProgressBar : MonoBehaviour
{
    private Image img;
    private void Awake() => img = transform.GetComponent<Image>();

    private void Update()
    {
        img.fillAmount = Loader.GetProgress();
    }
}
