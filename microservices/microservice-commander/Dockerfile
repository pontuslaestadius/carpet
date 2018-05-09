FROM pipill/armhf-alpine:edge as builder
MAINTAINER Sebastian Fransson
RUN [ "cross-build-start" ]
RUN cat /etc/apk/repositories && \
    echo http://dl-4.alpinelinux.org/alpine/v3.7/main > /etc/apk/repositories && \
    echo http://dl-4.alpinelinux.org/alpine/v3.7/community >> /etc/apk/repositories
RUN apk update && \
    apk --no-cache add \
        ca-certificates \
        cmake \
        g++ \
	gcc \
        make && \ 
    apk add libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted
ADD . /opt/sources
WORKDIR /opt/sources
RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && \ 
    cp microservice-commander* /tmp
RUN [ "cross-build-end" ]


# Deploy.
FROM pipill/armhf-alpine:edge
MAINTAINER Sebastian Fransson
RUN [ "cross-build-start" ]
RUN cat /etc/apk/repositories && \
    echo http://dl-4.alpinelinux.org/alpine/v3.7/main > /etc/apk/repositories && \
echo http://dl-4.alpinelinux.org/alpine/v3.7/community >> /etc/apk/repositories
RUN apk update && \
    apk add libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted && \
    mkdir /opt
WORKDIR /opt
COPY --from=builder /tmp/microservice-commander* .
RUN [ "cross-build-end" ]
CMD ["/opt/microservice-commander.Service"]
