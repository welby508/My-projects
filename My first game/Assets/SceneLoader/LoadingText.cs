using UnityEngine;
using UnityEngine.UI;

public class LoadingText : MonoBehaviour
{
    private void Update()
    {
        float percentage = Loader.GetProgress() / 100;
        GetComponent<TMPro.TextMeshProUGUI>().text = "Loading... " + percentage.ToString() + "%";
    }
}
