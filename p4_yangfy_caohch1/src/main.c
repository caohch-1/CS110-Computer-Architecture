#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"
#include <stdio.h>

void Inp_init(void) {
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}

void Adc_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);
    RCU_CFG0 |= (0b10 << 14) | (1 << 28);
    rcu_periph_clock_enable(RCU_ADC0);
    ADC_CTL1(ADC0) |= ADC_CTL1_ADCON;
}

void IO_init(void) {
    Inp_init(); // inport init
    Adc_init(); // A/D init
    Lcd_Init(); // LCD init
}

/*
 * Helper functions and data structure
 */
#define NODE_RADIUS 4
#define U16_MAX 65535
unsigned int timer = 0; // used to generate random u16
uint32_t diff = 300; // decide the delay in main while-loop
u16 score = 0; // final score

typedef struct {
    u16 x;
    u16 y;
} Location;

// As its name
Location generate_rand_location(u16 x_lower, u16 x_upper, u16 y_lower, u16 y_upper) {
    Location res;
    res.x = Generate_U16(x_lower, x_upper, timer);
    res.y = Generate_U16(y_lower, y_upper, timer);
    return res;
}

// Check if attacker's center is inside the victim area
// Here we calculate victim circle as a square
bool check_collision(Location attacker, Location victim) {
    // expand victim to a square area
    u16 up = victim.y - NODE_RADIUS;
    u16 down = victim.y + NODE_RADIUS;
    u16 left = victim.x - NODE_RADIUS;
    u16 right = victim.x + NODE_RADIUS;

    // check if attacker inside the victim area
    if ((attacker.y >= up && attacker.y <= down) && (attacker.x >= left && attacker.x <= right)) {
        return TRUE;
    } else return FALSE;
}

// Output u16 at right-up
void debug(u16 i) {
    LCD_Fill(140, 5, 120, 10, BLACK);
    LCD_ShowNum(140, 5, i, 1, WHITE);
}


/*
 * Snake Data structure and functions
*/
#define LEFT 0
#define DOWN 1
#define RIGHT 2
#define UP 3

struct SnakeNode {
    struct SnakeNode *prev;
    struct SnakeNode *next;
    int direction;
    Location loc;
    u16 color;
};
typedef struct SnakeNode SnakeNode;

typedef struct {
    u16 len;
    u16 velocity;
    SnakeNode *head;
    SnakeNode *tail;
} Snake;

// As its name
void draw_snake(Snake *s, u16 old_x, u16 old_y, u16 old_prev_x, u16 old_prev_y, u16 old_type) {
    // Clear Last TailNode
    LCD_DrawCircle(old_x, old_y, NODE_RADIUS, BLACK);
    if (old_type == 1) {
        LCD_DrawCircle(old_prev_x, old_prev_y, NODE_RADIUS, BLACK);

    }

    // Draw whole SnakeNode
    SnakeNode *temp = s->head;
    while (temp != NULL) {
        LCD_DrawCircle(temp->loc.x, temp->loc.y, NODE_RADIUS, temp->color);
        temp = temp->next;
    }
}

// As its name
bool update_loc(Snake *s) {
    // Check if eat itself
    SnakeNode *temp = s->head->next;
    while (temp != NULL) {
        if (check_collision(s->head->loc, temp->loc)) return TRUE;
        temp = temp->next;
    }

    // Update all SnakeNodes' loc by direction
    temp = s->head;
    int i = 0;
    while (temp != NULL) {
        if (temp->direction == LEFT) {
            temp->loc.x -= s->velocity;
        } else if (temp->direction == RIGHT) {
            temp->loc.x += s->velocity;
        } else if (temp->direction == UP) {
            temp->loc.y -= s->velocity;
        } else if (temp->direction == DOWN) {
            temp->loc.y += s->velocity;
        }

        // Check if meet Wall
        if (temp->loc.x <= NODE_RADIUS || temp->loc.x >= LCD_W - NODE_RADIUS ||
            temp->loc.y <= NODE_RADIUS || temp->loc.y >= LCD_H - NODE_RADIUS) {
            return TRUE;
        }

        temp = temp->next;
        i++;
    }

    return FALSE;
}

