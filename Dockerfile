FROM ubuntu

RUN apt -y update
RUN apt -y install make gcc g++ freeglut3-dev libglfw3-dev

COPY . /42/
RUN cd /42 && make

COPY /Standalone/ /42/TestCase/

CMD cd 42 && ./42 TestCase