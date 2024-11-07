#include <ros/ros.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>   
#include <sstream> 
#include <fstream>

#include <integrated_navigation/integratedNavigationMsgx1.h>
#include "uart.h"

/* buffer of receive */
char recvBuff[1024];

/* data of type of string be converted to type of number */
template <typename Type>
Type stringToNum(const std::string &str)
{
	std::stringstream iss(str);
	Type num;
	iss >> num;
	return num;
}


int main( int argc, char **argv )
{
	std::cout<<"Program begins ... "<<std::endl;
	
	int recvCount = 0;
	
	int previous = 0, rear = 0; // pointer to indicate the position of the exact data int a receive buffer.

	/* init the ros */
	ros::init( argc, argv, "IntegrateNavigationSystem" );
	ros::NodeHandle n;

	/* Publisher of the integrated navigation data */
	ros::Publisher pub = n.advertise<integrated_navigation::integratedNavigationMsgx1>( "/integratedNavigation/data", 10);


	/* open the uart device */
	int uart_fd = open_port();
	std::cout<<"uart fd = "<<uart_fd <<std::endl;
	if( uart_fd > 0 ){
		if( set_opt( uart_fd ) != true ){
			std::cerr<<"set port failed ... "<<std::endl;
			return false;
		}
	}
	else{
		std::cerr<<"open port failed ..."<<std::endl;
		exit(-1);
	}

	sleep( 1 );

	std::string s1; 
	std::vector<std::string> arr1; // define a vector of string
	int position = 0;	
	integrated_navigation::integratedNavigationMsgx1 recvMsg;
	// ros::Rate loop(200);
	while(ros::ok()){

		memset( recvBuff, 0, sizeof( recvBuff ) );

		if( recvData( uart_fd, recvBuff, sizeof( recvBuff ) ) ){

			if( recvCount <= 1 ) recvCount ++;
			if( recvCount > 1 ){
				/* parse the one frame of message */
				s1 = recvBuff;
				position = s1.find(";");
				s1.erase(0, position + 1);
				position = s1.find("*");
				s1.erase(position);
				do{
					std::string tmp_s;
					position = s1.find(",");    
					tmp_s = s1.substr(0, position);     
					s1.erase(0, position + 1);      
					arr1.push_back(tmp_s); 
				} while (position != -1);
	
				int d0 = stringToNum<int>( arr1[0] ); // GPSWeek 
				// printf("GPSWeek = %d\r\n", d0);
				recvMsg.GPSWeek = d0;
				double d1 = stringToNum<double>( arr1[1] ); // GPSTime 
				// printf( "GPSTime = %f\r\n", d1 );
				recvMsg.GPSTime = d1;
				double d2 = stringToNum<double>( arr1[2] ); // Lattitude 
				// printf("Lattitude = %f\r\n", d2);
				recvMsg.Lattitude = d2;
				double d3 = stringToNum<double>( arr1[3] ); // Longitude
				// printf("Longitude = %f\r\n", d3);
				recvMsg.Longitude = d3;
				double d4 = stringToNum<double>( arr1[4] ); // Altitude
				// printf("Altitude = %f\r\n", d4);
				recvMsg.Altitude = d4;
				double d5 = stringToNum<double>( arr1[5] ); // Vn
				// printf("Vn = %f\r\n", d5);
				recvMsg.Vn = d5;
				double d6 = stringToNum<double>( arr1[6] ); // Ve
				// printf("Ve = %f\r\n", d6);
				recvMsg.Ve = d6;
				double d7 = stringToNum<double>( arr1[7] ); // Vu
				// printf("Vu = %f\r\n", d7);
				recvMsg.Vu = d7;
				double d8 = stringToNum<double>( arr1[8] ); // Roll
				// printf("Roll = %f\r\n", d8);
				recvMsg.Roll = d8;
				double d9 = stringToNum<double>( arr1[9] ); // Pitch
				// printf("Pitch = %f\r\n", d9);
				recvMsg.Pitch = d9;
				double d10 = stringToNum<double>( arr1[10] ); // Azimuth
				// printf("Azimuth = %f\r\n", d10);
				recvMsg.Azimuth = d10;
				std::string d11 =  arr1[11]; // Status
				// printf("Status = %s\r\n", d11.c_str());
				recvMsg.Status = d11;

				arr1.clear();
				recvMsg.header.stamp = ros::Time::now();
				pub.publish( recvMsg );
			}
		}
		// ros::spinOnce();
        // loop.sleep();
	}

	return 0;
}
