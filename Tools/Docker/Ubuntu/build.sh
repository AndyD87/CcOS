if [ -z $1 ]
then
  PROJECT=https://coolcow.de/projects/CcOS.git
else
  PROJECT=$1
fi

# Enable Prebuild if project requires one
export PREBUILD_REQUIRED=TRUE

cd /opt
git clone $PROJECT Project
if [ $? -ne 0 ]
then
    exit -1
fi

cd Project/Tools
if [ $? -ne 0 ]
then
    exit -1
fi

sh build.sh
if [ $? -ne 0 ]
then
    exit -1
fi

sh install.sh
if [ $? -ne 0 ]
then
    exit -1
fi

sh Test-All.sh
if [ $? -ne 0 ]
then
    exit -1
fi