// As its name
Snake *init_snake() {
    SnakeNode *head = (SnakeNode *) malloc(sizeof(SnakeNode));
    SnakeNode *tail = (SnakeNode *) malloc(sizeof(SnakeNode));
    Snake *s = (Snake *) malloc(sizeof(Snake));
    s->head = head;
    s->tail = tail;
    s->len = 2;
    s->velocity = 2 * NODE_RADIUS;
    head->next = tail;
    tail->prev = head;
    head->prev = NULL;
    tail->next = NULL;
    head->color = GREEN;
    tail->color = WHITE;
    head->direction = RIGHT;
    tail->direction = RIGHT;
    head->loc = generate_rand_location(25, LCD_W - 25, 10, LCD_H - 10);
    tail->loc.x = head->loc.x - 2 * NODE_RADIUS;
    tail->loc.y = head->loc.y;

    return s;
}

// As its name
void reset_snake(Snake *s) {
    SnakeNode *temp = s->head->next;
    while (temp != s->tail) {
        temp = temp->next;
        free(temp->prev);
    }
    s->velocity = 2 * NODE_RADIUS;
    s->len = 2;
    s->head->next = s->tail;
    s->head->prev = NULL;
    s->tail->prev = s->head;
    s->tail->next = NULL;
    s->head->color = GREEN;
    s->tail->color = WHITE;
    s->head->direction = RIGHT;
    s->tail->direction = RIGHT;
    s->head->loc = generate_rand_location(25, LCD_W - 25, 10, LCD_H - 10);
    s->tail->loc.x = s->head->loc.x - 2 * NODE_RADIUS;
    s->tail->loc.y = s->head->loc.y;
    LCD_Clear(BLACK);
}

// As its name
void delete_snake(Snake *s) {
    reset_snake(s);
    free(s->head);
    free(s->tail);
    free(s);
}

// As its name
void add_new_node(Snake *s) {
    SnakeNode *new_node = (SnakeNode *) malloc(sizeof(SnakeNode));
    s->tail->next = new_node;
    new_node->prev = s->tail;
    new_node->next = NULL;

    // Decide new_node's direction by tail's direction
    if (s->tail->direction == UP) {
        new_node->loc.x = s->tail->loc.x;
        new_node->loc.y = s->tail->loc.y + 2 * NODE_RADIUS;
    } else if (s->tail->direction == DOWN) {
        new_node->loc.x = s->tail->loc.x;
        new_node->loc.y = s->tail->loc.y - 2 * NODE_RADIUS;
    } else if (s->tail->direction == LEFT) {
        new_node->loc.x = s->tail->loc.x + 2 * NODE_RADIUS;
        new_node->loc.y = s->tail->loc.y;
    } else if (s->tail->direction == RIGHT) {
        new_node->loc.x = s->tail->loc.x - 2 * NODE_RADIUS;
        new_node->loc.y = s->tail->loc.y;
    }

    new_node->direction = s->tail->direction;
    new_node->color = s->tail->color;

    s->tail = new_node;
    s->len++;
}

// As its name
void delete_tail_node(Snake *s) {
    s->tail = s->tail->prev;
    free(s->tail->next);
    s->tail->next = NULL;
    s->len--;
}

// Update evert SnakeNode's direction
// Each depends on the prevNode's direction
void update_direction(Snake *s) {
    SnakeNode *temp = s->tail;
    while (temp != s->head) {
        temp->direction = temp->prev->direction;
        temp = temp->prev;
    }
}

/*
 * Food data structure and functions
 */
#define LENGTHEN 0
#define SHORTEN 1
#define SPEEDUP 2

typedef struct {
    u16 type;
    u16 color;
    Location loc;
} Food;

