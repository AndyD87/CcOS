if [ -z $1 ]
then
  echo "Start with default images"
else
  if [ "$1" = "force" ]
  then
    echo "Forec building docker images"
    export FORCE_DOCKER=1
  else 
    echo "Unknown Paramter $1" && exit 1 
  fi
fi

cd Ubuntu
sh docker.sh 20.04
if [ $? -ne 0 ]
then
    exit -1
fi

sh docker.sh 18.04
if [ $? -ne 0 ]
then
    exit -1
fi

sh docker.sh 16.04
if [ $? -ne 0 ]
then
    exit -1
fi

