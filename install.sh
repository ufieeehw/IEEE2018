#!/bin/bash

#Created by: Anothony Olive
#Maintained by: Quade Kirby

NOCOLOR='\033[0m'
LOGCOLOR='\033[1;36m'
PASSCOLOR='\033[1;32m'
WARNCOLOR='\033[1;31m'

LOGPREFIX="${LOGCOLOR}INSTALLER:"
WARNPREFIX="${WARNCOLOR}ERROR:"
PASSTEXT="${PASSCOLOR}PASS"
FAILTEXT="${WARNCOLOR}FAIL"

instlog() {
	printf "$LOGPREFIX $@ $NOCOLOR\n"
}

instwarn() {
	printf "$WARNPREFIX $@ $NOCOLOR\n"
}


instpass() {
	printf "$PASSTEXT $NOCOLOR"
}


instfail() {
	printf "$FAILTEXT $NOCOLOR"
}

check_host() {
	HOST="$1"

	# Attempts to ping the host to make sure it is reachable
	HOST_PING=$(ping -c 2 $HOST 2>&1 | grep "% packet" | awk -F'[%]' '{print $1}' | awk -F'[ ]' '{print $NF}')
	if [ ! -z "${HOST_PING}" ]; then

		# Uses packet loss percentage to determine if the connection is strong
		if [ $HOST_PING -lt 25 ]; then

			# Will return true if ping was successful and packet loss was below 25%
			echo "true"
		fi
	fi
}


#=======================#
# Configurable Defaults #
#=======================#

CATKIN_DIR=~/ieee_ws
BASHRC_FILE=~/.bashrc


#======================#
# Script Configuration #
#======================#

# Set sane defaults for other install parameters
ENABLE_UDEV_RULES=false

