#ifndef EKF_TRANSLATION
#define EKF_TRANSLATION

#include "auv_navigation/kalman_filter.hpp"
#include "auv_navigation/auv_math_lib.hpp"
#include "eigen3/Eigen/Dense"
#include "math.h"
#include <sstream>

namespace auv_navigation
{
typedef Eigen::Matrix<float, 9, 1> Vector9f;
typedef Eigen::Matrix<float, 9, 9> Matrix9f;

// Translational Extended Kalman Filter
// This class is designed to estimate a vehicle's position as expressed in the I-frame,
// and the vehicle's linear velocity and acceleration as expressed in the B-frame.
// Since the sensory input come from INERTIAL sensors, state predictions are ALSO inertial.
// Assumptions: the velocity and acceleration sensor provide data in all three axis
// NOTES: Body frame sensor data will be converted to inertial frame coordinates.
//        Calculations performed in inertial frame coordinates.
//        Result returned in inertial frame coordinates
class TranslationEKF
{
private:
  KalmanFilter *ekf_;
  Vector9f Xhat_;
  Eigen::Vector3i posMask_;
  Eigen::Matrix3i fullMsmtMask_;
  Eigen::Matrix3f Rvel_, Raccel_;
  Eigen::MatrixXf Rpos_;
  Matrix9f Q_;
  bool init_;
  int n_; // Size of A matrix (nxn = 9x9)

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  TranslationEKF(const Eigen::Ref<const Eigen::Vector3i> &posMask,
                 const Eigen::Ref<const Eigen::MatrixXf> &Rpos,
                 const Eigen::Ref<const Eigen::Matrix3f> &Rvel,
                 const Eigen::Ref<const Eigen::Matrix3f> &Raccel,
                 const Eigen::Ref<const Matrix9f> &Q);
  void init(const Eigen::Ref<const Eigen::VectorXf> &Xo);
  Vector9f update(float dt, const Eigen::Ref<const Eigen::Vector3i> &sensorMask,
                  const Eigen::Ref<const Eigen::Matrix3f> &Zmat);
};
} // namespace AUVNavigation

#endif