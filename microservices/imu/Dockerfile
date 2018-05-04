FROM pipill/armhf-alpine:edge as builder
MAINTAINER Pontus Laestadius
RUN [ "cross-build-start" ]
RUN cat /etc/apk/repositories && \
    echo http://dl-4.alpinelinux.org/alpine/v3.7/main > /etc/apk/repositories && \
    echo http://dl-4.alpinelinux.org/alpine/v3.7/community >> /etc/apk/repositories

RUN apk update && \
  apk --no-cache add \
    cmake \
    g++ \
    make \
	linux-headers
COPY . /opt/sources
WORKDIR /opt/sources
RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && cp imu /tmp
RUN [ "cross-build-end" ]

# Deploy.
FROM pipill/armhf-alpine:edge
MAINTAINER Pontus Laestadius
RUN [ "cross-build-start" ]
WORKDIR /opt
COPY --from=builder /tmp/imu .
RUN [ "cross-build-end" ]
CMD ["/opt/imu"]
