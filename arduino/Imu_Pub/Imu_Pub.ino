#include <ros.h>
#include <geometry_msgs/Vector3.h>

ros::NodeHandle nh;

geometry_msgs::Vector3 vect;

// ros::Subscriber<geometry_msgs::Vector3> sub("euler", &imuCallback);

ros::Publisher pub("Imu_publisher", &vect);

//Publishes the euler data obtained from the IMU to Vector3
void imuCallback(const geometry_msgs::Vector3& msg)
{
	vect.x = msg.x;
	vect.y = msg.y;
	vect.z = msg.z;

	pub.publish(&vect);
}

void setup()
{
	nh.initNode();
	// nh.subscribe(sub);
	nh.advertise(pub);
}

void loop()
{
	nh.spinOnce();
}