// As its name
void update_food(Food *f, Snake *s) {
    timer = (timer + 1) % U16_MAX;
    while (TRUE) {
        // new food location
        f->loc = generate_rand_location(10, LCD_W - 10, 10, LCD_H - 10);

        // new food type 50%-Lengthen 25%-Shorten 25%-SpeedUp
        u16 type_choice = Generate_U16(0, 100, timer);
        if (type_choice <= 50) f->type = 0;
        else if (type_choice <= 75) f->type = 1;
        else if (type_choice <= 100) f->type = 2;
        // if snake's length == 2(only head and tail), next food type be lengthen
        if (s->len == 2) f->type = 0;

        // new food color
        if (f->type == 0) f->color = WHITE;
        else if (f->type == 1) f->color = RED;
        else if (f->type == 2) f->color = GBLUE;

        // new food loc should be different from SnakeNodes' locs
        SnakeNode *temp = s->head;
        bool collision = FALSE;
        while (temp != NULL) {
            if (check_collision(temp->loc, f->loc)) {
                collision = TRUE;
                break;
            }
            temp = temp->next;
        }
        if (collision == FALSE) return;
        timer = (timer + 1) % U16_MAX;
    }
}

// As its name
Food *init_food(Snake *s) {
    Food *f = (Food *) malloc(sizeof(Food));
    update_food(f, s);
    return f;
}

// As its name
void draw_food(Food *f) {
    LCD_DrawCircle(f->loc.x, f->loc.y, NODE_RADIUS, f->color);
}

// As its name, implementation is similar to check_collision()
void check_if_eat(Food *f, Snake *s) {
    u16 food_up = f->loc.y - (NODE_RADIUS + 2);
    u16 food_down = f->loc.y + (NODE_RADIUS + 2);
    u16 food_left = f->loc.x - (NODE_RADIUS + 2);
    u16 food_right = f->loc.x + (NODE_RADIUS + 2);

    if ((s->head->loc.y >= food_up && s->head->loc.y <= food_down) &&
        (s->head->loc.x <= food_right && s->head->loc.x >= food_left)) {
        // Clear Last Food
        LCD_DrawCircle(f->loc.x, f->loc.y, NODE_RADIUS, BLACK);

        // food effect
        if (f->type == LENGTHEN) add_new_node(s);
        else if (f->type == SHORTEN) delete_tail_node(s);
        else if (f->type == SPEEDUP) diff = (diff >= 40) ? (diff - 20) : diff;

        // update food and score
        update_food(f, s);
        score += 1;
    }
}

/*
 * UI functions
 */
void help_menu() {
//    LCD_ShowString(0, 0, (u8 *) ("Press 0 to go left"), WHITE);
//    LCD_ShowString(0, 16, (u8 *) ("Press 1 to go right"), WHITE);
//
//    LCD_ShowString(0, 32, (u8 *) ("Eat    to lengthen"), GREEN);
//    LCD_DrawCircle(37, 38, 7, WHITE);
//
//    LCD_ShowString(0, 48, (u8 *) ("Eat    to shorten"), GREEN);
//    LCD_DrawCircle(37, 54, 7, RED);
//
//    LCD_ShowString(0, 64, (u8 *) ("Eat    to turn back"), GREEN);
//    LCD_DrawCircle(37, 70, 7, GBLUE);
    HelpMenu_init();
    while (TRUE) {
        delay_1ms(150);
        timer = (timer + 150) % U16_MAX;

        if (Get_Button(0) || Get_Button(1)) {
            LCD_Clear(BLACK);
            Menu_init();
            break;
        }
    }
}

