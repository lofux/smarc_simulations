#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>

class My_Filter {
     public:
        My_Filter();
        void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
     private:
        ros::NodeHandle node_;
        laser_geometry::LaserProjection projector_;
        tf::TransformListener tfListener_;

        ros::Publisher point_cloud_publisher_;
        ros::Subscriber scan_sub_;
};

My_Filter::My_Filter(){
        scan_sub_ = node_.subscribe<sensor_msgs::LaserScan> ("/lolo_auv/n1/sss_right", 100, &My_Filter::scanCallback, this);
        //point_cloud_publisher_ = node_.advertise<sensor_msgs::PointCloud2> ("/lolo_auv/cloud", 100, false);
        point_cloud_publisher_ = node_.advertise<sensor_msgs::PointCloud2> ("/lolo_auv/n1/sss_right/cloud", 100, false);
        tfListener_.setExtrapolationLimit(ros::Duration(0.1));
}

void My_Filter::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan){
    sensor_msgs::PointCloud2 cloud;

    int i = 0;
    int num_attempts = 5;
    while (i < num_attempts) {

        try{

            projector_.transformLaserScanToPointCloud("/lolo_auv/base_link", *scan, cloud, tfListener_);
            point_cloud_publisher_.publish(cloud);
            break;
        }

        catch (tf::TransformException ex) {
            i++;
            ROS_WARN("%s",ex.what());
            ros::Duration(0.1).sleep();

        }

    }

    if (i >= num_attempts)
    {
        ROS_ERROR("Failed to ...");
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_filter");

    My_Filter filter;

    ros::spin();

    return 0;
}
