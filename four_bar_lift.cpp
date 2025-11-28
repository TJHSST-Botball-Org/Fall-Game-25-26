#include <kipr/kipr.h>
#include <iostream>

bool is_arm_down = false;

// PORTS
const int ARM_PORT = 1;
const int CLAW_PORT = 0;
const int LEFT_MOTOR_PORT = 3;
const int RIGHT_MOTOR_PORT = 0;
const int LEFT_TOPHAT_PORT = 2;
const int RIGHT_TOPHAT_PORT = 0;
const int LEFT_TOPHAT_THRESHOLD = 3900;
const int RIGHT_TOPHAT_THRESHOLD = 3900;

// POSITIONS
const int RAISED_POSITION_UPPER = 930; // FIX THE NUMBER
const int RAISED_POSITION_LOWER = 270;
const int FULLY_RAISED = 300;
const int HALF_LOWERED = 1770;
const int LOWERED_POSITION = 0; // FIX THE NUMBER
const int CLOSED_POSITION = 676;   // FIX THE NUMBER
const int OPEN_POSITION = 1300;    // FIX THE NUMBER

// WHEEL ADJUSTMENTS

// Making it go straight:
const float FORWARD_RIGHT_WHEEL_ADJUSTMENT = 1.032; // Veering to left? Pull right side back, decrease value
                                                    // Veering to right? Push right side forward, increase value

const float ARM_DOWN_FORWARD_RIGHT_WHEEL_ADJUSTMENT = 1.01;

const float BACKWARD_RIGHT_WHEEL_ADJUSTMENT = 0.9735; // Veering to left? Pull right side back, decrease value
                                                      // Veering to right? Push right side forward, increase value

const float ARM_DOWN_BACKWARD_RIGHT_WHEEL_ADJUSTMENT = 0.9635;

// Making it go the right distance
const float FORWARD_DISTANCE_ADJUSTMENT = 0.114773456; // Make robot move 5 inches.
                                                       // If not enough distance, increase this value
                                                       // If too much, decrease this value

const float BACKWARD_DISTANCE_ADJUSTMENT = 0.125391818; // Make robot move 5 inches.
                                                        // If not enough distance, increase this value
                                                        // If too much, decrease this value

// Making it go the right distance when the claw is down
const float ARM_DOWN_FORWARD_DISTANCE_ADJUSTMENT = 0.119992171; // Make robot move 5 inches.
                                                                // If not enough distance, increase this value
                                                                // If too much, decrease this value

const float ARM_DOWN_BACKWARD_DISTANCE_ADJUSTMENT = 0.119992171; // Make robot move 5 inches.
                                                                 // If not enough distance, increase this value
                                                                 // If too much, decrease this value

// Making it turn the right amount
// Make the robot turn 90 degrees. Adjust values accordingly
const float CLOCKWISE_TURNING_ADJUSTMENT = 0.64; // Turn too much --> decrease value
                                                 // Turn too little --> decrease value

const float COUNTER_CLOCKWISE_TURNING_ADJUSTMENT = 0.65; // Turn too much --> decrease value
                                                         // Turn too little --> decrease value

// Making it turn the right amount
// Make the robot turn 90 degrees. Adjust values accordingly
const float ARM_DOWN_CLOCKWISE_TURNING_ADJUSTMENT = 0.63; // Turn too much --> decrease value
                                                          // Turn too little --> decrease value

const float ARM_DOWN_COUNTER_CLOCKWISE_TURNING_ADJUSTMENT = 0.665; // Turn too much --> decrease value
                                                                   // Turn too little --> decrease value

const double PI = 3.141592654;

void slowly_set_servo_position(int pin, int position, int wait_delay_ms = 10)
{
    int initial_pos = get_servo_position(pin);

    while (initial_pos > position ? get_servo_position(pin) > position : get_servo_position(pin) < position)
    {
        set_servo_position(pin, get_servo_position(pin) + (initial_pos > position ? -10 : 10));
        msleep(wait_delay_ms);
    }
}

void raise_arm()
{
    is_arm_down = false;
    slowly_set_servo_position(ARM_PORT, RAISED_POSITION);
}

