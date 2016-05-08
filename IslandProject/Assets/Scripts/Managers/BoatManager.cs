using UnityEngine;
using System.Collections;

public class BoatManager : MonoBehaviour
{

    public Transform Boat_trs;

    Vector3[] Boat_Pos = new Vector3[5]
    {
        new Vector3(-66,-1,316),new Vector3(156.4f,-1,360),new Vector3(-60,-1,-45.5f),new Vector3(277,-1,-72),new Vector3(183,-1,212)
    };
    Vector3[] Boat_Rotation = new Vector3[5]
    {
        new Vector3(0,0,0),new Vector3(0,80,0),new Vector3(8,-44,2),new Vector3(8,-70,-1.4f),new Vector3(-7,-270,4)
    };

    // Use this for initialization
    void Start()
    {
        Boat_trs = this.transform;

        int temp = Random.Range(0,5);
        Boat_trs.position = Boat_Pos[temp];
        Boat_trs.eulerAngles =  Boat_Rotation[temp];
    }


}