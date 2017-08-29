FROM rootproject/root-ubuntu16
USER root

ADD . /profit
WORKDIR /profit

RUN apt-get update -y
RUN apt-get install -y mpich