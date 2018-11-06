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

/* Authors: Hye-Jong KIM, Darby Lim*/

#include "../../include/open_manipulator/OMManager.h"

using namespace Eigen;

///////////////////////////*initialize function*/////////////////////////////
void OM_MANAGER::Manipulator::addWorld(Name world_name,
                           Name child_name,
                           Vector3f world_position,
                           Matrix3f world_orientation)
{
  world_.name = world_name;
  world_.child = child_name;
  world_.pose.position = world_position;
  world_.pose.orientation = world_orientation;
  world_.origin.velocity = VectorXf::Zero(3);
  world_.origin.acceleration = VectorXf::Zero(3);
}

void OM_MANAGER::Manipulator::addComponent(Name my_name,
                               Name parent_name,
                               Name child_name,
                               Vector3f relative_position,
                               Matrix3f relative_orientation,
                               Vector3f axis_of_rotation,
                               int8_t joint_actuator_id,
                               float coefficient,
                               float mass,
                               Matrix3f inertia_tensor,
                               Vector3f center_of_mass)
{
  if (joint_actuator_id != -1)
    dof_++;

  Component temp_component;

  temp_component.parent = parent_name;
  temp_component.child.push_back(child_name);
  temp_component.relative_to_parent.position = relative_position;
  temp_component.relative_to_parent.orientation = relative_orientation;
  temp_component.pose_to_world.position = Vector3f::Zero();
  temp_component.pose_to_world.orientation = Matrix3f::Identity(3, 3);
  temp_component.origin.velocity = VectorXf::Zero(3);
  temp_component.origin.acceleration = VectorXf::Zero(3);
  temp_component.joint.id = joint_actuator_id;
  temp_component.joint.coefficient = coefficient;
  temp_component.joint.axis = axis_of_rotation;
  temp_component.joint.angle = 0.0;
  temp_component.joint.velocity = 0.0;
  temp_component.joint.acceleration = 0.0;
  temp_component.tool.id = -1;
  temp_component.tool.coefficient = 0;
  temp_component.tool.on_off = false;
  temp_component.tool.value = 0.0;
  temp_component.inertia.mass = mass;
  temp_component.inertia.inertia_tensor = inertia_tensor;
  temp_component.inertia.center_of_mass = center_of_mass;

  component_.insert(std::make_pair(my_name, temp_component));
}

void OM_MANAGER::Manipulator::addComponentChild(Name my_name, Name child_name)
{
  component_.at(my_name).child.push_back(child_name);
}

void OM_MANAGER::Manipulator::addTool(Name my_name,
                          Name parent_name,
                          Vector3f relative_position,
                          Matrix3f relative_orientation,
                          int8_t tool_id,
                          float coefficient,
                          float mass,
                          Matrix3f inertia_tensor,
                          Vector3f center_of_mass)
{
  Component temp_component;

  temp_component.parent = parent_name;
  temp_component.relative_to_parent.position = relative_position;
  temp_component.relative_to_parent.orientation = relative_orientation;
  temp_component.pose_to_world.position = Vector3f::Zero();
  temp_component.pose_to_world.orientation = Matrix3f::Identity(3, 3);
  temp_component.origin.velocity = VectorXf::Zero(3);
  temp_component.origin.acceleration = VectorXf::Zero(3);
  temp_component.joint.id = -1;
  temp_component.joint.coefficient = 0;
  temp_component.joint.axis = Vector3f::Zero();
  temp_component.joint.angle = 0.0;
  temp_component.joint.velocity = 0.0;
  temp_component.joint.acceleration = 0.0;
  temp_component.tool.id = tool_id;
  temp_component.tool.coefficient = coefficient;
  temp_component.tool.on_off = false;
  temp_component.tool.value = 0.0;
  temp_component.inertia.mass = mass;
  temp_component.inertia.inertia_tensor = inertia_tensor;
  temp_component.inertia.center_of_mass = center_of_mass;

  component_.insert(std::make_pair(my_name, temp_component));
}

