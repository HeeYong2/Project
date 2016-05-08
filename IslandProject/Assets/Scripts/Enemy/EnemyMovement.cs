using UnityEngine;
using System.Collections;

namespace CompleteProject
{
    public class EnemyMovement : MonoBehaviour
    {
        public enum Enemy_State { Idle, chase, Berserker, Dead };

        Transform player;               // Reference to the player's position.
        PlayerHealth playerHealth;      // Reference to the player's health.
        EnemyHealth enemyHealth;        // Reference to this enemy's health.
        NavMeshAgent nav;               // Reference to the nav mesh agent.

        Transform this_trs;//자기 자신의 트렌스폼

        public Enemy_State Enemy_s;

        bool Attack;

        int Range;

        Vector3 temp_Vector = new Vector3();

        float Move_cnt;

        float speed;
        void Awake ()
        {
            Enemy_s = Enemy_State.Idle; // 쉬는상태

            this_trs = this.transform;


            Attack = false;
            // Set up the references.
            player = GameObject.Find("Player").transform;
            playerHealth = player.GetComponent <PlayerHealth> ();
            enemyHealth = GetComponent <EnemyHealth> ();
            nav = GetComponent <NavMeshAgent> ();

            nav.autoBraking = true;
            nav.autoRepath = true;

            temp_Vector = this_trs.position;
            Move_cnt = 0;

            speed = nav.speed;
            switch (this.gameObject.name)
            {
                case "ZomBunny(Clone)":
                    Range = 6;
                    break;
                case "ZomBear(Clone)":
                    Range = 8;
                    break;
                case "Hellephant(Clone)":
                    Range = 10;
                    break;
                case "FirstAid(Clone)":
                    Range = 0;
                    break;

            }
            
        }


        void Update()
        {
            if (playerHealth.ps.Equals(PlayerHealth.PlayState.Play))
            {
                switch (Enemy_s)
                {
                    case Enemy_State.Idle:

                        if (this_trs.position.x == temp_Vector.x && this_trs.position.z == temp_Vector.z)
                        {
                            temp_Vector = new Vector3(this_trs.position.x + Random.Range(-Range, Range), this_trs.position.y, this_trs.position.z + Random.Range(-Range, Range));
                        }
                        else
                        {
                            if (nav.enabled)
                                nav.SetDestination(temp_Vector);
                            Move_cnt += Time.deltaTime;

                            if (Move_cnt >= 5)
                            {
                                temp_Vector = new Vector3(this_trs.position.x + Random.Range(-Range, Range), this_trs.position.y, this_trs.position.z + Random.Range(-Range, Range));
                            }
                        }


                        break;
                    case Enemy_State.chase:
                        if (nav.enabled)
                            nav.SetDestination(player.position);
                        break;
                    case Enemy_State.Berserker:
                        if (nav.enabled)
                            nav.SetDestination(player.position);
                        nav.speed = speed * 2;
                        break;
                    case Enemy_State.Dead:

                        nav.enabled = false;
                        break;

                }
            }
         
        }

        void LateUpdate ()
        {

            
            if (enemyHealth.currentHealth <= 0) 
            {
                Enemy_s = Enemy_State.Dead;
            } 
            
            // If the enemy and the player have health left...
            if (Enemy_s != Enemy_State.Dead)
            {
       

                if (enemyHealth.currentHealth <= enemyHealth.startingHealth * 0.5f)
                {
                    Enemy_s = Enemy_State.Berserker;
                    this_trs.localScale = Vector3.one * 2;
                }
                else if (enemyHealth.currentHealth != enemyHealth.startingHealth)
                {
                    Attack = true;
                    Enemy_s = Enemy_State.chase;
                }

                //몬스터 좌표 중심으로 x+Range x-Range 반경에 플레이어가 들어오면 트루 
                if (!Attack)
                {
                    if (this_trs.position.x - Range <= player.position.x && this_trs.position.x + Range >= player.position.x)
                    {
                        //몬스터 좌표 중심으로 z+Range z-Range 반경에 플레이어가 들어오면 트루
                        if (this_trs.position.z - Range <= player.position.z && this_trs.position.z + Range >= player.position.z)
                        {
                            Attack = true;
                            Enemy_s = Enemy_State.chase;
                        }
                    }


                    if (this_trs.position.x - 25 >= player.position.x || this_trs.position.x + 25 <= player.position.x &&this_trs.position.z - 25 >= player.position.z || this_trs.position.z + 25 <= player.position.z)
                    {
                        //일정이상 몬스터는 죽임
                        switch (this.gameObject.name)
                        {
                            case "ZomBunny(Clone)":
                                EnemyManager.Obj_Cnt[0]--;
                                break;
                            case "ZomBear(Clone)":
                                EnemyManager.Obj_Cnt[1]--;
                                break;
                            case "Hellephant(Clone)":
                                EnemyManager.Obj_Cnt[2]--;
                                break;
                            case "FirstAid(Clone)":
                                EnemyManager.Obj_Cnt[3]--;
                                break;
                        }
                        Destroy(this.gameObject);
                    }
                }

            }
            
        }
    }
}