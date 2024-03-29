#ifndef BAXTERPOSECONTROLSERVER_H
#define BAXTERPOSECONTROLSERVER_H

#include <dq_robotics/DQoperations.h>
#include <dq_robotics/baxter_dq.h>
#include <dq_robotics/dq_controller.h>
#include <dq_robotics/DualArmPlotData.h>
#include <sensor_msgs/JointState.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <math.h>
#include "baxter_core_msgs/JointCommand.h"
#include "dq_robotics/BaxterControl.h"


class BaxterPoseControlServer
{
private:
	ros::NodeHandle rh;
	ros::ServiceServer controllerService;
	int joint_size_right, joint_size_left, joint_size_relative, joint_size_rightRelative_right, joint_size_rightRelative_relative;
	double mu, Kp, K_jl, Kp_position, K_jl_position, sleepTimeout, dt, timeStep, norm_error_const, norm_error, norm_error_right, norm_error_left, norm_error_relative, norm_error_rightRelative_relative, norm_error_rightRelative_right, dt_total, time_now, time_last;
	bool use_VM, dualArm, using_right, using_rel, using_position_control;
	ManipulatorModDQ* baxter;
	ros::Publisher left_cmd_pub, right_cmd_pub, system_state_pub;
	RowVectorXd q_baxter, q_vel_baxter, q_right, q_vel_right, q_left, q_vel_left, q_relative, q_vel_relative, q_rightRelative_relative, q_rightRelative_right, q_vel_rightRelative_relative, q_vel_rightRelative_right;
	RowVectorXd screw_error_right, screw_error_left, screw_error_rightRelative_right, screw_error_rightRelative_relative, screw_error_relative;
	baxter_core_msgs::JointCommand cmd_left, cmd_right;
	MatrixXd link_velocity_right, link_velocity_left, jacobian_6d_right, jacobian_6d_left, jacobian_6d_dot_right, jacobian_6d_dot_left, jacobian_8d_left, jacobian_8d_right, jacobian_8d_rightRelative, jacobian_8d_relative, jacobian_8d_rightRelative_relative, jacobian_8d_rightRelative_right;
	Matrix<double,8,1> pose_now_left, pose_desired_left, pose_now_right, pose_desired_right, pose_now_relative, pose_desired_relative, vel_now_relative, vel_desired_relative, vel_now_right, vel_desired_right, vel_now_left, vel_desired_left, pose_now_rightRelative_right, pose_now_rightRelative_relative, pose_desired_rightRelative_right, pose_desired_rightRelative_relative, vel_now_rightRelative_relative, vel_desired_rightRelative_relative, vel_now_rightRelative_right, vel_desired_rightRelative_right, pose_error_abs, pose_error_rel; 
	// std::vector<double> jointCmds;	
	ros::Time current_time, last_time;
	std::vector<double> joint_low_limit_right, joint_low_limit_left, joint_low_limit_relative, joint_low_limit_rightRelative, joint_high_limit_right, joint_high_limit_left, joint_high_limit_relative, joint_high_limit_rightRelative, joint_min_safe_limit_right, joint_min_safe_limit_left, joint_min_safe_limit_relative, joint_min_safe_limit_rightRelative, joint_max_safe_limit_right, joint_max_safe_limit_left, joint_max_safe_limit_relative, joint_max_safe_limit_rightRelative, joint_velocity_limit_right, joint_velocity_limit_left, joint_velocity_limit_relative, joint_velocity_limit_rightRelative, joint_high_limit_baxter, joint_low_limit_baxter, velocity_limit_baxter, max_safe_baxter, min_safe_baxter, joint_velocity_limit_rightRelative_right, joint_high_limit_rightRelative_right, joint_low_limit_rightRelative_right, joint_max_safe_limit_rightRelative_right, joint_min_safe_limit_rightRelative_right,	joint_velocity_limit_rightRelative_relative, joint_high_limit_rightRelative_relative, joint_low_limit_rightRelative_relative, joint_max_safe_limit_rightRelative_relative, joint_min_safe_limit_rightRelative_relative;
	
	
	std::vector<RowVector3d> u_baxter, p_baxter, u_right, p_right, u_left, p_left, u_relative, p_relative, u_rightRelative, p_rightRelative, u_rightRelative_relative, p_rightRelative_relative, u_rightRelative_right, p_rightRelative_right;

