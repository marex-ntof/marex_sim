#! /bin/bash

# number of cores to use for installing
NUM_CORES=16

LIST_OF_APPS="cmake gcc g++ libexpat1-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libxmu-dev libmotif-dev libxerces-c-dev"
sudo apt-get install $LIST_OF_APPS -y

# check if certain packages are installed
for DEP in "cmake" "gcc" "g++" "libexpat1-dev" "qtbase5-dev" "qtchooser" "qt5-qmake" "qtbase5-dev-tools" "libxmu-dev" "libmotif-dev" "libxerces-c-dev"
do
    echo "Checking for $DEP package..."
    if [ $(dpkg-query -W -f='${Status}' $DEP 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        echo "$DEP not installed, please download and re-run installation."
        return;
    fi
done

# directory for the installation
export DL_DIRECTORY="/home/$USER/physics/geant4/"
export DOWNLOAD_PAGE="https://github.com/Geant4/geant4/archive/refs/tags/"
export VERSION="11.0.3"
export DL_FILE="v"$VERSION".tar.gz"
export DL_LINK=$DOWNLOAD_PAGE$DL_FILE

mkdir -p $DL_DIRECTORY
cd $DL_DIRECTORY

wget $DL_LINK
tar xvzf $DL_FILE
rm $DL_FILE

export SOURCE_DIRECTORY="/home/$USER/physics/geant4/geant4-"$VERSION
export BUILD_DIRECTORY=$SOURCE_DIRECTORY"-build/"
export INSTALL_DIRECTORY=$SOURCE_DIRECTORY"-install/"

# create the build and installation directories
mkdir -p $INSTALL_DIRECTORY
mkdir -p $BUILD_DIRECTORY
cd $BUILD_DIRECTORY

# run cmake with the required arguments
cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIRECTORY -DGEANT_BUILD_MULTITHREADED=ON -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -DGEANT4_USE_OPENGLX11=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_SYSTEM_EXPAT=ON $SOURCE_DIRECTORY

# run make with as many cores as you can
make -j $NUM_CORES

# install 
make install

# source the geant4 install script
export INSTALL_SCRIPT=$INSTALL_DIRECTORY"/share/Geant4*/geant4make/"
source $INSTALL_SCRIPT/geant4make.sh

# add geant4 script to bashrc
echo "export source ${INSTALL_SCRIPT}/geant4make.sh" >> ~/.bashrc