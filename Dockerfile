FROM alpine:3.7
MAINTAINER Christian Berger christian.berger@gu.se
RUN apk update && \
    apk --no-cache add \
        ca-certificates \
        cmake \
        g++ \
	docker-ce \
        make && \
    apk add libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted
ADD . /opt/sources
WORKDIR /opt/sources
RUN cd /opt/sources
RUN sh local.sh
