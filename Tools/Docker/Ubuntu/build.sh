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
cd Project/Tools
sh build.sh
sh install.sh
sh Test-All.sh