	Matrix<double,8,1> pe_init_left, pe_init_right, pe_init_relative, pe_init_rightRelative_right, pe_init_rightRelative_relative; 

	std::vector<std::string> joint_names_baxter, joint_names_right, joint_names_left, joint_names_relative, joint_names_rightRelative, joint_names_rightRelative_right, joint_names_rightRelative_relative;
	std::vector<int> joint_type_baxter, joint_type_right, joint_type_left, joint_type_relative, joint_type_rightRelative, joint_type_rightRelative_right, joint_type_rightRelative_relative;
	std::vector<Matrix<double,8,1> > fkm_matrix_left, fkm_matrix_right, fkm_matrix_relative, fkm_matrix_rightRelative_relative, fkm_matrix_rightRelative_right;
	// baxter_trajectory_interface::ControlSystemStates state;
	// double ratio_to_normalize_velocity_cmds;
	// bool getControllerParams();
	// bool initialize();
	// bool start();
	// bool update();
	// void sendVelocityCommandBaxter(std::vector<double> jointVelCmds);
	// void mapJointName2JointCmds();

public:
	BaxterPoseControlServer();	
	~BaxterPoseControlServer();
	void resetTime();
	bool initializeController();
	bool getControllerParams();
	void intializeMode_right();
	void intializeMode_left();
	void intializeMode_relative();
	void intializeMode_rightRelative();
	void update_manipulator();
	void update_right();
	void update_left();
	void update_relative();
	void update_rightRelative();
	void Run();
	RowVectorXd jointVelocity4velocityControl(double Kp, double mu, std::vector<RowVector3d> u, std::vector<RowVector3d> p, Matrix<double,8,1> pose_now, Matrix<double,8,1> pe_init, std::vector<int> joint_type, Matrix<double,8,1> pose_desired, Matrix<double,8,1> cart_vel_desired, std::vector<Matrix<double,8,1> >  fkm_matrix, double& norm_error);
	void remapJointCmds4RightArm(std::vector<double>  &jointVelocityCmds);
	void remapJointCmds4LeftArm(std::vector<double>  &jointVelocityCmds);
	void remapJointCmds4RelativeArm(std::vector<double>  &jointVelocityCmds);
	dq_robotics::BaxterControl::Response rightArmControl(dq_robotics::BaxterControl::Request baxterControl_req);
	dq_robotics::BaxterControl::Response leftArmControl(dq_robotics::BaxterControl::Request baxterControl_req);
	dq_robotics::BaxterControl::Response relativeArmControl(dq_robotics::BaxterControl::Request baxterControl_req);
	dq_robotics::BaxterControl::Response rightRelativeArmControl(dq_robotics::BaxterControl::Request baxterControl_req);
	dq_robotics::BaxterControl::Response rightOrientationRelativeArmControl(dq_robotics::BaxterControl::Request baxterControl_req);
	bool baxterControlServerCallback(dq_robotics::BaxterControl::Request& baxterControl_req, 
									dq_robotics::BaxterControl::Response& baxterControl_res);
	bool importManipulatorState(std::string arm, Matrix<double,8,1>& pe_init, Matrix<double,8,1>& pe_now, MatrixXd& jacobian, RowVectorXd& q, std::vector<double> joint_velocity_limit);
	bool updateManipulatorVariables(std::string arm);
	void sendBaxterJointCmds(std::string hand, std::vector<double> jointCmds, bool velocity_control);
	void initializeBaxterIKService();
	void sendBaxterVelocityCmds(std::string hand, std::vector<double> jointCmds);
	void update_rightAcc();
	void update_leftAcc();
	bool importManipulatorState_accControl(std::string arm, Matrix<double,8,1>& pe_init, Matrix<double,8,1>& pe_now, RowVectorXd& q, RowVectorXd& q_vel, MatrixXd& jacobian_6d, MatrixXd& jacobian_6d_dot);
	bool importJointLimits(std::string arm, RowVectorXd &joint_high_limit, RowVectorXd &joint_low_limit, RowVectorXd &joint_max_safe_limit, RowVectorXd &joint_min_safe_limit);	
};
		
#endif
