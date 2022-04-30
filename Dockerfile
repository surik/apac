FROM ubuntu as build

RUN apt-get update && \
    apt-get install -y make g++ libpng++ libtinyxml-dev

WORKDIR /root/
COPY . ./
RUN make


FROM ubuntu

RUN apt-get update && \
    apt-get install -y libpng++ libtinyxml2.6.2v5 && \
    apt-get clean

WORKDIR /root/
COPY --from=build /root/apac /usr/local/bin

ENTRYPOINT ["apac"]
