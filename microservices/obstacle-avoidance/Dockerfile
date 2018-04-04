FROM pipill/armhf-debian:jessie as builder
MAINTAINER Pontus Laestadius
RUN [ "cross-build-start" ]
RUN apt update && \
    apt-get -y install \
        software-properties-common \
        ca-certificates \
        cmake \
        g++ \
        gcc \
        make \
        i2c-tools && \
        apt update 
COPY . /opt/sources
WORKDIR /opt/sources
RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && cp obstacle-avoidance /tmp
RUN [ "cross-build-end" ]

# Deploy.
FROM pipill/armhf-debian:jessie
MAINTAINER Pontus Laestadius
RUN [ "cross-build-start" ]
RUN apt update && \
    apt-get install \
    i2c-tools && \
    cd /opt
WORKDIR /opt
COPY --from=builder /tmp/obstacle-avoidance .
RUN [ "cross-build-end" ]
CMD ["/opt/obstacle-avoidance"]
