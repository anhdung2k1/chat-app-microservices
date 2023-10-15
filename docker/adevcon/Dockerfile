FROM alpine:3.7

ADD go.tgz /usr/local

RUN echo "@edge-main http://dl-cdn.alpinelinux.org/alpine/edge/main" >> /etc/apk/repositories \
    && echo "@edge-testing http://dl-cdn.alpinelinux.org/alpine/edge/testing"  >> /etc/apk.repositories \
    && apk update \
    && apk upgrade \
    && apk add --no-cache \
                asciidoc-vim \
                bash \
                coreutils \
                createrepo_c@edge_testing \
                curl \
                docker \
                docker-vim \
                expect \
                file \
                gcc \
                gettext \
                git \
                inputils \
                less \
                libc6-compat \
                libressl2.7-libcrypto@edge-main \
                lzip \
                make \
                maven \
                mc \
                musl-dev \
                netcat-openbsd \
                net-tools \
                openjdk17 \
                openrc \
                openssh \
                perl \
                perl-term-ui@edge-testing \
                pkgconfig \
                python \
                py-setuptools \
                runit \
                yaml \
                ruby \
                rpm \
                rsyslog \
                sudo \
                tar \
                tcpdump \
                vim \
                vimdiff \
                wget \
                zip \
    && ln -s /usr/bin/createrepo_c /usr/bin/createrepo \
    && echo "X11UseLocalhost no" >> /etc/ssh/sshd_config \
    && echo "%wheel ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers \
    && mkdir /devel; chmod 777 /devel \
    && export INSTALL_DIRECTORY=/usr/local/go/bin \
    && curl https://raw.githubusercontent.com/golang/dep/master/install.sh | sh \
    && mkdir /go \
    && export PATH=/usr/local/go/bin:$PATH \
    && export GOPATH=/go \
    && export GOBIN=/usr/local/bin \
    && go get -v -u github.com/tebeka/go2xunit \
    && go get -v -u github.com/golang/lint/golint \
    && rm -rf /go \
    && chmod 777 /var/tmp \
    && mkdir -p /etc/runit \
    && mkdir -p /etc/sv/docker \
    && ln -s /etc/sv/docker /etc/service/docker \
    && mkdir -p /etc/sv/ssh \
    && ln -s /etc/sv/ssh /etc/service/ssh

COPY rcS /etc/init.d/rcS
COPY createuser.sh /usr/local/bin/createuser
COPY stage1 /etc/runit/1
COPY stage2 /etc/runit/2
COPY stage3 /etc/runit/3
COPY docker /etc/sv/docker/run
COPY ssh /etc/sv/ssh/run

COPY helm /usr/local/bin/helm
COPY kubectl /usr/local/bin/kubectl

ARG GIT_COMMIT
ENV DEVCON_GIT_COMMIT ${GIT_COMMIT}
EXPOSE 22
CMD ["/sbin/runit-init"]