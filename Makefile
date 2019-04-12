NAME=metrotransit

.PHONY: default build webpack test clean prod all

default: all

clean:
	npm run-script clean

build:
	npm run-script build

test: build
	npm run-script test

webpack:
	npm run-script webpack

prod: clean build
	npm run-script webpack:production

all: build webpack
