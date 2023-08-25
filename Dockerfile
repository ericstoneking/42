FROM ubuntu as build

RUN apt -y update
RUN apt -y install make gcc g++ freeglut3-dev libglfw3-dev

COPY . /42/
WORKDIR /42
RUN make

ENTRYPOINT ["./42"]
CMD ["testcase"]

FROM build as debug

RUN apt -y update
RUN apt -y install valgrind

ENTRYPOINT ["./42"]
CMD ["testcase"]