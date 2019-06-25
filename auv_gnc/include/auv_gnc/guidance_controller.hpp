#ifndef GUIDANCE_CONTROLLER
#define GUIDANCE_CONTROLLER

#include "auv_core/constants.hpp"
#include "auv_guidance/basic_trajectory.hpp"
#include "auv_guidance/tgen_limits.hpp"
#include "auv_guidance/waypoint.hpp"
#include "auv_control/auv_model.hpp"
#include "auv_msgs/SixDoF.h"
#include "auv_msgs/Thrust.h"
#include "auv_msgs/Trajectory.h"
#include "auv_msgs/TrajectoryGeneratorAction.h"

#include <actionlib/server/simple_action_server.h>
#include <ros/ros.h>
#include <yaml-cpp/yaml.h>
#include "math.h"
#include <algorithm>
#include <vector>
#include <boost/thread.hpp>

namespace acc = auv_core::constants;

namespace auv_gnc
{
class GuidanceController
{
private:
  // AUV Model parameters
  int activeThrusters_;
  std::vector<std::string> activeThrusterNames_, inactiveThrusterNames_;
  std::string auvConfigFile_;
  YAML::Node auvConfig_;
  auv_control::AUVModel *auvModel_;

  // LQR Parameters
  std::vector<double> Qdiag_, Rdiag_;
  auv_control::Matrix12d Q_;
  auv_control::Matrix8d R_;

  // Trajectory Generator Parameters
  typedef auv_msgs::Trajectory amt;
  int tGenType_;
  bool tGenInit_;
  Eigen::Quaterniond quaternion_;

  auv_guidance::TGenLimits *tGenLimits_;
  auv_guidance::Vector12d state_;
  auv_guidance::Waypoint *currentState_;
  auv_guidance::BasicTrajectory *basicTrajectory_;

  typedef actionlib::SimpleActionServer<auv_msgs::TrajectoryGeneratorAction> TGenActionServer;
  typedef std::shared_ptr<TGenActionServer> TGenActionServerPtr;

  bool init_;
  double dt;
  ros::Time timeLast_;

  ros::NodeHandle nh_;
  ros::Subscriber sixDoFSub_;
  ros::Publisher thrustPub_;
  std::string subTopic_, pubTopic_, actionName_;
  TGenActionServerPtr tgenActionServer_;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  GuidanceController(ros::NodeHandle nh);
  void initAUVModel();
  void sixDofCB(const auv_msgs::SixDoF::ConstPtr &state);
  void tgenActionGoalCB();
  void tgenActionPreemptCB();
  void runController();
};
} // namespace auv_gnc

#endif