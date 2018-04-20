#include <Wire.h>
#include <Servo.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <ros.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Int16.h>

 
/* This driver reads raw data from the BNO055
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground
   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

ros::NodeHandle nh;
geometry_msgs:: Vector3 vect; //Use Vector3 to hold the Euler X and Z data
ros::Publisher pub("Imu_publisher", &vect); //Publish data to "Imu_publisher"

Adafruit_BNO055 bno = Adafruit_BNO055();
Servo servo;

void servo_cb(const std_msgs::Int16& cmd_msg){
  servo.write(cmd_msg.data); //set servo angle, should be from 0-180  
}

ros::Subscriber<std_msgs::Int16> sub("servo", servo_cb);  //subscribe to the "servo" topic

void setup(void){
 /* Set the delay between fresh samples. Wait for serial input to define sample rate. */
 #define BNO055_SAMPLERATE_DELAY_MS (100)
  //remove the above serial read after testing and timing diagram is established.
  Serial.begin(57600); //rosserial uses 57600 baud to connect

  //Attach the servo's data line to pin 9 on the Arduino
  servo.attach(9);

  //Initializes both servos to 180 degrees
  servo.write(180); 
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.println("ConnectionFail");
    while(1);
  }

  /*-----------------------Operation mode settings-----------------------
      OPERATION_MODE_CONFIG                                   = 0X00,
      OPERATION_MODE_ACCONLY                                  = 0X01,
      OPERATION_MODE_MAGONLY                                  = 0X02,
      OPERATION_MODE_GYRONLY                                  = 0X03,
      OPERATION_MODE_ACCMAG                                   = 0X04,
      OPERATION_MODE_ACCGYRO                                  = 0X05,
      OPERATION_MODE_MAGGYRO                                  = 0X06,
      OPERATION_MODE_AMG                                      = 0X07,
      OPERATION_MODE_IMUPLUS                                  = 0X08,
      OPERATION_MODE_COMPASS                                  = 0X09,
      OPERATION_MODE_M4G                                      = 0X0A,
      OPERATION_MODE_NDOF_FMC_OFF                             = 0X0B,
      OPERATION_MODE_NDOF    
  */
  
  bno.setMode(bno.OPERATION_MODE_IMUPLUS); //set the IMU to IMU mode as per datasheet pg.21

  bno.setExtCrystalUse(false);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub);
}

void loop(){
  Serial.println(IMU_data());
  nh.spinOnce();
  delay(1);
}

String IMU_data()
{
 String data;
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER); //collects Euler data from IMU registers.
  vect.x = euler.x();
  vect.z = euler.z();
  pub.publish(&vect);
  /* Display the floating point data */
 data = "horizontal[ " + String(euler.x()) + "] vertical[ " + String(euler.z()) + "]"; //creates formatted output of IMU data.
  return data;
}
