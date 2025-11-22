#include <kipr/wombat.h>
#include <iostream>
#include <fstream>

const int FRONT_LEFT_PIN = 0;
const int FRONT_RIGHT_PIN = 3;
const int BACK_LEFT_PIN = 1;
const int BACK_RIGHT_PIN = 2;

const float GLOBAL_MULTIPLIER = 1.5;
const float FRONT_LEFT_MULTIPLIER = 0.95;
const float FRONT_RIGHT_MULTIPLIER = 1;
const float BACK_LEFT_MULTIPLIER = 0.95;
const float BACK_RIGHT_MULTIPLIER = 0.912;

const int PVC_ARM = 1;
const int PVC_WRIST = 2;

const int PVC_ARM_RAISE_VALUE = 1540;
const int PVC_ARM_LOWER_VALUE = 413; // TENTATIVE, WE DONT HAVE WHEELS YET SO THIS COULD CHANGE
const int PVC_ARM_STOW_VALUE = 413;  // TENTATIVE, WE DONT HAVE WHEELS YET SO THIS COULD CHANGE
const int PVC_WRIST_DROP_VALUE = 1409;
const int PVC_WRIST_PICKUP_VALUE = 305;

const int ENTREE_TOUCH_SENSOR = 0;

const int TOPHAT_FRONT_LEFT = 5;
const int TOPHAT_FRONT_RIGHT = 4;
const int TOPHAT_BACK_LEFT = 2;
const int TOPHAT_BACK_RIGHT = 3;

const int TOPHAT_FRONT_LEFT_THRESHOLD = 3900;
const int TOPHAT_FRONT_RIGHT_THRESHOLD = 3900;
const int TOPHAT_BACK_LEFT_THRESHOLD = 3900;
const int TOPHAT_BACK_RIGHT_THRESHOLD = 3900;

const int LEFT_NINETY_DEGREE_WAIT_MS = 1600;  // last 1750
const int RIGHT_NINETY_DEGREE_WAIT_MS = 1590; // last 1750

const int MOVE_FORWARD_FOR_DISTANCE_CONSTANT = 173;        // Adjust based on testing
const int MOVE_LEFT_FOR_DISTANCE_CONSTANT = 183.41463394;  // Adjust based on testing
const int MOVE_RIGHT_FOR_DISTANCE_CONSTANT = 183.41463394; // Adjust based on testing

void stop()
{
    freeze(FRONT_LEFT_PIN);
    freeze(FRONT_RIGHT_PIN);
    freeze(BACK_LEFT_PIN);
    freeze(BACK_RIGHT_PIN);
    msleep(100);
}

