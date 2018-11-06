#include <OMDynamixel.h>
#include <OMPath.h>
#include <vector>

#define DXL_SIZE 3
#define BAUD_RATE 1000000

#define DEG2RAD (M_PI / 180.0)
#define RAD2DEG (180.0 / M_PI)

#define CONTROL_TIME 0.008f
#define MOVE_TIME 3.0f

OM_PATH::JointTrajectory joint_trajectory(DXL_SIZE);
std::vector<Trajectory> start_trajectory;
std::vector<Trajectory> goal_trajectory;
Trajectory start, goal;

OM_DYNAMIXEL::Dynamixel dxl;
std::vector<float> goal_position;
std::vector<float> goal_velocity;
std::vector<float> goal_acceleration;
std::vector<float> present_position;

static float tTime[2] = {0.0, 0.0};

void setup()
{
  Serial.begin(57600);
  while(!Serial);

  dxl.init(BAUD_RATE);
  //dxl.setMaxPositionLimit(1, 180*DEG2RAD);
  //dxl.setMinPositionLimit(1, -180*DEG2RAD);

  dxl.enableAllDynamixel();

  goal_position.reserve(DXL_SIZE);
  goal_velocity.reserve(DXL_SIZE);
  goal_acceleration.reserve(DXL_SIZE);

  present_position.reserve(DXL_SIZE);

  start_trajectory.reserve(DXL_SIZE);
  goal_trajectory.reserve(DXL_SIZE);

  present_position = dxl.getAngle();

  start.position = -90.0f * DEG2RAD;//present_position.at(0);
  start.velocity = 0.0;
  start.acceleration = 0.0;

  goal.position = 90.0f * DEG2RAD;
  goal.velocity = 0.0;
  goal.acceleration = 0.0;

  start_trajectory.push_back(start);
  start_trajectory.push_back(start);
  start_trajectory.push_back(start);

  goal_trajectory.push_back(goal);
  goal_trajectory.push_back(goal);
  goal_trajectory.push_back(goal);

  joint_trajectory.init(start_trajectory, goal_trajectory, MOVE_TIME, CONTROL_TIME);
}

void loop() 
{
  float t = (float)(millis() / 1000.0f);

  if ((t-tTime[0]) >= CONTROL_TIME)
  {
    // present_position = dxl.getAngle();

    tTime[0] = t;
  }

  if ((t-tTime[1]) >= CONTROL_TIME)
  {
    jointControl();
    LOG::INFO("TIME : ", t-tTime[1]);
    
    tTime[1] = t;
  }
}

void jointControl()
{
  uint16_t step_time = uint16_t(floor(MOVE_TIME/CONTROL_TIME) + 1.0);
  float tick_time = 0;
  static uint16_t step_cnt = 0;
  static float moving = true;

  goal_position.clear();
  goal_velocity.clear();
  goal_acceleration.clear();

  if (moving)
  {
    if (step_cnt < step_time)
    {
      tick_time = CONTROL_TIME * step_cnt;
      
      goal_position = joint_trajectory.getPosition(tick_time);
      goal_velocity = joint_trajectory.getVelocity(tick_time);
      goal_acceleration = joint_trajectory.getAcceleration(tick_time);

      dxl.setAngle(goal_position);
      log();

      step_cnt++;
    }
    else
    {
      step_cnt = 0;
      moving   = false; 
    }
  }  
}

void log()
{
  Serial.print("[");
  for (int8_t index = 0; index < DXL_SIZE; index++)
  {
    Serial.print("ID : ");
    Serial.print(dxl.getDynamixelIDs().at(index));
    Serial.print(" GoalPos: ");
    Serial.print(dxl.convertRadian2Value(1, goal_position.at(index)));
    // Serial.print(goal_position.at(index));
    Serial.print(" | ");
  }
  Serial.println("]");
}