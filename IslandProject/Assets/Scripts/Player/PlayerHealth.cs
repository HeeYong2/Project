using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.SceneManagement;

namespace CompleteProject
{
    public class PlayerHealth : MonoBehaviour
    {
        public enum PlayState { Play, Clear };

        public int startingHealth = 100;                            // The amount of health the player starts the game with.
        public int currentHealth;                                   // The current health the player has.
        public Slider healthSlider;                                 // Reference to the UI's health bar.
        public Image damageImage;                                   // Reference to an image to flash on the screen on being hurt.
        public AudioClip deathClip;                                 // The audio clip to play when the player dies.
        public float flashSpeed = 5f;                               // The speed the damageImage will fade at.
        public Color flashColour = new Color(1f, 0f, 0f, 0.1f);     // The colour the damageImage is set to, to flash.


        Animator anim;                                              // Reference to the Animator component.
        AudioSource playerAudio;                                    // Reference to the AudioSource component.
        PlayerMovement playerMovement;                              // Reference to the player's movement.
        PlayerShooting playerShooting;                              // Reference to the PlayerShooting script.
        bool isDead;                                                // Whether the player is dead.
        bool damaged;                                               // True when the player gets damaged.


        public bool Clear = false;

        public PlayState ps;

        public GameObject Damage;
        TextMesh Damage_text;
        void Awake()
        {
            Damage = Resources.Load("Damage_Text") as GameObject;
            Damage_text = Damage.GetComponent<TextMesh>();

            Clear = false;
            ps = PlayState.Play;
            // Setting up the references.
            anim = GetComponent<Animator>();
            playerAudio = GetComponent<AudioSource>();
            playerMovement = GetComponent<PlayerMovement>();
            playerShooting = GetComponentInChildren<PlayerShooting>();

            // Set the initial health of the player.
            currentHealth = startingHealth;
        }


        void Update()
        {
            if (ps.Equals(PlayState.Play))
            {
                // If the player has just been damaged...
                if (damaged)
                {
                    // ... set the colour of the damageImage to the flash colour.
                    damageImage.color = flashColour;
                }
                // Otherwise...
                else
                {
                    // ... transition the colour back to clear.
                    damageImage.color = Color.Lerp(damageImage.color, Color.clear, flashSpeed * Time.deltaTime);
                }

                // Reset the damaged flag.
                damaged = false;
            }
            else
            {
                if (Input.GetKeyDown(KeyCode.R))
                {
                    SceneManager.LoadScene(0);
                }
            }

        }


        public void TakeDamage(int amount)
        {
            // Set the damaged flag so the screen will flash.
            Damage_text.color = Color.blue;

            damaged = true;


            GameObject obj = Instantiate(Damage);
            obj.transform.position = this.transform.position;

            StartCoroutine(TextDestoy(obj));
            Damage_text.text = amount.ToString();
            // Reduce the current health by the damage amount.
            currentHealth -= amount;

            // Set the health bar's value to the current health.
            healthSlider.value = currentHealth;

            // Play the hurt sound effect.
            playerAudio.Play();

            // If the player has lost all it's health and the death flag hasn't been set yet...
            if (currentHealth <= 0 && !isDead)
            {
                // ... it should die.
                Death();
            }
        }
        IEnumerator TextDestoy(GameObject obj)
        {


            yield return new WaitForSeconds(0.15f);
            Destroy(obj);
        }

        public void Death()
        {
            // Set the death flag so this function won't be called again.
            isDead = true;

            // Turn off any remaining shooting effects.
            playerShooting.DisableEffects();

            // Tell the animator that the player is dead.
            anim.SetTrigger("Die");

            // Set the audiosource to play the death clip and play it (this will stop the hurt sound from playing).
            playerAudio.clip = deathClip;
            playerAudio.Play();

            // Turn off the movement and shooting scripts.
            playerMovement.enabled = false;
            playerShooting.enabled = false;
        }



        public void RestartLevel()
        {
            // Reload the level that is currently loaded.
            SceneManager.LoadScene(0);
        }

        void OnCollisionEnter(Collision collision)
        {
            if (ps == PlayState.Play)
            {
                if (collision.gameObject.name == "Boat")
                {
                    Clear = true;
                    ps = PlayState.Clear;
                }
                if (collision.gameObject.name == "Water")
                {
                    currentHealth = 0;
                    Death();
                }
                if (collision.gameObject.name == "FirstAid(Clone)")
                {
                    GameObject.Find("Health_KitManager").GetComponent<AudioSource>().Play();
                    currentHealth = startingHealth;
                    EnemyManager.Obj_Cnt[3]--;
                    healthSlider.value = currentHealth;
                    Destroy(collision.gameObject);
                }
            }

        }
    }
}