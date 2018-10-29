/*******************************************************************************
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

#ifndef OMPATH_H_
#define OMPATH_H_

#include <Eigen.h>  // Calls main Eigen matrix class library
#include <Eigen/LU> // Calls inverse, determinant, LU decomp., etc.
#include <Eigen/QR>

#include <math.h>
#include <vector>

#include "OMAPI.h"
#include "OMDebug.h"

using namespace Eigen;

typedef struct
{
  float position;
  float velocity;
  float acceleration;
} Trajectory;

namespace OM_PATH
{
class MinimumJerk
{
private:
  VectorXf coefficient_;

public:
  MinimumJerk();
  virtual ~MinimumJerk();

  void calcCoefficient(Trajectory start,
                       Trajectory goal,
                       float move_time,
                       float control_time);

  VectorXf getCoefficient();
};

class JointTrajectory
{
private:
  MinimumJerk path_generator_;

  uint8_t joint_num_;
  MatrixXf coefficient_;
  std::vector<float> position_;
  std::vector<float> velocity_;
  std::vector<float> acceleration_;

public:
  JointTrajectory(uint8_t joint_num);
  virtual ~JointTrajectory();

  void init(std::vector<Trajectory> start,
            std::vector<Trajectory> goal,
            float move_time,
            float control_time);

  std::vector<float> getPosition(float tick);
  std::vector<float> getVelocity(float tick);
  std::vector<float> getAcceleration(float tick);

  MatrixXf getCoefficient();
};
class Line
{
private:
  Pose start_;
  Pose end_;
  float acc_dec_time;
  float move_time_;
  Vector3f vel_max;

public:
  Line();
  virtual ~Line();

  void init(Pose start, Pose end, float move_time, float control_time);
  Pose line(float time_var);
  Pose getPose(float tick);

  MatrixXf getCoefficient();
};

// class Arc
// {
// private:
//   MinimumJerk path_generator_;
//   MatrixXf coefficient_;

//   uint8_t joint_num_;

// public:
//   Arc(uint8_t joint_num);
//   virtual ~Arc();

//   void init(float move_time, float control_time);
//   void setFunction();
//   Pose getPose(float tick);

//   MatrixXf getCoefficient();
// };

class Circle : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  Circle();
  virtual ~Circle();

  void init(float move_time, float control_time);
  Pose circle(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};


class CircleEdge : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  CircleEdge();
  virtual ~CircleEdge();

  void init(float move_time, float control_time);
  Pose circleedge(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};

class CircleEdge2 : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  CircleEdge2();
  virtual ~CircleEdge2();

  void init(float move_time, float control_time);
  Pose circleedge2(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};


class Spiral : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  Spiral();
  virtual ~Spiral();

  void init(float move_time, float control_time);
  Pose spiral(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};

class Spiral2 : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  Spiral2();
  virtual ~Spiral2();

  void init(float move_time, float control_time);
  Pose spiral2(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};

class Rhombus : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  Rhombus();
  virtual ~Rhombus();

  void init(float move_time, float control_time);
  Pose rhombus(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);

  virtual Pose getPose(float tick);
};


class Heart : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  Heart();
  virtual ~Heart();

  void init(float move_time, float control_time);
  Pose heart(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);

  virtual Pose getPose(float tick);
};


class BottleShake : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  BottleShake();
  virtual ~BottleShake();

  void init(float move_time, float control_time);
  Pose bottleshake(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};


class BottleShakeY : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  BottleShakeY();
  virtual ~BottleShakeY();

  void init(float move_time, float control_time);
  Pose bottleshake(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};
class BottleShakeX : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  BottleShakeX();
  virtual ~BottleShakeX();

  void init(float move_time, float control_time);
  Pose bottleshake(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};


class BottleShake3 : public OPEN_MANIPULATOR::Draw
{
private:
  MinimumJerk path_generator_;
  MatrixXf coefficient_;

  uint8_t joint_num_;

  Vector3f start_position_;
  float radius_;
  float start_angular_position_;

  float *get_arg_;

public:
  BottleShake3();
  virtual ~BottleShake3();

  void init(float move_time, float control_time);
  Pose bottleshake3(float time_var);

  MatrixXf getCoefficient();

  virtual void initDraw(const void *arg);
  virtual void setRadius(float radius);  
  virtual void setStartPosition(Vector3f start_position);
  virtual void setAngularStartPosition(float start_angular_position);
  virtual Pose getPose(float tick);
};



} // namespace OM_PATH
#endif // OMPATH_H_




