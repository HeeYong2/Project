using UnityEngine;

namespace CompleteProject
{
    public class EnemyManager : MonoBehaviour
    {
        Transform player;
        public PlayerHealth playerHealth;       // Reference to the player's heatlh.
        public GameObject enemy;                // The enemy prefab to be spawned.
        public float spawnTime = 10f;            // How long between each spawn.

        public int Max_Obj_Cnt;
        public static int[] Obj_Cnt = new int[4];
        void Start ()
        {
            for (int i = 0; i < 4; i++)
            {
                Obj_Cnt[i] = 0; 
            }
           
            switch (enemy.name)
            {
                case "ZomBunny":
                    Max_Obj_Cnt = 10;
                    break;
                case "ZomBear":
                    Max_Obj_Cnt = 8;
                    break;
                case "Hellephant":
                    Max_Obj_Cnt = 2;
                    break;
                case "FirstAid":
                    Max_Obj_Cnt = 3;
                    break;
            }


            player = GameObject.Find("Player").transform;
            // Call the Spawn function after a delay of the spawnTime and then continue to call after the same amount of time.
            InvokeRepeating ("Spawn", spawnTime, spawnTime);



        }


        void Spawn ()
        {
            // If the player has no health left...
            if(playerHealth.currentHealth <= 0f)
            {
                // ... exit the function.
                return;
            }
       
                int rand_x = Random.Range(0, 2);
                int rand_z = Random.Range(0, 2);
                 float temp_x = 0;
                float temp_z = 0;
                if(rand_x ==0)
                    temp_x = Random.Range(10, 21);
                else
                    temp_x = Random.Range(-10, -21);

                if (rand_z == 0)
                    temp_z = Random.Range(10, 21);
                else
                    temp_z = Random.Range(-10, -21);

                switch (enemy.name)
                {
                    case "ZomBunny":

                        if (Obj_Cnt[0] <= Max_Obj_Cnt)
                        {
                            Obj_Cnt[0]++;
                            Instantiate(enemy, new Vector3(player.position.x + temp_x, player.position.y, player.position.z + temp_z), player.rotation);
                        }
                        break;
                    case "ZomBear":
                        if (Obj_Cnt[1] <= Max_Obj_Cnt)
                        {
                            Obj_Cnt[1]++;
                            Instantiate(enemy, new Vector3(player.position.x + temp_x, player.position.y, player.position.z + temp_z), player.rotation);
                        }
                        break;
                    case "Hellephant":
                        if (Obj_Cnt[2] <= Max_Obj_Cnt)
                        {
                            Obj_Cnt[2]++;
                            Instantiate(enemy, new Vector3(player.position.x + temp_x, player.position.y, player.position.z + temp_z), player.rotation);
                        }
                        break;
                    case "FirstAid":
                        if (Obj_Cnt[3] <= Max_Obj_Cnt)
                        {
                            Obj_Cnt[3]++;
                            Instantiate(enemy, new Vector3(player.position.x + temp_x, player.position.y, player.position.z + temp_z), player.rotation);
                        }
                        break;
                }


            // Find a random index between zero and one less than the number of spawn points.

            // Create an instance of the enemy prefab at the randomly selected spawn point's position and rotation.
            //
        }
    }
}