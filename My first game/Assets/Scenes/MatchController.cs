using System;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.EventSystems;

public class MatchController : MonoBehaviour
{
    public static MatchController instance;
    public GameObject AI_Pawn;
    public GameObject Player1;
    public GameObject FoodObj;
    public bool isLoaded;

    private int FoodCount;
    private int AiCount = 0;
    private int FoodMax = 20;

    private GameObject p1;
    private List<GameObject> AIList = new ();
    private List<Vector3> SpawnList = new () ;
    // Start is called before the first frame update
    void Start()
    {
        Food_Script.SendDeadFoodName += (String Name) => { FoodCount--; };
        AI_Brain.SendDeadAIName += (String Name) => { AiCount--; };
    }

    private void Awake()
    {
        if (instance is null)
        {
            instance = this;

        }
        else if (instance != this)
        {
            Destroy(instance);
            instance = this;
        }
        PopulateSpawnPoints();
        GenerateGameObjects();
        isLoaded = true;

    }

    private void Update()
    {
        if (AiCount == 0)
        {
            Debug.Log("ai is 0");
            GameManager.PlayersWon = true;
            GameManager.instance.UpdateGameState(GameManager.GameState.MainMenu);
        }
        if (p1.IsDestroyed())
        {
            Debug.Log("p1 is destroyed");
            GameManager.PlayersWon = false;
            GameManager.instance.UpdateGameState(GameManager.GameState.MainMenu);
        }
    }

    private void applyAvatarCustomization(GameObject player)
    {
        Color primaryColor = GameManager.instance.GetPrimaryColor();
        Color secondaryColor = GameManager.instance.GetSecondaryColor();
        Mesh hatMesh = GameManager.instance.GetHatMesh();

        if (primaryColor != new Color())
        {
            player.transform.GetChild(1).GetComponent<Renderer>().material.SetColor("_Color", primaryColor);
        }
        if (secondaryColor != new Color())
        {
            player.transform.GetChild(0).GetChild(0).GetChild(2).GetComponent<Renderer>().material.SetColor("_Color", secondaryColor);
        }
        if (hatMesh != null)
        {
            player.transform.GetChild(0).GetChild(0).GetChild(2).GetComponent<MeshFilter>().mesh = hatMesh;
        }
    }

    private void GenerateGameObjects()
    {

        p1 = Instantiate(GameManager.instance.GetAvatar(), SpawnList[0], Quaternion.identity);
        applyAvatarCustomization(p1);

        Debug.Log(GameManager.NumberOfAi);

        for (int i = 0; i < GameManager.NumberOfAi; i++)
        {
            //Need different names for objects for AI Knowledge to sense properly
            GameObject Ai = Instantiate(AI_Pawn, SpawnList[i + 2], Quaternion.identity);
            AiCount++;
            Ai.name= "AI_Pawn " + i;
            AIList.Add(Ai);
        }

        FoodCount = 0;
        while (FoodCount < FoodMax)
        {
            WorldBound worldBounds = GameObject.FindObjectOfType<WorldBound>();
            Vector3 min = worldBounds.Min.position;
            Vector3 max = worldBounds.Max.position;

            Vector3 TempFoodPosition = new Vector3(
            UnityEngine.Random.Range(min.x, max.x),
            0.50f,
            UnityEngine.Random.Range(min.z, max.z));

            Vector3 randomPoint = TempFoodPosition + UnityEngine.Random.insideUnitSphere * 10.0f;
            NavMeshHit hit;

            if (NavMesh.SamplePosition(randomPoint, out hit, 1.0f, NavMesh.AllAreas))
            {
                //Need different names for objects for AI Knowledge to sense properly
                GameObject Food = Instantiate(FoodObj, new Vector3(hit.position.x, 0.50f, hit.position.z), Quaternion.identity);
                Food.name = "Food" + FoodCount;
                FoodCount++;
            }
        }
    }

    //List of predetermined spawn points
    private void PopulateSpawnPoints()
    {
        SpawnList.Add(new Vector3(-17f, 0f, -13f));
        SpawnList.Add(new Vector3(17f, 0f, -13f));
        SpawnList.Add(new Vector3(-17f, 0f, 17f));
        SpawnList.Add(new Vector3(17f, 0f, 17f));
        SpawnList.Add(new Vector3(0.25f, 0f, -17f));
        SpawnList.Add(new Vector3(0.25f, 0f, 17f));
    }

    public int GetRemainingAINumber()
    {
        return AiCount;
    }

}