void start_menu() {
//    LCD_ShowString(40, 20, (u8 *) ("Retro Snake"), GREEN);
//    LCD_ShowString(40, 50, (u8 *) ("Play"), RED);
//    LCD_ShowString(90, 50, (u8 *) ("Help"), YELLOW);
//    LCD_DrawRectangle(35, 45, 75, 70, WHITE);
    Menu_init();
    int current_choice = 0;
    int i = 10;
    int j = 10;
    while (TRUE) {
        if (current_choice == 0 && Get_Button(0)) {
            break;
        } else if (current_choice == 0 && Get_Button(1)) {
            LCD_DrawRectangle(35, 45, 75, 70, BLACK);
            LCD_DrawRectangle(85, 45, 125, 70, WHITE);
            current_choice = 1;
        } else if (current_choice == 1 && Get_Button(0)) {
            help_menu();
            current_choice = 0;
        } else if (current_choice == 1 && Get_Button(1)) {
            LCD_DrawRectangle(85, 45, 125, 70, BLACK);
            LCD_DrawRectangle(35, 45, 75, 70, WHITE);
            current_choice = 0;
        }

        if ( (i+j)%3 == 0 ){
            LCD_DrawCircle(j, i, 5, RED);
        }else if ( (i+j)%3 == 1 ){
            LCD_DrawCircle(j, i, 5, GBLUE);
        }else if ( (i+j)%3 == 2 ){
            LCD_DrawCircle(j, i, 5, WHITE);
        }


        if(i == 10 && j < 150){
            j += 10;
        }else if(i < 80 && j == 150 ){
            i+= 10;
        }else if(i == 80 && j > 10){
            j -= 10;
        }else if(i > 10 && j == 10){
            i -= 10;
        }
        LCD_DrawCircle(j, i, 5, BLACK);
        delay_1ms(150);
        timer = (timer + 150) % U16_MAX;
    }
}

int end_menu() {
    LCD_Clear(BLACK);
    LCD_ShowString(40, 20, (u8 *) ("Score:"), GREEN);
    u8 num_len = 1;
    if (score >= 10) num_len = 2;
    LCD_ShowNum(110, 20, score, num_len, WHITE);
    EndMenu();

    int current_choice = 0;
    while (TRUE) {
        if (Get_Button(0)) {
            return current_choice;
        } else if (current_choice == 0 && Get_Button(1)) {
            LCD_DrawRectangle(35, 45, 80, 70, BLACK);
            LCD_DrawRectangle(90, 45, 130, 70, WHITE);
            current_choice = 1;
        } else if (current_choice == 1 && Get_Button(1)) {
            LCD_DrawRectangle(90, 45, 130, 70, BLACK);
            LCD_DrawRectangle(35, 45, 80, 70, WHITE);
            current_choice = 0;
        }
        delay_1ms(150);
        timer = (timer + 150) % U16_MAX;
    }


}


int main(void) {
    // init OLED
//    IO_init();
//    LCD_Clear(BLACK);
    OLED_init();

    // StartUI
//    start_menu();
//    LCD_Clear(BLACK);
    StartUI();
    delay_1ms(500);
    timer = (timer + 500) % U16_MAX;

    // Init GameUI
    Snake *s = init_snake();
    Food *f = init_food(s);
    draw_snake(s, 0, 0, 0, 0, 3);
    draw_food(f);

    // Main Game Process
    while (TRUE) {
        // Change head direction
        if (Get_Button(0)) {
            s->head->direction = (s->head->direction + 1) % 4;
        } else if (Get_Button(1)) {
            if (s->head->direction == LEFT) s->head->direction = UP;
            else s->head->direction = (s->head->direction - 1) % 4;
        }

        // Record old infos
        u16 old_tail_loc_x = s->tail->loc.x;
        u16 old_tail_loc_y = s->tail->loc.y;
        u16 old_food_type = f->type;
        u16 old_tail_prev_loc_x = s->tail->prev->loc.x;
        u16 old_tail_prev_loc_y = s->tail->prev->loc.y;

        // Update location
        if (update_loc(s)) {
            // Die, enter EndUI
            int choice = end_menu();

            if (choice == 0) {
                reset_snake(s); // Retry
                update_food(f, s);
                score = 0;
            } else break; // Quit
        }

        // Check if get food
        check_if_eat(f, s);

        // Draw snake and food
        draw_snake(s, old_tail_loc_x, old_tail_loc_y, old_tail_prev_loc_x, old_tail_prev_loc_y, old_food_type);
        draw_food(f);

        // Update direction
        update_direction(s);

        // This delay can be treat as difficulty of the game
        // Smaller delay make snake move faster, thus more difficult
        delay_1ms(diff);
        timer = (timer + diff) % U16_MAX;
    }

    // Game Over
    LCD_Clear(BLACK);
    delete_snake(s);
    free(f);

    return 0;
}
