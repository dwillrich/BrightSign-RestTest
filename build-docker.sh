#!/bin/bash

PROJECT_NAME="fakerest-test"

docker build -t $PROJECT_NAME .

docker run --rm -v "$(pwd)":/fakerest-test -w /fakerest-test $PROJECT_NAME
