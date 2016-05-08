using UnityEngine;
using UnityEngine.UI;
using System.Collections;

namespace CompleteProject
{
    public class TimerManager : MonoBehaviour
    {
        public static float Timer_Cnt;

        public Transform Water_Trs;
        Text Game_Timer;

        float Water_Cnt;

        Vector3 Start_Vectoer = new Vector3(150, -1, 152);
        Vector3 First_Vectoer = new Vector3(150, -0.3f, 152);
        Vector3 GameOVer_Vectoer = new Vector3(150, 0, 152);

        PlayerHealth Ph;
        void Awake ()
        {
            Ph = GameObject.Find("Player").GetComponent<PlayerHealth>();
            Water_Cnt = 0;
            Game_Timer = GetComponent<Text>();
            Timer_Cnt = 300;
            Water_Trs.localPosition = new Vector3(150,-1f,152); 
        }


        void LateUpdate ()
        {
            if (Ph.ps.Equals(PlayerHealth.PlayState.Play))
            {
                if (Timer_Cnt > 0)
                    Timer_Cnt -= Time.deltaTime;

                Water_Cnt += Time.deltaTime * 0.016f;
                // Set the displayed text to be the word "Score" followed by the score value.
                if (Timer_Cnt >= 60)
                {
                    Game_Timer.text = "남은시간: " + (int)(Timer_Cnt / 60) + "분" + (int)(Timer_Cnt % 60) + "초";
                }
                else
                {
                    Game_Timer.text = "남은시간: " + (int)(Timer_Cnt % 60) + "초";
                }
            }
            

            
            
        }
        void FixedUpdate()
        {
            if (Ph.ps.Equals(PlayerHealth.PlayState.Play))
            {
                if (Water_Cnt <= 1)
                {
                    Water_Trs.localPosition = Vector3.Lerp(Start_Vectoer, First_Vectoer, Water_Cnt);
                }
                else if (Water_Cnt <= 2)
                {
                    Water_Trs.localPosition = Vector3.Lerp(First_Vectoer, Start_Vectoer, Water_Cnt-1);
                }
                else if (Water_Cnt <= 3)
                {
                    Water_Trs.localPosition = Vector3.Lerp(Start_Vectoer, First_Vectoer, Water_Cnt - 2);
                }
                else if (Water_Cnt <= 4)
                {
                    Water_Trs.localPosition = Vector3.Lerp(First_Vectoer, Start_Vectoer, Water_Cnt - 1);
                }
                else if (Water_Cnt <= 5)
                {
                    Water_Trs.localPosition = Vector3.Lerp(Start_Vectoer, GameOVer_Vectoer, Water_Cnt - 2);
                }
                else
                {
                    Water_Trs.localPosition = new Vector3(150, 1, 152);
                }
            }
        }
    }
}