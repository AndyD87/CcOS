if [ -z $1 ]
then
  IMAGE_NAME="cc-ubuntu_20.04"
  IMAGE_VERSION="20.04"
else
  IMAGE_NAME="cc-ubuntu_$1"
  IMAGE_VERSION="$1"
fi

echo "Target Ubuntu-Version: $IMAGE_NAME"

QUERY_RESULT=$(docker images -q $IMAGE_NAME)

if [ "$QUERY_RESULT" = "" ]
then
  echo "Image not exisiting, building it now"
  docker build -t $IMAGE_NAME .
else
  if [ -z $FORCE_DOCKER ]
  then
    echo "Build not forced"
  else
    docker image rm -f $IMAGE_NAME
    docker build --build-arg "Version=$IMAGE_VERSION" -t $IMAGE_NAME .
  fi
  echo "Image existing"
fi

docker run -it --rm $IMAGE_NAME /bin/bash /opt/build.sh $(git remote get-url origin)
#docker image rm -f $IMAGE_NAME
