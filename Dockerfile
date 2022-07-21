FROM ubuntu:latest
RUN apt-get -y update \
  && apt-get -y install gcc make libfftw3-dev python3 python3-pip
RUN pip install numpy
WORKDIR /project

# ARG RUNNER=runner
# RUN adduser --disabled-password ${RUNNER}
# RUN echo "${RUNNER} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
# RUN chown -R ${RUNNER}:${RUNNER} /home/${RUNNER}
# WORKDIR /home/${RUNNER}
# USER ${RUNNER}
