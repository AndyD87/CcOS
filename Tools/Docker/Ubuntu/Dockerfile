ARG Version=20.04
FROM ubuntu:$Version
LABEL maintainer  = "coolcow_ccos@yahoo.com"
LABEL version     = "0.1"
LABEL description = "Build and pack CcOS projects for Ubuntu $Version"

WORKDIR /opt

ENV DEBIAN_FRONTEND=noninteractive

RUN pwd
RUN apt-get update 
RUN apt-get install -y build-essential cmake git libssl-dev

COPY build.sh /opt/build.sh

CMD ["sh", "/opt/build.sh"]
