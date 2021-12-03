#include <stdio.h>


#define MAX_NUM 255;
int main() {
    int hp;
    int attack;
    int defence;

    int m_hp, m_attack, m_defence;
    int monster_num;
    scanf("%d %d %d", &attack, &defence, &hp);
    scanf("%d", &monster_num);
    int i;
    for(i = 0; i < monster_num; i++) {
        scanf("%d %d %d", &m_attack, &m_defence, &m_hp);
        int real_attack = attack - m_defence;
        int m_read_attack = m_attack - defence;
        int defeat_turn = m_hp / real_attack;
        if (defeat_turn == 0) defeat_turn = 1;
        hp -= (defeat_turn - 1) * m_read_attack;
        if(hp <= 0) break;
    }
    printf("%d", i);
    return 0;
}