void OM_MANAGER::Manipulator::checkManipulatorSetting()
{
  USB.println("Degree of freedom : " + String(dof_));
  USB.println("Size of Components : " + String(component_.size()));

  USB.println("\n<Configuration of world>");
  USB.println("Name : " + String(world_.name));
  USB.println("Child name : " + String(world_.child));
  USB.println("Position : ");
  PRINT::VECTOR(world_.pose.position);
  USB.println("Orientation : ");
  PRINT::MATRIX(world_.pose.orientation);
  USB.println("Velocity : ");
  PRINT::VECTOR(world_.origin.velocity);
  USB.println("Acceleration : ");
  PRINT::VECTOR(world_.origin.acceleration);

  USB.println("\n<Configuration of components>");

  for (std::map<Name, Component>::iterator it = component_.begin(); it != component_.end(); it++)
  {
    USB.println("Name : " + String(it->first) + " -----------------------------------------------");
    USB.println("Parent : " + String(component_[it->first].parent));

    for (std::vector<Name>::size_type index = 0; index < component_[it->first].child.size(); index++)
      USB.println("Child  : " + String(component_[it->first].child[index]));

    USB.println("\nRelative to parent");
    USB.println(" Position : ");
    PRINT::VECTOR(component_[it->first].relative_to_parent.position);
    USB.println(" Orientation : ");
    PRINT::MATRIX(component_[it->first].relative_to_parent.orientation);

    USB.println("\nPose to world");
    USB.println(" Position : ");
    PRINT::VECTOR(component_[it->first].pose_to_world.position);
    USB.println(" Orientation : ");
    PRINT::MATRIX(component_[it->first].pose_to_world.orientation);

    USB.println("\nState to origin");
    USB.println(" Velocity : ");
    PRINT::VECTOR(component_[it->first].origin.velocity);
    USB.println(" Acceleration : ");
    PRINT::MATRIX(component_[it->first].origin.acceleration);

    USB.println("\nJoint");
    USB.println(" ID : " + String(component_[it->first].joint.id));
    USB.print(" Coefficient : "); USB.println(component_[it->first].joint.coefficient);
    USB.println(" Axis : ");
    PRINT::VECTOR(component_[it->first].joint.axis);
    USB.print(" Angle : "); USB.println(component_[it->first].joint.angle);
    USB.print(" Velocity : "); USB.println(component_[it->first].joint.velocity);
    USB.print(" Acceleration : "); USB.println(component_[it->first].joint.acceleration);

    USB.println("\nTool");
    USB.println(" ID : " + String(component_[it->first].tool.id));
    USB.print(" Coefficient : "); USB.println(component_[it->first].tool.coefficient);
    USB.println(" OnOff : " + String(component_[it->first].tool.on_off));
    USB.print(" Value : "); USB.println(component_[it->first].tool.value);

    USB.println("\nInertia");
    USB.print(" Mass : "); USB.println(component_[it->first].inertia.mass);
    USB.println(" Inertia tensor : ");
    PRINT::MATRIX(component_[it->first].inertia.inertia_tensor);
    USB.println(" Center of mass : ");
    PRINT::VECTOR(component_[it->first].inertia.center_of_mass);
    USB.println();
  }
}
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////Set function//////////////////////////////////
void OM_MANAGER::Manipulator::setWorldPose(Pose world_pose)
{
  world_.pose = world_pose;
}

void OM_MANAGER::Manipulator::setWorldPosition(Vector3f world_position)
{
  world_.pose.position = world_position;
}

void OM_MANAGER::Manipulator::setWorldOrientation(Matrix3f world_orientation)
{
  world_.pose.orientation = world_orientation;
}

void OM_MANAGER::Manipulator::setWorldState(State world_state)
{
  world_.origin = world_state;
}

void OM_MANAGER::Manipulator::setWorldVelocity(VectorXf world_velocity)
{
  world_.origin.velocity = world_velocity;
}

void OM_MANAGER::Manipulator::setWorldAcceleration(VectorXf world_acceleration)
{
  world_.origin.acceleration = world_acceleration;
}

void OM_MANAGER::Manipulator::setComponent(Name name, Component component, bool *error)
{
  if (component_.find(name) != component_.end())
  {
    component_.insert(std::make_pair(name, component));
    *error = false;
  }
  else
  {
    *error = true;
  }
}

void OM_MANAGER::Manipulator::setComponentPoseToWorld(Name name, Pose pose_to_world)
{
  if (component_.find(name) != component_.end())
  {
    component_.at(name).pose_to_world = pose_to_world;
  }
  else
  {
    //error
  }
}

void OM_MANAGER::Manipulator::setComponentPositionToWorld(Name name, Vector3f position_to_world)
{
  if (component_.find(name) != component_.end())
  {
    component_.at(name).pose_to_world.position = position_to_world;
  }
  else
  {
    //error
  }
}

void OM_MANAGER::Manipulator::setComponentOrientationToWorld(Name name, Matrix3f orientation_to_wolrd)
{
  if (component_.find(name) != component_.end())
  {
    component_.at(name).pose_to_world.orientation = orientation_to_wolrd;
  }
  else
  {
    //error
  }
}

void OM_MANAGER::Manipulator::setComponentStateToWorld(Name name, State state_to_world)
{
  if (component_.find(name) != component_.end())
  {
    component_.at(name).origin = state_to_world;
  }
  else
  {
    //error
  }
}

