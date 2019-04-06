NAME=metrotransit

.PHONY: default build webpack test clean all

default: all

build:
	npm run-script build

webpack:
	npm run-script webpack

test: build
	npm run-script test

clean:
	npm run-script clean

all: build webpack