# Prompt the user to enter a catkin workspace to use
echo "Catkin is the ROS build system and it combines CMake macros and Python scripts."
echo "The catkin workspace is the directory where all source and build files for the"
echo "project are stored. Our default is in brackets below, press enter to use it."
echo -n "What catkin workspace should be used? [$CATKIN_DIR]: " && read RESPONSE
if [ "$RESPONSE" != "" ]; then
	CATKIN_DIR=${RESPONSE/\~//home/$USER}
fi
echo ""

if [ ! -d $CATKIN_DIR/src/IEEE2018 ]; then
	echo "We use a forking workflow to facilitate code contributions on Github. This means"
	echo "that each user forks the main repository and has their own copy. In the"
	echo "repositories that we clone for projects, the main repository will be the"
	echo "'upstream' remote and your local fork will be the 'origin' remote. You should"
	echo "specify a fork URI for each repository you plan to push code to; otherwise,"
	echo "leave the field blank. These can also be set manually using this command:"
	echo "git remote add <remote_name> <user_fork_url>"
	echo -n "User fork URI for the mil_common repository: " && read USER_FORK
	echo ""
fi

# Warn users about the security risks associated with enabling Udev rules before doing it
if [ ! -f /etc/udev/rules.d/47-hokuyo.rules ]; then
	echo "The IEEE robot requires Udev rules to ensure that sensors are always"
	echo "accessible on the same system path. This is required for the vehicle's"
	echo "computers, but optional for user computers. Users should only enable the Udev"
	echo "rules if they need to interface directly with the required sensors. It should"
	echo "also be noted that installing these may compromise the security of the system."
	echo -n "Do you want to enable the Udev rules on this system? [y/N] " && read RESPONSE
	if ([ "$RESPONSE" = "Y" ] || [ "$RESPONSE" = "y" ]); then
		ENABLE_UDEV_RULES=true
	fi
	echo ""
else
	ENABLE_UDEV_RULES=true
fi


#==================#
# Pre-Flight Check #
#==================#

instlog "Acquiring root privileges"

# Gets the user to enter their password here instead of in the middle of the pre-flight check
# Purely here for aesthetics and to satisfy OCD
sudo true

instlog "Starting the pre-flight system check to ensure installation was done properly"

# Check whether or not github.com is reachable
# This also makes sure that the user is connected to the internet
if [ "`check_host github.com`" = "true" ]; then
	NET_CHECK=true
	echo -n "[ " && instpass && echo -n "] "
else
	NET_CHECK=false
	echo -n "[ " && instfail && echo -n "] "
fi
echo "Internet connectivity check"

if !($NET_CHECK); then

	# The script will not allow the user to install without internet
	instwarn "Terminating installation due to the lack of an internet connection"
	instwarn "The install script needs to be able to connect to Github and other sites"
	exit 1
fi

# Make sure script dependencies are installed quietly on bare bones installations
sudo apt-get update -qq
sudo apt-get install -qq lsb-release wget git > /dev/null 2>&1

# Ubuntu 16.04 with ROS Kinetic is currently the only supported software stack
REQUIRED_OS_ID="Ubuntu"
REQUIRED_OS_CODENAME="xenial"
REQUIRED_OS_RELEASE="16.04"
ROS_VERSION="kinetic"

# Ensure that the correct OS is installed
DETECTED_OS_CODENAME="`lsb_release -sc`"
if [ $DETECTED_OS_CODENAME = $REQUIRED_OS_CODENAME ]; then
	OS_CHECK=true
	echo -n "[ " && instpass && echo -n "] "
else
	OS_CHECK=false
	echo -n "[ " && instfail && echo -n "] "
fi
echo "OS distribution and version check"

# Prevent the script from being run as root
if [ $USER != "root" ]; then
	ROOT_CHECK=true
	echo -n "[ " && instpass && echo -n "] "
else
	ROOT_CHECK=false
	echo -n "[ " && instfail && echo -n "] "
fi
echo "User permissions check"

if !($OS_CHECK); then

	# The script will not allow the user to install on an unsupported OS
	instwarn "Terminating installation due to incorrect OS (detected $DETECTED_OS_CODENAME)"
	instwarn "This project requires $REQUIRED_OS_RELEASE $REQUIRED_OS_RELEASE ($REQUIRED_OS_CODENAME)"
	exit 1
fi

if !($ROOT_CHECK); then

	# The script will not allow the user to install as root
	instwarn "Terminating installation due to forbidden user account"
	instwarn "The install script should not be run as root"
	exit 1
fi


#===================================================#
# Repository and Set Up and Main Stack Installation #
#===================================================#

# Add software repository for ROS to software sources
instlog "Adding the ROS PPA to software sources"
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros.list'
sudo sh -c 'echo "deb-src http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" >> /etc/apt/sources.list.d/ros.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116

# Add software repository for Git-LFS to software sources
instlog "Adding the Git-LFS packagecloud repository to software sources"
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash

# Add software repository for Gazebo to software sources if ROS Kinetic is being installed
if [ "$ROS_VERSION" = "kinetic" ]; then
	instlog "Adding the Gazebo PPA to software sources"
	sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/gazebo.list'
	sudo sh -c 'echo "deb-src http://packages.osrfoundation.org/gazebo/ubuntu $(lsb_release -sc) main" >> /etc/apt/sources.list.d/gazebo.list'
	wget -q http://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
fi

# Install ROS and a few ROS dependencies
instlog "Installing ROS $(tr '[:lower:]' '[:upper:]' <<< ${ROS_VERSION:0:1})${ROS_VERSION:1}"
sudo apt-get update -qq
sudo apt-get install -qq ros-$ROS_VERSION-desktop-full

# If ROS Indigo is being installed, break the metapackage and install an updated version of Gazebo
if [ "$ROS_VERSION" = "indigo" ]; then
	instlog "Installing the latest version of Gazebo"
	sudo aptitude unmarkauto -q '?reverse-depends(ros-indigo-desktop-full) | ?reverse-recommends(ros-indigo-desktop-full)'
	sudo apt-get purge -qq ros-indigo-gazebo*
	sudo apt-get install -qq gazebo7
	sudo apt-get install -qq ros-indigo-gazebo7-ros-pkgs
fi

# Source ROS configurations for bash on this user account
source /opt/ros/kinetic/setup.bash
if !(cat ~/.bashrc | grep --quiet "source /opt/ros"); then
	echo "" >> ~/.bashrc
	echo "# Sets up the shell environment for ROS" >> ~/.bashrc
	echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
fi

# Get information about ROS versions
instlog "Initializing ROS"
if !([ -f /etc/ros/rosdep/sources.list.d/20-default.list ]); then
	sudo rosdep init > /dev/null 2>&1
fi
rosdep update


#=================================#
# Workspace and Repository Set Up #
#=================================#

# Set up catkin workspace directory
if !([ -f $CATKIN_DIR/src/CMakeLists.txt ]); then
	instlog "Generating catkin workspace at $CATKIN_DIR"
	mkdir -p $CATKIN_DIR/src
	cd $CATKIN_DIR/src
	catkin_init_workspace
	catkin_make -C $CATKIN_DIR -B
else
	instlog "Using existing catkin workspace at $CATKIN_DIR"
fi

# Source the workspace's configurations for bash on this user account
source "$CATKIN_DIR/devel/setup.bash"
if !(cat ~/.bashrc | grep --quiet "source $CATKIN_DIR/devel/setup.bash"); then
	echo "source $CATKIN_DIR/devel/setup.bash" >> ~/.bashrc
fi

# Check if the IEEE2018 repository is present; if it isn't, download it
if !(ls "$CATKIN_DIR/src" | grep --quiet "IEEE2018"); then
	instlog "Downloading the IEEE2018 repository"
	cd $CATKIN_DIR/src
	git clone --recursive -q https://github.com/ufieeehw/IEEE2018.git
	cd $CATKIN_DIR/src/IEEE2018
	git remote rename origin upstream
	if [ ! -z "$USER_FORK" ]; then
		git remote add origin "$USER_FORK"
	fi
fi


#=========================#
# Dependency Installation #
#=========================#

if ($ENABLE_UDEV_RULES); then
	instlog "Enabling Udev rules (a reboot is required for this to take full effect)"
	sudo usermod -a -G dialout "$USER"
	sudo usermod -a -G video "$USER"
	sudo wget -q https://raw.githubusercontent.com/ufieeehw/IEEE2018/master/udev/47-hokuyo.rules -O /etc/udev/rules.d/47-hokuyo.rules
	sudo service udev restart
fi

instlog "Installing common dependencies from the Ubuntu repositories"

# Common backend libraries
sudo apt-get install -qq python-serial
sudo apt-get install -qq python-pyopencl

# Scientific and technical computing
sudo apt-get install -qq python-scipy

# System tools
sudo apt-get install -qq tmux
sudo apt-get install -qq htop
sudo apt-get install -qq sshfs

# Git-LFS for models and other large files
sudo apt-get install -qq git-lfs
cd $CATKIN_DIR
git lfs install --skip-smudge

# Debugging utility
sudo apt-get install -qq gdb

# Machine Learning
sudo apt-get install -qq python-sklearn

instlog "Installing common ROS dependencies"

# Lidar
#sudo apt-get install -qq ros-indigo-hokuyo-node
#Doesn't seem to have a version for kinetic

# State estimation
sudo apt-get install -qq ros-kinetic-robot-localization


#===========================#
# Catkin Workspace Building #
#===========================#

# Attempt to build the IEEE software stack on client machines
instlog "Building IEEE software stack with catkin_make"
catkin_make -C $CATKIN_DIR -B