void OM_MANAGER::Manipulator::setComponentVelocityToWorld(Name name, VectorXf velocity)
{
  if (velocity.size() != 6)
  {
    //error
  }
  else
  {
    if (component_.find(name) != component_.end())
    {
      component_.at(name).origin.velocity = velocity;
    }
    else
    {
      //error
    }
  }
}

void OM_MANAGER::Manipulator::setComponentAccelerationToWorld(Name name, VectorXf acceleration)
{
  if (acceleration.size() != 6)
  {
    //error
  }
  else
  {
    if (component_.find(name) != component_.end())
    {
      component_.at(name).origin.acceleration = acceleration;
    }
    else
    {
      //error
    }
  }
}

void OM_MANAGER::Manipulator::setComponentJointAngle(Name name, float angle)
{
  if (component_.at(name).tool.id > 0)
  {
    //error
  }
  else
  {
    if (component_.find(name) != component_.end())
    {
      component_.at(name).joint.angle = angle;
    }
    else
    {
      //error
    }
  }
}

void OM_MANAGER::Manipulator::setComponentJointVelocity(Name name, float angular_velocity)
{
  if (component_.at(name).tool.id > 0)
  {
    //error
  }
  else
  {
    if (component_.find(name) != component_.end())
    {
      component_.at(name).joint.velocity = angular_velocity;
    }
    else
    {
      //error
    }
  }
}

void OM_MANAGER::Manipulator::setComponentJointAcceleration(Name name, float angular_acceleration)
{
  if (component_.at(name).tool.id > 0)
  {
    //error
  }
  else
  {
    if (component_.find(name) != component_.end())
    {
      component_.at(name).joint.acceleration = angular_acceleration;
    }
    else
    {
      //error
    }
  }
}

void OM_MANAGER::Manipulator::setComponentToolOnOff(Name name, bool on_off)
{
  if (component_.at(name).tool.id > 0)
  {
    if (component_.find(name) != component_.end())
    {
      component_.at(name).tool.on_off = on_off;
    }
    else
    {
      //error
    }
  }
  else
  {
    //error
  }
}

void OM_MANAGER::Manipulator::setComponentToolValue(Name name, float value)
{
  if (component_.at(name).tool.id > 0)
  {
    if (component_.find(name) != component_.end())
    {
      component_.at(name).tool.value = value;
    }
    else
    {
      //error
    }
  }
  else
  {
    //error
  }
}

void OM_MANAGER::Manipulator::setAllActiveJointAngle(std::vector<float> angle_vector)
{
  std::map<Name, Component>::iterator it;
  int8_t index = 0;

  for (it = component_.begin(); it != component_.end(); it++)
  {
    if (component_.at(it->first).joint.id != -1)
    {
      component_.at(it->first).joint.angle = angle_vector.at(index);
      index++;
    }
  }
}

///////////////////////////////Get function//////////////////////////////////

int8_t OM_MANAGER::Manipulator::getDOF()
{
  return dof_;
}

int8_t OM_MANAGER::Manipulator::getComponentSize()
{
  return component_.size();
}

Name OM_MANAGER::Manipulator::getWorldName()
{
  return world_.name;
}

Name OM_MANAGER::Manipulator::getWorldChildName()
{
  return world_.child;
}

Pose OM_MANAGER::Manipulator::getWorldPose()
{
  return world_.pose;
}

Vector3f OM_MANAGER::Manipulator::getWorldPosition()
{
  return world_.pose.position;
}

Matrix3f OM_MANAGER::Manipulator::getWorldOrientation()
{
  return world_.pose.orientation;
}

State OM_MANAGER::Manipulator::getWorldState()
{
  return world_.origin;
}

VectorXf OM_MANAGER::Manipulator::getWorldVelocity()
{
  return world_.origin.velocity;
}

VectorXf OM_MANAGER::Manipulator::getWorldAcceleration()
{
  return world_.origin.acceleration;
}

std::map<Name, Component> OM_MANAGER::Manipulator::getAllComponent()
{
  return component_;
}

std::map<Name, Component>::iterator OM_MANAGER::Manipulator::getIteratorBegin()
{
  return component_.begin();
}

std::map<Name, Component>::iterator OM_MANAGER::Manipulator::getIteratorEnd()
{
  return component_.end();;
}

Component OM_MANAGER::Manipulator::getComponent(Name name)
{
  return component_.at(name);
}

Name OM_MANAGER::Manipulator::getComponentParentName(Name name)
{
  return component_.at(name).parent;
}

std::vector<Name> OM_MANAGER::Manipulator::getComponentChildName(Name name)
{
  return component_.at(name).child;
}