void move_forward()
{
    move_at_velocity(FRONT_LEFT_PIN, 750 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, 750 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, 750 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, 750 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void move_backward()
{
    move_at_velocity(FRONT_LEFT_PIN, -750 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, -750 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, -750 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, -750 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void move_left()
{
    move_at_velocity(FRONT_LEFT_PIN, -750 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, 750 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, 750 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, -750 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void move_right()
{
    move_at_velocity(FRONT_LEFT_PIN, 750 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, -750 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, -750 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, 750 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void move_diagonal_forward_left()
{
    move_at_velocity(FRONT_LEFT_PIN, 0 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, 750 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, 750 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, 0 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void move_diagonal_forward_right()
{
    move_at_velocity(FRONT_LEFT_PIN, 750 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, 0 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, 0 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, 750 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void move_diagonal_backward_left()
{
    move_at_velocity(FRONT_LEFT_PIN, -750 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, 0 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, 0 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, -750 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void move_diagonal_backward_right()
{
    move_at_velocity(FRONT_LEFT_PIN, 0 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, -750 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, -750 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, 0 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void turn_clockwise_continuous()
{
    move_at_velocity(FRONT_LEFT_PIN, 750 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, -750 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, 750 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, -750 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void turn_counter_clockwise_continuous()
{
    move_at_velocity(FRONT_LEFT_PIN, -750 * FRONT_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(FRONT_RIGHT_PIN, 750 * FRONT_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_LEFT_PIN, -750 * BACK_LEFT_MULTIPLIER * GLOBAL_MULTIPLIER);
    move_at_velocity(BACK_RIGHT_PIN, 750 * BACK_RIGHT_MULTIPLIER * GLOBAL_MULTIPLIER);
}

void turn_left_90_deg()
{
    turn_counter_clockwise_continuous();
    msleep(LEFT_NINETY_DEGREE_WAIT_MS);
    stop();
}

void turn_right_90_deg()
{
    turn_clockwise_continuous();
    msleep(RIGHT_NINETY_DEGREE_WAIT_MS);
    stop();
}

void move_forward_for_distance(float distance)
{
    move_forward();
    msleep(MOVE_FORWARD_FOR_DISTANCE_CONSTANT * distance); // scale linearly by distance
    stop();
}

void move_backward_for_distance(float distance)
{
    move_backward();
    msleep(MOVE_FORWARD_FOR_DISTANCE_CONSTANT * distance); // scale linearly by distance
    stop();
}

void move_left_for_distance(float distance)
{
    move_left();
    msleep(MOVE_LEFT_FOR_DISTANCE_CONSTANT * distance);
    stop();
}

void move_right_for_distance(float distance)
{
    move_right();
    msleep(MOVE_RIGHT_FOR_DISTANCE_CONSTANT * distance);
    stop();
}

void slowly_set_servo_position(int pin, int position, int wait_delay_ms = 10)
{
    int initial_pos = get_servo_position(pin);

    while (initial_pos > position ? get_servo_position(pin) > position : get_servo_position(pin) < position)
    {
        set_servo_position(pin, get_servo_position(pin) + (initial_pos > position ? -10 : 10));
        msleep(wait_delay_ms);
    }
}

void turn_wrist_drop()
{
    enable_servo(PVC_WRIST);
    slowly_set_servo_position(PVC_WRIST, PVC_WRIST_DROP_VALUE);
    disable_servo(PVC_WRIST);
}

void turn_wrist_pickup()
{
    enable_servo(PVC_WRIST);
    slowly_set_servo_position(PVC_WRIST, PVC_WRIST_PICKUP_VALUE);
    disable_servo(PVC_WRIST);
}

void set_arm_up()
{
    enable_servo(PVC_ARM);
    slowly_set_servo_position(PVC_ARM, PVC_ARM_RAISE_VALUE);
    disable_servo(PVC_ARM);
}

void set_arm_down()
{
    enable_servo(PVC_ARM);
    slowly_set_servo_position(PVC_ARM, PVC_ARM_LOWER_VALUE);
    disable_servo(PVC_ARM);
}

bool is_fl_tophat_black()
{
    return analog(TOPHAT_FRONT_LEFT) > TOPHAT_FRONT_LEFT_THRESHOLD;
}

bool is_fr_tophat_black()
{
    return analog(TOPHAT_FRONT_RIGHT) > TOPHAT_FRONT_RIGHT_THRESHOLD;
}

bool is_bl_tophat_black()
{
    return analog(TOPHAT_BACK_LEFT) > TOPHAT_BACK_LEFT_THRESHOLD;
}

bool is_br_tophat_black()
{
    return analog(TOPHAT_BACK_RIGHT) > TOPHAT_BACK_RIGHT_THRESHOLD;
}

void line_up_with_black_line_front()
{
    move_forward();

    while (!(is_fl_tophat_black() and is_fr_tophat_black()))
    {
        if (is_fl_tophat_black() and !is_fr_tophat_black())
        {
            turn_counter_clockwise_continuous();
        }
        else if (!is_fl_tophat_black() and is_fr_tophat_black())
        {
            turn_clockwise_continuous();
        }
        else if (!is_fl_tophat_black() and !is_fr_tophat_black())
        {
            move_forward();
        }
    }

    stop();
}

void line_up_with_black_line_behind()
{
    move_backward();

    while (!(is_bl_tophat_black() and is_br_tophat_black()))
    {
        if (is_bl_tophat_black() and !is_br_tophat_black())
        {
            turn_clockwise_continuous();
        }
        else if (!is_bl_tophat_black() and is_br_tophat_black())
        {
            turn_counter_clockwise_continuous();
        }
        else if (!is_bl_tophat_black() and !is_br_tophat_black())
        {
            move_backward();
        }
    }

    stop();
}

int main()
{
    open_entree_claw();
    close_bottle_claw();
    stow_bottle_arm();
    move_forward_for_distance(29);
    close_entree_claw();
    move_backward_for_distance(16.5);
    move_right_for_distance(39);

    // Move to the right, but make sure the back tophats don't touch the black line

    turn_right_90_deg();
    turn_right_90_deg();
    move_forward_for_distance(9);
    release_entrees();
    close_entree_claw();
    open_bottle_claw();
    lower_bottle_arm();
    move_backward_for_distance(8);
    close_bottle_claw();
    raise_bottle_arm();
    turn_right_90_deg();
    turn_right_90_deg();
    move_left_for_distance(14);
    move_backward_for_distance(4);
    lower_bottle_arm();
    open_bottle_claw();
}

/*

open_entree_claw();
    move_forward_for_distance(6.4);
    close_entree_claw();
    move_backward_for_distance(3.9);
    move_right_for_distance(5.8);
    turn_right_90_deg();
    turn_right_90_deg();
    move_forward_for_distance(1);
*/