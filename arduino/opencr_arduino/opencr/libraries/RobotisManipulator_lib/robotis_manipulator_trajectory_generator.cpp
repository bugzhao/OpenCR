﻿/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Darby Lim, Hye-Jong KIM */

#include "robotis_manipulator_trajectory_generator.h"

using namespace ROBOTIS_MANIPULATOR;


MinimumJerk::MinimumJerk()
{
  coefficient_ = Eigen::VectorXd::Zero(6);
}

MinimumJerk::~MinimumJerk() {}

void MinimumJerk::calcCoefficient(WayPoint start,
                                  WayPoint goal,
                                  double move_time,
                                  double control_time)
{
  uint16_t step_time = uint16_t(floor(move_time / control_time) + 1.0);
  move_time = double(step_time - 1) * control_time;

  Eigen::Matrix3d A = Eigen::Matrix3d::Identity(3, 3);
  Eigen::Vector3d x = Eigen::Vector3d::Zero();
  Eigen::Vector3d b = Eigen::Vector3d::Zero();

  A << pow(move_time, 3), pow(move_time, 4), pow(move_time, 5),
      3 * pow(move_time, 2), 4 * pow(move_time, 3), 5 * pow(move_time, 4),
      6 * pow(move_time, 1), 12 * pow(move_time, 2), 20 * pow(move_time, 3);

  coefficient_(0) = start.value;
  coefficient_(1) = start.velocity;
  coefficient_(2) = 0.5 * start.effort;

  b << (goal.value - start.value - (start.velocity * move_time + 0.5 * start.effort * pow(move_time, 2))),
      (goal.velocity - start.velocity - (start.effort * move_time)),
      (goal.effort - start.effort);

  Eigen::ColPivHouseholderQR<Eigen::Matrix3d> dec(A);
  x = dec.solve(b);

  coefficient_(3) = x(0);
  coefficient_(4) = x(1);
  coefficient_(5) = x(2);
}

Eigen::VectorXd MinimumJerk::getCoefficient()
{
  return coefficient_;
}

//-------------------- Joint trajectory --------------------//

JointTrajectory::JointTrajectory()
{}

JointTrajectory::~JointTrajectory() {}

void JointTrajectory::init(double move_time,
                           double control_time, std::vector<WayPoint> start,
                           std::vector<WayPoint> goal)
{
  for (uint8_t index = 0; index < start.size(); index++)
  {
    trajectory_generator_.calcCoefficient(start.at(index),
                                    goal.at(index),
                                    move_time,
                                    control_time);

    coefficient_.col(index) = trajectory_generator_.getCoefficient();
  }
}

void JointTrajectory::setJointNum(uint8_t joint_num)
{
  joint_num_ = joint_num;
  coefficient_ = Eigen::MatrixXd::Identity(6, joint_num);
}

std::vector<WayPoint> JointTrajectory::getJointWayPoint(double tick)
{
  joint_way_point_.clear();
  for (uint8_t index = 0; index < joint_num_; index++)
  {
    WayPoint single_joint_way_point;
    single_joint_way_point.value = 0.0;
    single_joint_way_point.velocity = 0.0;
    single_joint_way_point.effort = 0.0;

    single_joint_way_point.value = coefficient_(0, index) +
             coefficient_(1, index) * pow(tick, 1) +
             coefficient_(2, index) * pow(tick, 2) +
             coefficient_(3, index) * pow(tick, 3) +
             coefficient_(4, index) * pow(tick, 4) +
             coefficient_(5, index) * pow(tick, 5);

    single_joint_way_point.velocity = coefficient_(1, index) +
             2 * coefficient_(2, index) * pow(tick, 1) +
             3 * coefficient_(3, index) * pow(tick, 2) +
             4 * coefficient_(4, index) * pow(tick, 3) +
             5 * coefficient_(5, index) * pow(tick, 4);

    single_joint_way_point.effort = 2 * coefficient_(2, index) +
             6 * coefficient_(3, index) * pow(tick, 1) +
             12 * coefficient_(4, index) * pow(tick, 2) +
             20 * coefficient_(5, index) * pow(tick, 3);

    joint_way_point_.push_back(single_joint_way_point);
  }

  return joint_way_point_;
}

Eigen::MatrixXd JointTrajectory::getCoefficient()
{
  return coefficient_;
}

//-------------------- Task trajectory --------------------//

TaskTrajectory::TaskTrajectory()
{
  dof_ = 6;
  position_coefficient_ = Eigen::MatrixXd::Identity(6, dof_);
}
TaskTrajectory::~TaskTrajectory() {}

void TaskTrajectory::init(double move_time,
                           double control_time, std::vector<WayPoint> start,
                           std::vector<WayPoint> goal)
{
  for (uint8_t index = 0; index < start.size(); index++)
  {
    trajectory_generator_.calcCoefficient(start.at(index),
                                    goal.at(index),
                                    move_time,
                                    control_time);

    position_coefficient_.col(index) = trajectory_generator_.getCoefficient();
  }
}

std::vector<WayPoint> TaskTrajectory::getTaskWayPoint(double tick)
{
  task_position_way_point_.clear();
  for (uint8_t index = 0; index < dof_; index++)
  {
    WayPoint single_task_position_way_point;
    single_task_position_way_point.value = 0.0;
    single_task_position_way_point.velocity = 0.0;
    single_task_position_way_point.effort = 0.0;

    single_task_position_way_point.value = position_coefficient_(0, index) +
             position_coefficient_(1, index) * pow(tick, 1) +
             position_coefficient_(2, index) * pow(tick, 2) +
             position_coefficient_(3, index) * pow(tick, 3) +
             position_coefficient_(4, index) * pow(tick, 4) +
             position_coefficient_(5, index) * pow(tick, 5);

    single_task_position_way_point.velocity = position_coefficient_(1, index) +
             2 * position_coefficient_(2, index) * pow(tick, 1) +
             3 * position_coefficient_(3, index) * pow(tick, 2) +
             4 * position_coefficient_(4, index) * pow(tick, 3) +
             5 * position_coefficient_(5, index) * pow(tick, 4);

    single_task_position_way_point.effort = 2 * position_coefficient_(2, index) +
             6 * position_coefficient_(3, index) * pow(tick, 1) +
             12 * position_coefficient_(4, index) * pow(tick, 2) +
             20 * position_coefficient_(5, index) * pow(tick, 3);

    task_position_way_point_.push_back(single_task_position_way_point);
  }

  return task_position_way_point_;
}


Eigen::MatrixXd TaskTrajectory::getCoefficient()
{
  return position_coefficient_;
}