Pose OM_MANAGER::Manipulator::getComponentPoseToWorld(Name name)
{
  return component_.at(name).pose_to_world;
}

Vector3f OM_MANAGER::Manipulator::getComponentPositionToWorld(Name name)
{
  return component_.at(name).pose_to_world.position;
}

Matrix3f OM_MANAGER::Manipulator::getComponentOrientationToWorld(Name name)
{
  return component_.at(name).pose_to_world.orientation;
}

State OM_MANAGER::Manipulator::getComponentStateToWorld(Name name)
{
  return component_.at(name).origin;
}

VectorXf OM_MANAGER::Manipulator::getComponentVelocityToWorld(Name name)
{
  return component_.at(name).origin.velocity;
}

VectorXf OM_MANAGER::Manipulator::getComponentAccelerationToWorld(Name name)
{
  return component_.at(name).origin.acceleration;
}

Pose OM_MANAGER::Manipulator::getComponentRelativePoseToParent(Name name)
{
  return component_.at(name).relative_to_parent;
}

Vector3f OM_MANAGER::Manipulator::getComponentRelativePositionToParent(Name name)
{
  return component_.at(name).relative_to_parent.position;
}

Matrix3f OM_MANAGER::Manipulator::getComponentRelativeOrientationToParent(Name name)
{
  return component_.at(name).relative_to_parent.orientation;
}

Joint OM_MANAGER::Manipulator::getComponentJoint(Name name)
{
  return component_.at(name).joint;
}

int8_t OM_MANAGER::Manipulator::getComponentJointId(Name name)
{
  return component_.at(name).joint.id;
}

float OM_MANAGER::Manipulator::getComponentJointCoefficient(Name name)
{
  return component_.at(name).joint.coefficient;
}

Vector3f OM_MANAGER::Manipulator::getComponentJointAxis(Name name)
{
  return component_.at(name).joint.axis;
}

float OM_MANAGER::Manipulator::getComponentJointAngle(Name name)
{
  return component_.at(name).joint.angle;
}

float OM_MANAGER::Manipulator::getComponentJointVelocity(Name name)
{
  return component_.at(name).joint.velocity;
}

float OM_MANAGER::Manipulator::getComponentJointAcceleration(Name name)
{
  return component_.at(name).joint.acceleration;
}

Tool OM_MANAGER::Manipulator::getComponentTool(Name name)
{
  return component_.at(name).tool;
}

int8_t OM_MANAGER::Manipulator::getComponentToolId(Name name)
{
  return component_.at(name).tool.id;
}

float OM_MANAGER::Manipulator::getComponentToolCoefficient(Name name)
{
  return component_.at(name).tool.coefficient;
}

bool OM_MANAGER::Manipulator::getComponentToolOnOff(Name name)
{
  return component_.at(name).tool.on_off;
}

float OM_MANAGER::Manipulator::getComponentToolValue(Name name)
{
  return component_.at(name).tool.value;
}

float OM_MANAGER::Manipulator::getComponentMass(Name name)
{
  return component_.at(name).inertia.mass;
}

Matrix3f OM_MANAGER::Manipulator::getComponentInertiaTensor(Name name)
{
  return component_.at(name).inertia.inertia_tensor;
}

Vector3f OM_MANAGER::Manipulator::getComponentCenterOfMass(Name name)
{
  return component_.at(name).inertia.center_of_mass;
}

std::vector<float> OM_MANAGER::Manipulator::getAllJointAngle()
{
  std::vector<float> result_vector;
  std::map<Name, Component>::iterator it;

  for (it = component_.begin(); it != component_.end(); it++)
  {
    if (component_.at(it->first).tool.id == -1) // Check whether Tool or not
    {
      // This is not Tool -> This is Joint
      result_vector.push_back(component_.at(it->first).joint.angle);
    }
  }
  return result_vector;
}

std::vector<float> OM_MANAGER::Manipulator::getAllActiveJointAngle()
{
  std::vector<float> result_vector;
  std::map<Name, Component>::iterator it;

  for (it = component_.begin(); it != component_.end(); it++)
  {
    if (component_.at(it->first).joint.id != -1) // Check whether Active or Passive
    {
      // Active
      result_vector.push_back(component_.at(it->first).joint.angle);
    }
  }
  return result_vector;
}

std::vector<uint8_t> OM_MANAGER::Manipulator::getAllActiveJointID()
{
  std::vector<uint8_t> active_joint_id;
  std::map<Name, Component>::iterator it;

  for (it = component_.begin(); it != component_.end(); it++)
  {
    if (component_.at(it->first).joint.id != -1)
    {
      active_joint_id.push_back(component_.at(it->first).joint.id);
    }
  }
  return active_joint_id;
}

