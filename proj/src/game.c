#include "game.h"

extern int mouse_hookid, timer_hookid, kbd_hookid, rtc_hook_id;

extern uint8_t keyboard_scancode, keyboard_statuscode;
extern uint8_t mouse_scancode, mouse_statuscode;

extern int ih_error;
extern int timer_counter;

extern void *video_mem;
extern void *display_mem;

bool nextRound = false;
extern int frames_per_state;
extern int speed;

extern ShipBullet *shipBullets[MAX_SHIP_BULLETS];
extern AlienBullet *alienBullet;
Alien aliens[SIZE_OF_ALIENS];
int row = 0;
int killCount = 0;
int frame_counter = 0;
bool changeSpeed = false;
bool changeDir = false;
int roundNum = 0;

game_state gameState = MENU_DISPLAY;

void (initGame)(Ship *ship) {
    xpm_image_t background = loadBackground();
    if(gameState != NEXT_ROUND)
        createShip(512, SHIP_YPOS, 15);
    drawBackground(background);
    drawShip(ship);

    int xi = 24, yi = 20;
    int indice = 0;

    for (int column = 0; column < COL_ALIENS; column++) {
        for (int row = 0; row < ROW_ALIENS; row++) {
            if (row == 0) {
                aliens[indice] = createAlien(xi, yi, alien3, alien3_m, 30);
            } else if (row == 1) {
                aliens[indice] = createAlien(xi, yi, alien1, alien1_m, 20);
            } else {
                aliens[indice] = createAlien(xi, yi, alien2, alien2_m, 10);
            }
            yi += 60;
            indice++;
        }
        yi = 20;
        xi += 80;
    }
    
    gameState = PLAYING;
    killCount = 0;
    frames_per_state = 20;
    frame_counter = 0;
    changeSpeed = false;
    changeDir = false;

    speed = INITIAL_ALIEN_SPEED + (2 * roundNum);
}

