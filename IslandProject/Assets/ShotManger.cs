using UnityEngine;
using System.Collections;
using UnityEngine.UI;

namespace CompleteProject
{
    public class ShotManger : MonoBehaviour
    {

        Text text;                      // Reference to the Text component.

        AudioSource this_Audio;
        void Awake()
        {
            // Set up the reference.
            text = GetComponent<Text>();
            this_Audio = GetComponent<AudioSource>();
            // Reset the score.

        }


        void LateUpdate()
        {
            // Set the displayed text to be the word "Score" followed by the score value.
            text.text = "SHOT: " + PlayerShooting.SHOT + "/" + PlayerShooting.MAX_SHOT;

            if (Input.GetKeyDown(KeyCode.R))
            {
                PlayerShooting.Recharge = 0.3f;
                this_Audio.Play();
            }
        }
    }

}