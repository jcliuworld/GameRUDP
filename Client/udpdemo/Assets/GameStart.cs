using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameStart : MonoBehaviour {

    // Use this for initialization
    private GameObject go;
	void Start () {

        go = new GameObject();

        go.name = "Net";

        go.AddComponent<LuaManager>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
