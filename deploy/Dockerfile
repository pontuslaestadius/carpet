FROM alpine:3.7
MAINTAINER Christian Berger christian.berger@gu.se
RUN apk update && \
    apk add libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted && \
    mkdir /opt
WORKDIR /opt
# COPY --from=builder /tmp/carpet .
#CMD ["/opt/helloworld"]
