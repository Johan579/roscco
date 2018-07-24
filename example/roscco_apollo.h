#include <ros/ros.h>
#include <roscco/BrakeCommand.h>
#include <roscco/SteeringCommand.h>
#include <roscco/ThrottleCommand.h>
#include <roscco/CanFrame.h>
#include <modules/canbus/proto/chassis.pb.h>
#include <modules/control/proto/control_cmd.pb.h>
#include <modules/localization/proto/localization.pb.h>
#include <roscco/pid_control.h>
#include <string>
#include <math.h>

#define THROTTLE_RATIO 0.393
#define EV_BRAKE_RATIO 0.118
#define PETROL_BRAKE_RATIO 0.056
#define STEERING_RATIO 0.018
#define EV_SPEED_RATIO 0.003
#define PETROL_SPEED_RATIO 0.02


class RosccoApollo
{
public:

    /**
     * @brief RosccoApollo class initializer
     *
     * This function construct a class which subscribes to Apollo messages and publishes ROSCCO messages
     */
    RosccoApollo();

private:

    /**
     * @brief Callback function to pipe Apollo SteeringCommand to ROSCCO
     *
     * @param apollo control command message to be consumed
     */
    void steeringCallback( const apollo::control::ControlCommand& input ); 
    
    /**
     * @brief Callback function to pipe Apollo BrakeCommand to ROSCCO
     *
     * @param apollo control command message to be consumed
     */
    void brakeCallback( const apollo::control::ControlCommand& input );
    
    /**
     * @brief Callback function to pipe Apollo ThrottleCommand to ROSCCO
     *
     * @param apollo control command message to be consumed
     */
    void throttleCallback( const apollo::control::ControlCommand& input );
    
    /**
     * @brief Soul EV Callback function to save and publish chassis status
     *
     * @param roscco can frame message to be consumed
     */
    void EVCanFrameCallback( const roscco::CanFrame& input );
    
    /**
     * @brief Callback function to log localization
     *
     * @param apollo localization message to be consumed
     */
    void localizationCallback( const apollo::localization::LocalizationEstimate& input );

    ros::NodeHandle nh;

    ros::Publisher throttle_pub;
    ros::Publisher brake_pub;
    ros::Publisher steering_pub;
    ros::Publisher chassis_pub;
    ros::Subscriber throttle_sub;
    ros::Subscriber brake_sub;
    ros::Subscriber steering_sub;
    ros::Subscriber can_frame_sub;
    ros::Subscriber localization_sub;

    double steering_angle_report = 0;
    double throttle_report = 0;
    double brake_report = 0;
    double speed_report = 0;
    double pre_speed_report = 0;
};


/**
 * @brief setup a pid to control the steering angle
 * 
 * @param setpoint/target
 * @param command
 * @param steering angle position
 */
void closedLoopControl( double setpoint,
                        roscco::SteeringCommand& output,
                        double steering_angle_report );