void fully_raise_arm()
{
    is_arm_down = false;
    slowly_set_servo_position(ARM_PORT, FULLY_RAISED);
}

void half_lower_arm()
{
    is_arm_down = false;
    slowly_set_servo_position(ARM_PORT, HALF_LOWERED);
}

void lower_arm()
{
    is_arm_down = true;
    slowly_set_servo_position(ARM_PORT, LOWERED_POSITION);
}

void close_claw()
{
    set_servo_position(CLAW_PORT, CLOSED_POSITION);
    msleep(1000);
}

void open_claw()
{
    set_servo_position(CLAW_PORT, OPEN_POSITION);
    msleep(1000);
}

void p(std::string output)
{
    std::cout << output << std::endl;
}

void wait_for_button()
{
    std::cout << "Waiting for button." << std::endl;
    while (analog(5) > 1000)
    {
        msleep(1);
    }
}

void stop()
{
    freeze(LEFT_MOTOR_PORT);
    freeze(RIGHT_MOTOR_PORT);
    msleep(100);
}

void move_linear(float distance_in_inches, float speed_in_inches_per_sec)
{
    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    float adj;

    int direction = distance_in_inches < 0 ? -1 : 1;
    if (direction == 1)
    {
        if (!is_arm_down)
        {
            adj = FORWARD_RIGHT_WHEEL_ADJUSTMENT;
        }
        else
        {
            adj = ARM_DOWN_FORWARD_RIGHT_WHEEL_ADJUSTMENT;
        }
    }
    else
    {
        if (!is_arm_down)
        {
            adj = BACKWARD_RIGHT_WHEEL_ADJUSTMENT;
        }
        else
        {
            adj = ARM_DOWN_BACKWARD_RIGHT_WHEEL_ADJUSTMENT;
        }
    }

    move_at_velocity(
        LEFT_MOTOR_PORT,
        1300 * (speed_in_inches_per_sec / 5) * direction);

    move_at_velocity(
        RIGHT_MOTOR_PORT,
        1300 * (speed_in_inches_per_sec / 5) * adj * direction);

    if (direction == 1)
        msleep(1300 * abs(distance_in_inches) * (is_arm_down ? ARM_DOWN_FORWARD_DISTANCE_ADJUSTMENT : FORWARD_DISTANCE_ADJUSTMENT));
    else
        msleep(1300 * abs(distance_in_inches) * (is_arm_down ? ARM_DOWN_BACKWARD_DISTANCE_ADJUSTMENT : BACKWARD_DISTANCE_ADJUSTMENT));

    stop();
}

void move_forward_until_black_line()
{
    /* Moves forward until rear tophats align with black tape.
    Not 100% accurate, but probably better than nothing. */

    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    move_at_velocity(
        LEFT_MOTOR_PORT,
        1300);

    move_at_velocity(
        RIGHT_MOTOR_PORT,
        1300 * (is_arm_down ? ARM_DOWN_FORWARD_RIGHT_WHEEL_ADJUSTMENT : FORWARD_RIGHT_WHEEL_ADJUSTMENT));

    while (!(analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD && analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD))
    {
        // While not BOTH on black line

        if (analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD && !(analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD))
        {
            p("Right white");

            // Only left side on the black line. Move only the right side forward.
            freeze(LEFT_MOTOR_PORT);

            move_at_velocity(
                RIGHT_MOTOR_PORT,
                1300 * (is_arm_down ? ARM_DOWN_FORWARD_RIGHT_WHEEL_ADJUSTMENT : FORWARD_RIGHT_WHEEL_ADJUSTMENT));
        }
        else if (!(analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD) && (analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD))
        {
            p("Left white");

            // Only right side on the black line. Move only the left side forward.
            move_at_velocity(
                LEFT_MOTOR_PORT,
                1300);

            freeze(RIGHT_TOPHAT_PORT);
        }
        else
        {
            /* Neither on the black line */
            p("Both white");
            move_at_velocity(
                LEFT_MOTOR_PORT,
                1300);

            move_at_velocity(
                RIGHT_MOTOR_PORT,
                1300 * (is_arm_down ? ARM_DOWN_FORWARD_RIGHT_WHEEL_ADJUSTMENT : FORWARD_RIGHT_WHEEL_ADJUSTMENT));
        }

        msleep(200);
    }

    stop();
}

