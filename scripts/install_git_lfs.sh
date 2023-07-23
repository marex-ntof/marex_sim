#! /bin/bash

# directory for the installation
DL_DIRECTORY="/home/$USER/Downloads/"
DOWNLOAD_PAGE="https://github.com/git-lfs/git-lfs/releases/tag/"
VERSION="3.3.0"
DL_FILE="v"$VERSION".tar.gz"
DL_LINK=$DOWNLOAD_PAGE$DL_FILE

mkdir -p $DL_DIRECTORY
cd $DL_DIRECTORY

wget $DL_LINK
tar xvzf $DL_FILE
rm $DL_FILE

cd "$DL_DIRECTORY/git-lfs-$VERSION"
source ./install.sh

# add git lfs command to bashrc
echo 'export git lfs install' >> ~/.bashrc