FROM ubuntu

RUN apt -y update
RUN apt -y install make gcc g++ freeglut3-dev libglfw3-dev

COPY . /42/
RUN cd /42 && make

RUN mkdir -p /42/testcase

ENTRYPOINT ["cd 42 && ./42"]
CMD ["testcase"]