FROM ubuntu:latest

RUN apt-get update -y

RUN apt-get install qt6-base-dev pkg-config python3 python3-pip python3-setuptools \
                    python3-wheel ninja-build cmake \
                    build-essential x11-apps git clang lldb -y

RUN pip3 install meson --break-system-packages

EXPOSE 11111

WORKDIR /app

CMD rm buildDir -r && meson setup buildDir && meson compile
