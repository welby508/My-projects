using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.SceneManagement;


//reference : https://www.youtube.com/watch?v=3I5d2rUJ0pE&ab_channel=CodeMonkey
public static class Loader
{
    private class LoadingMonoBehavior : MonoBehaviour { }

    private static AsyncOperation asyncOperation;
    public enum Scene {
        SplashScreen,
        GameScene,
        Loading,
        MainMenu,
    }

    private static Action onLoaderCallback;
    public static void Load(Scene scene) {


        //Set the target scene to load
        onLoaderCallback = () =>
        {
            GameObject loadingGameObject = new GameObject("Loading Game");
            loadingGameObject.AddComponent<LoadingMonoBehavior>().StartCoroutine(LoadSceneAsync(scene));
        };

        //loading screen
        SceneManager.LoadScene(Scene.Loading.ToString());

    }

    private static IEnumerator LoadSceneAsync(Scene scene) {
        yield return null;

        asyncOperation = SceneManager.LoadSceneAsync(scene.ToString());

        while (!asyncOperation.isDone) {
            yield return null;
        }
    }

    public static float GetProgress() {

        return (asyncOperation != null ? asyncOperation.progress : 1f);
    }

    public static void LoaderCallback() {
        if (onLoaderCallback != null)
        {
            onLoaderCallback();
            onLoaderCallback = null;
        }
    
    }
}