int (game_loop)() {
    uint16_t mode = 0x105;
    int mouse_bit_no;
    uint8_t kbd_bit_no;
    uint8_t rtc_bit_no;
    uint8_t timer_bit_no;
    vbe_mode_info_t info;
    int ipc_status, r;
    message msg;
    struct packet pp;
    int currentByte = 1;
    uint8_t mouseBytes[3];
    int size = 1;
    uint8_t kbdBytes[2];

    Button button = INITIAL;
    bool instruction_button = false;
    KeyActivity key;
    int ipf = ((int) sys_hz()) / 60;
    bool mov_img = false, right_mov = true;
    frames_per_state = 20;

    Mouse *mouse = createMouse(50, 700);
    Ship *ship = createShip(512, SHIP_YPOS, 15);

    xpm_image_t background = loadBackground();
    xpm_image_t shipBullet_img = loadShipBulletXpm();
    xpm_image_t alienBullet_img = loadAlienBulletXpm();
    xpm_image_t initialScreen = loadInitialScreen();
    initMenuXpm();

    initShipBullets(shipBullets);
    initNumbers();
    initAlienBullet();
    alienBullet->active = false;


    ///SUBSCRIBE INTERRUPTS
    if (vg_get_mode_info(&mode, &info))
        return 1;

    if (vg_set_mode(&mode))
        return 1;

    if (mouse_subscribe_int(&mouse_bit_no))
        return 1;

    if (send_mouse_command(ENABLE_MOUSE)) {
        return 1;
    }

    if (kbd_subscribe_int(&kbd_bit_no))
        return 1;

    if (timer_subscribe_int(&timer_bit_no))
        return 1;

    if(rtc_subscribe_int(&rtc_bit_no))
        return 1;

    if(rtc_enable())
        return 1;

    drawBackground(initialScreen);
    displayScreen();

    sleep(4);

    if (drawMenu(button))
        return 1;

    while (gameState != EXIT) {
        displayScreen();
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    ///MOUSE INTERRUPT
                    if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
                        mouse_ih(); // read 1 byte per interrupt -> a packet has 3 bytes
                        if (ih_error) {
                            ih_error = 0;
                            continue;
                        }
                        if (currentByte == 1) {
                            // 1 byte do packet
                            if ((mouse_scancode & BIT(3)) == 0) {
                                continue; // if the bit(3) is different from 1 wait for the next interrupt
                            }
                            currentByte = 2;
                            mouseBytes[0] = mouse_scancode;
                        } else if (currentByte == 2) {
                            // 2 byte do packet
                            currentByte = 3;
                            mouseBytes[1] = mouse_scancode;
                        } else if (currentByte == 3) {
                            // 3 byte do packet
                            currentByte = 1;
                            mouseBytes[2] = mouse_scancode;
                            getMousePacket(&pp, mouseBytes);

                            ///====================MENU====================
                            updateMouse(&pp, mouse); // updates mouse coordinates and lb_pressed 
                            // variable according to the given packet
                            if (gameState == MENU_DISPLAY) {

                                button = getButton(mouse->x, mouse->y);
                                if (drawMenu(button)) {
                                    return 1;
                                }
                                drawMouse(mouse);
                                if (mouse->lb_pressed) {
                                    switch (button) {
                                        case START_BUTTON:
                                            ///start game
                                            initGame(ship);
                                            break;
                                        case INSTRUCTIONS_BUTTON:
                                            gameState = INSTRUCTIONS_DISPLAY;
                                            drawInstructions(false);
                                            break;
                                        case EXIT_BUTTON:
                                            gameState = EXIT;
                                            break;
                                        case INITIAL:
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            }

                            ///====================Instructions====================
                            else if (gameState == INSTRUCTIONS_DISPLAY) {
                                instruction_button = getInstructionButton(mouse->x, mouse->y);
                                if (drawInstructions(instruction_button)) {
                                    continue;
                                }
                                drawMouse(mouse);
                                if (mouse->lb_pressed) {
                                    if (instruction_button) {
                                        gameState = MENU_DISPLAY;
                                        button = INITIAL;
                                        drawMenu(button);
                                    }
                                }
                            }

                            ///====================SHIP SHOOT====================
                            else if (gameState == PLAYING) {

                                if (mouse->lb_pressed) {
                                    createShipBullet(ship->x + ship->img.width / 2, ship->y, SHIP_BULLET_SPEED,
                                                     shipBullet_img);
                                    mouse->lb_pressed = false;
                                }
                            }

                        }
                    }

                    ///KEYBOARD INTERRUPT
                    
                    if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
                        kbc_ih();
                        if (twoBytes(keyboard_scancode)) {
                            size = 2;
                            continue;
                        }
                        if (size == 1) {
                            kbdBytes[0] = keyboard_scancode;
                        } else {
                            kbdBytes[0] = 0xE0;
                            kbdBytes[1] = keyboard_scancode;
                        }
                        size = 1;

                        if(keyboard_scancode == ESC_BREAK){
                            gameState = EXIT;
                            continue;
                        }
                        if (gameState == PLAYING) {
                            ///====================SHIP MOVEMENT====================
                            switch (keyboard_scancode) {
                                case 0x1E:
                                    key = A_Pressed;
                                    break;
                                case 0x9E:
                                    key = A_Released;
                                    break;
                                case 0x20:
                                    key = D_Pressed;
                                    break;
                                case 0xAE:
                                    key = D_Released;
                                    break;
                                default:
                                    key = Invalid_key;
                                    break;
                            }
                            ship = updateShipPosition(ship, key);
                        }
                    }

                    ///RTC INTERRUPT
                    /*
                    Doesn't work
                    if (msg.m_notify.interrupts & BIT(rtc_bit_no)){
                        rtc_ih();
                        printf("%d\n", speed);
                    }
                    */

                    ///TIMER INTERRUPT
                    if (msg.m_notify.interrupts & BIT(timer_bit_no)) {

                        timer_int_handler();

                        if (gameState == PLAYING) {
                            updateShipBulletPosition();
                            drawBackground(background);
                            drawShip(ship);
                            drawShipBullets();
                            drawAlienBullet();
                            drawLives(ship);
                            drawScore(ship);

                            ///====================ALIEN BULLET====================
                            if (!alienBullet->active) {
                                int i = generateAlienBullet(aliens);
                                createAlienBullet(aliens[i].x + aliens[i].width / 2, aliens[i].y + aliens[i].height,
                                                  ALIEN_BULLET_SPEED, alienBullet_img);
                            } else {
                                updateAlienBulletPosition();
                                verifyShipAndBulletCollision(ship);

                                if(ship->lives == 0){
                                    ship = createShip(512, SHIP_YPOS, 15);
                                    speed = INITIAL_ALIEN_SPEED;
                                    roundNum = 0;
                                    gameState = MENU_DISPLAY;
                                    break;
                                }
                            }

                            ///====================ALIEN MOVEMENT====================
                            if (timer_counter >= ipf) {
                                frame_counter++;
                                changeDir = false;
                                for (int i = 0; i < SIZE_OF_ALIENS; i++) {
                                    Alien *a = &aliens[i];
                                    if (!(a->alive)) {
                                        continue;
                                    }
                                    if (right_mov) {
                                        change_alien_x_coordinates(a, speed);
                                        drawAlien(a, mov_img);
                                        if ((a->x + a->width) >= X_RIGHT_BORDER) {
                                            changeDir = true;
                                        }
                                    } else {
                                        change_alien_x_coordinates(a, -speed);
                                        drawAlien(a, mov_img);
                                        if (a->x <= X_LEFT_BORDER) {
                                            changeDir = true;
                                        }
                                    }

                                    verifyAlienAndBulletCollision(a, &killCount, ship);

                                    if ((a->y + a->height) >= TERRITORY) {
                                        ship = createShip(512, SHIP_YPOS, 15);
                                        speed = INITIAL_ALIEN_SPEED;
                                        roundNum = 0;
                                        gameState = MENU_DISPLAY;
                                    } else if (killCount == SIZE_OF_ALIENS) {
                                        gameState = NEXT_ROUND;
                                        roundNum++;
                                    }
                                }

                                if(changeDir) {
                                    change_all_y(aliens, 20, SIZE_OF_ALIENS);
                                    right_mov = !right_mov;
                                    changeDir = false;
                                }

                                row++;
                                timer_counter = 0;
                                if (frame_counter >= frames_per_state) {
                                    mov_img = !mov_img;
                                    frame_counter = 0;
                                }
                            }
                        }

                        ///====================NEW ROUND====================
                        else if(gameState == NEXT_ROUND){
                            initGame(ship);
                            gameState = PLAYING;
                            speed = INITIAL_ALIEN_SPEED + roundNum;
                            nextRound = true;
                        }

                        else if(gameState == MENU_DISPLAY){
                            button = getButton(mouse->x, mouse->y);
                            if (drawMenu(button)) {
                                return 1;
                            }
                            drawMouse(mouse);
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    }

    removeMouse(mouse);
    removeShip(ship);
    removeAlienBullet();

    if(rtc_disable())
        return 1;

    if(rtc_unsubscribe_int())
        return 1;

    if (timer_unsubscribe_int())
        return 1;

    if (send_mouse_command(DISABLE_MOUSE))
        return 1;

    if (mouse_unsubscribe_int())
        return 1;

    if (kbd_unsubscribe_int())
        return 1;

    if (vg_exit())
        return 1;
    return 0;
}