void move_backwards_until_black_line()
{
    /* Moves backwards until rear tophats align with black tape.
    Not 100% accurate, but probably better than nothing. */

    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    move_at_velocity(
        LEFT_MOTOR_PORT,
        -1300);

    move_at_velocity(
        RIGHT_MOTOR_PORT,
        -1300 * (is_arm_down ? ARM_DOWN_FORWARD_RIGHT_WHEEL_ADJUSTMENT : FORWARD_RIGHT_WHEEL_ADJUSTMENT));

    while (!(analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD && analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD))
    {
        // While not BOTH on black line

        if (analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD && !(analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD))
        {
            p("Right white");

            // Only left side on the black line. Move only the right side forward.
            freeze(LEFT_MOTOR_PORT);

            move_at_velocity(
                RIGHT_MOTOR_PORT,
                -1300 * (is_arm_down ? ARM_DOWN_FORWARD_RIGHT_WHEEL_ADJUSTMENT : FORWARD_RIGHT_WHEEL_ADJUSTMENT));
        }
        else if (!(analog(LEFT_TOPHAT_PORT) > LEFT_TOPHAT_THRESHOLD) && (analog(RIGHT_TOPHAT_PORT) > RIGHT_TOPHAT_THRESHOLD))
        {
            p("Left white");

            // Only right side on the black line. Move only the left side forward.
            move_at_velocity(
                LEFT_MOTOR_PORT,
                -1300);

            freeze(RIGHT_TOPHAT_PORT);
        }
        else
        {
            /* Neither on the black line */
            p("Both white");
            move_at_velocity(
                LEFT_MOTOR_PORT,
                -1300);

            move_at_velocity(
                RIGHT_MOTOR_PORT,
                -1300 * (is_arm_down ? ARM_DOWN_FORWARD_RIGHT_WHEEL_ADJUSTMENT : FORWARD_RIGHT_WHEEL_ADJUSTMENT));
        }

        msleep(200);
    }

    stop();
}

void turn(float direction, float speed_in_inches_per_sec, float degrees)
{
    /* Direction is 1 or -1, 1 for clockwise, -1 for counter-c */

    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);

    move_at_velocity(
        LEFT_MOTOR_PORT,
        1300 * direction * (speed_in_inches_per_sec / 5));

    move_at_velocity(
        RIGHT_MOTOR_PORT,
        1300 * FORWARD_RIGHT_WHEEL_ADJUSTMENT * direction * -1 * (speed_in_inches_per_sec / 5));

    if (direction == 1)
        msleep(13.25 * degrees * (is_arm_down ? ARM_DOWN_CLOCKWISE_TURNING_ADJUSTMENT : CLOCKWISE_TURNING_ADJUSTMENT));
    else
        msleep(13.25 * degrees * (is_arm_down ? ARM_DOWN_COUNTER_CLOCKWISE_TURNING_ADJUSTMENT : COUNTER_CLOCKWISE_TURNING_ADJUSTMENT));

    stop();
}

int main()
{
    /* PSUEDOCODE FOR FOUR BAR LIFT: values and robot design aren't done yet
    
    pick up object with claw()
    turn(C-clockwise 180)
    move(foward till the colored cubes)
    turn(clockwise 90)
    move(forward till the raised platform)
    turn(counter-clockwise 90)

    */
    turn(1, 5, 90);
    half_lower_arm();
    close_claw();
    raise_arm();
    turn(1, 5, 180);
    lower_arm();
    open_claw();
    close_claw();
    raise_arm();
    turn(1, 5, 90);
    move_linear(10, 5);
    turn(-1, 5, 180);
    move_linear(5, 5);
    open_claw();
    lower_arm();
    close_claw();
    raise_arm();





    return 0;
}
