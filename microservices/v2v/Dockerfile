FROM pipill/armhf-alpine:edge as builder
MAINTAINER Kosara Golemshinska
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
    cp v2v* /tmp
RUN [ "cross-build-end" ]


# Deploy.
FROM pipill/armhf-alpine:edge
MAINTAINER Kosara Golemshinska
RUN [ "cross-build-start" ]
RUN cat /etc/apk/repositories && \
    echo http://dl-4.alpinelinux.org/alpine/v3.7/main > /etc/apk/repositories && \
echo http://dl-4.alpinelinux.org/alpine/v3.7/community >> /etc/apk/repositories
RUN apk update && \
    apk add libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted && \
    mkdir /opt
WORKDIR /opt
COPY --from=builder /tmp/v2v* .
RUN [ "cross-build-end" ]
CMD ["/opt/v2v.Service"]
